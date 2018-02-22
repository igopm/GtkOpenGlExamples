//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#include <iostream>
#include <chrono>
#include <SOIL.h>
//=====================================================================================================
#include "OpenglWindow.h"
#include "SimpleCamera.h"
#include "Tex.h"
//=====================================================================================================
using namespace glm;
using namespace std::chrono;
using namespace cv;
//=====================================================================================================
using DSeconds = duration<double>;
auto tp0 = high_resolution_clock::now();
//=====================================================================================================

//=====================================================================================================

//=====================================================================================================
OpenglWindow::OpenglWindow()
{
    set_has_window(false);
    GLArea.set_auto_render(true);
    GLArea.set_has_depth_buffer	(true);
    GLArea.set_hexpand();
    GLArea.set_vexpand();
    GLArea.set_halign(Gtk::ALIGN_FILL);
    GLArea.set_valign(Gtk::ALIGN_FILL);
    GLArea.set_size_request(WindowWidth, WindowHeight);
    GLArea.set_required_version(3, 3); //your desired gl version

    GLArea.signal_realize().connect(sigc::mem_fun(this, &OpenglWindow::onRealize));
    GLArea.signal_unrealize().connect(sigc::mem_fun(this, &OpenglWindow::onUnrealize), false);
    GLArea.signal_render().connect(sigc::mem_fun(this, &OpenglWindow::onRender));
    pack_end(GLArea, Gtk::PACK_EXPAND_WIDGET);
    show_all();
}
//=====================================================================================================
bool OpenglWindow::onRender(const Glib::RefPtr<Gdk::GLContext>& context) {
    GLArea.throw_if_error();

    glClearColor(0.5, 1.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_bg();
    glFlush();
    return true;
}
//=====================================================================================================
void OpenglWindow::onRealize() {
    GLArea.make_current();
    glewExperimental = true; //GLArea only support Core profile.
    if(glewInit() != GLEW_OK) {
        std::cout <<"ERROR: Failed to initialize GLEW. (Maybe the OpenGL context not exist)"<<std::endl;
    }
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION);

    if(renderer && version) {
        std::cout
                << "Realizing GL context"
                << "\n\t" << renderer
                << "\n\t" << version
                << std::endl;
    }
    init_buffers();
    init_shaders();
    gtk_widget_add_tick_callback(
                (GtkWidget *) this->gobj(),
                OpenglWindow::frame_callback,
                this,
                NULL);
    init_opencv();

}
//=====================================================================================================
void OpenglWindow::onUnrealize() {
    GLArea.make_current();
    try
    {
        GLArea.throw_if_error();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    catch(const Gdk::GLError& gle)
    {
        std::cerr << "An error occured making the context current during unrealize" << std::endl;
        std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}
//=====================================================================================================
void OpenglWindow::init_opencv(){
    //    camera.aspect = aspectRatio;
    cap.read(frame);
    //    imshow("frame", frame);
    texBG = Tex::fromMat(frame);
    texBG.setToNearest();
}
//=====================================================================================================
gboolean OpenglWindow::frame_callback(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer data){
    OpenglWindow *self = (OpenglWindow *) data;
    self->GLArea.queue_draw();
    return G_SOURCE_CONTINUE;
}
//=====================================================================================================
void OpenglWindow::draw_bg(){
    shaderBg.use();
    cap.read(frame);
    texBG.updateImage(frame);
    texBG.bind(0);
    glBindVertexArray(VAO);
    glDepthMask(GL_FALSE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
//=====================================================================================================
void OpenglWindow::init_shaders(){
    shaderBg = Shader::fromFiles("shaders/bg.vert","shaders/bg.frag" );
}
//=====================================================================================================
void OpenglWindow::init_buffers(){
    //====================================================================
    // Square, with EBO
    GLfloat verticesBG[] = {
        // Positions          // Colors           // Texture Coords
        1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
        1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };
    GLuint indicesBG[] = {
        0, 1, 3
        , 1, 2, 3
    };
    //====================================================================
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //====================================================================
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBG), verticesBG, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBG), indicesBG, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    //====================================================================
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image("common_textures/container.jpg",
                                           &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily m
}
