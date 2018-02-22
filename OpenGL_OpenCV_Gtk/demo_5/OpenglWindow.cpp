//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#include <iostream>
#include <chrono>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//=====================================================================================================
#include "OpenglWindow.h"
//=====================================================================================================
using namespace glm;
using namespace GtkGL;
using namespace std::chrono;
//=====================================================================================================
using DSeconds = duration<double>;
auto tp0 = high_resolution_clock::now();
//=====================================================================================================
const GLuint WindowWidth = 540;
const GLuint WindowHeight = 380;
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
    //====================================================================
    thread_video.signal_increment.connect(sigc::mem_fun(*this, &OpenglWindow::pong));
    //====================================================================
    pack_end(GLArea, Gtk::PACK_EXPAND_WIDGET);
    show_all();
}
//=====================================================================================================
void OpenglWindow::pong(){
    //    std::cerr << __PRETTY_FUNCTION__ << " called by " << Glib::Thread::self() << " value = "  << "\n";
    //        Gtk::Main::quit();
}
//=====================================================================================================
bool OpenglWindow::onRender(const Glib::RefPtr<Gdk::GLContext>& /*context*/) {
    GLArea.throw_if_error();
    int errorCode = glGetError();
    if (errorCode > 0) {
        std::cout << "MARIA_ERROR :" << errorCode << std::endl;
    }
    glClearColor(0.5, 1.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_bg();
    draw_cub();
    glFlush();
    return true;
}
//=====================================================================================================
void OpenglWindow::onRealize(){
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
    init_opencv();
    gtk_widget_add_tick_callback(
                (GtkWidget *) this->gobj(),
                OpenglWindow::frame_callback,
                this,
                NULL);

}
//=====================================================================================================
void OpenglWindow::onUnrealize() {
    GLArea.make_current();
    try
    {
        GLArea.throw_if_error();
        glDeleteVertexArrays(2, VAOs);
        glDeleteBuffers(2, VBOs);
        glDeleteBuffers(1, &EBO);
    }
    catch(const Gdk::GLError& gle)
    {
        std::cerr << "An error occured making the context current during unrealize" << std::endl;
        std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}
//=====================================================================================================
gboolean OpenglWindow::frame_callback(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer data){
    OpenglWindow *self = (OpenglWindow *) data;
    self->GLArea.queue_draw();
    return G_SOURCE_CONTINUE;
}
//=====================================================================================================
void OpenglWindow::init_opencv(){
    std::cout << "OpenCV version : " << CV_VERSION << std::endl;
    //    //    cap.read(frame);
    //    //    texBG = Tex::fromMat(frame);
    //    //    texBG.setToNearest();
}
//=====================================================================================================
void OpenglWindow::draw_bg(){
    shader_bg.use();
    //    cap.read(frame);
    //    texBG.updateImage(frame);
    //    texBG.bind(0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAOs[0]);
    glDepthMask(GL_FALSE);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
//=====================================================================================================
void OpenglWindow::draw_cub(){
    //====================================================================
    auto tp = high_resolution_clock::now();
    double t = duration_cast<DSeconds>(tp - tp0).count();
    //====================================================================
    glDepthMask(GL_TRUE);
    //    //====================================================================
    shader_cub.use();
    glBindTexture(GL_TEXTURE_2D, texture);
    //    texBG.bind(0);
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    model = glm::rotate(model, ((float) t) * sqrtf(0.5f), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

    GLint modelLoc = glGetUniformLocation(shader_cub.prog, "model");
    GLint viewLoc = glGetUniformLocation(shader_cub.prog, "view");
    GLint projLoc = glGetUniformLocation(shader_cub.prog, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    // Draw container
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    //====================================================================
}
//=====================================================================================================
void OpenglWindow::init_shaders(){
    shader_bg = Shader::fromFiles("shaders/bg.vert","shaders/bg.frag" );
    shader_cub = Shader::fromFiles("shaders/cub.vert","shaders/cub.frag" );
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
    GLfloat verticCub[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    //====================================================================
    glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);
    //====================================================================
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBG), verticesBG, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBG), indicesBG, GL_STATIC_DRAW);
    //     Position attribute
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
    glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticCub), verticCub, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    //    //====================================================================
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
    unsigned char* image = SOIL_load_image(setImage,
                                           &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily m
}
//=====================================================================================================
OpenglWindow::~OpenglWindow()
{
}
//=====================================================================================================
