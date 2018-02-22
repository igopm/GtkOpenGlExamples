//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#include <iostream>
#include <chrono>
#include <thread>
//=====================================================================================================
#include "OpenglWindow.h"
//=====================================================================================================
using namespace glm;
using namespace GtkGL;
using namespace std;
using namespace std::chrono;
//=====================================================================================================
using DSeconds = duration<double>;
auto tp0 = high_resolution_clock::now();
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
    //======================================================================================
    button_one.signal_clicked().connect( sigc::mem_fun(*this,
                                                       &OpenglWindow::on_button_one));
    button_two.signal_clicked().connect( sigc::mem_fun(*this,
                                                       &OpenglWindow::on_button_two));
    button_three.signal_clicked().connect( sigc::mem_fun(*this,
                                                         &OpenglWindow::on_button_three));
    //======================================================================================
    pack_end(GLArea, Gtk::PACK_EXPAND_WIDGET);
    show_all();
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
    //======================================================================================
    int ind;
    const cv::Mat &frame =  thread_video.buf1.startReading(ind); // Try to read
    if (-1 != ind) {
        texBg.updateImage(frame);// Update texture if a frame is available, non-blocking
        thread_video.buf1.endReading(ind);
    }
    //======================================================================================
    drawBg();
    drawCube();
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
    initBuffers();
    initShaders();
    initOpencv();
    gtk_widget_add_tick_callback(
                (GtkWidget *) this->gobj(),
                OpenglWindow::frame_callback,
                this,
                NULL);
}
//=====================================================================================================
void OpenglWindow::onUnrealize() {
    GLArea.make_current();
    try {
        GLArea.throw_if_error();
    }
    catch(const Gdk::GLError& gle){
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
void OpenglWindow::initOpencv(){
    std::cout << "OpenCV version : " << CV_VERSION << std::endl;
    cv::Mat firstFrame;
    while (true) {
        int ind;
        firstFrame = thread_video.buf1.startReading(ind);
        // Wait until we get a frame
        if (ind != -1) {
            thread_video.buf1.endReading(ind);
            break;
        }  else
            std::this_thread::sleep_for(milliseconds(10)); // No data available for now, we wait
    }
    texBg = Tex::fromMat(firstFrame);
    texBg.setToNearest();
    //====================================================================
    texCube.Tex::fromMat(firstFrame);
    texCube.setToNearest();
    camera.aspect = aspectRatio;
}
//=====================================================================================================
void OpenglWindow::drawBg(){
    shaderBg.use();
    texBg.bind(0);
    glDepthMask(GL_FALSE);
    vaoBg.draw();
}
//=====================================================================================================
void OpenglWindow::drawCube(){
    //====================================================================
    auto tp = high_resolution_clock::now();
    double t = duration_cast<DSeconds>(tp - tp0).count();
    //====================================================================
    glDepthMask(GL_TRUE);
    //====================================================================
    //     Set up the camera pos if needed
    camera.pos = vec3(2.0f, 2.0f, 7.0f);
    // Camera matrix
    mat4 model;
    mat4 view;
    mat4 projection;
    //===== Render the cube
    shaderCube.use();
    //====================================================================
    switchTexture();
    //====================================================================
    model = mat4();
    model = rotate(model, ((float) t) * sqrtf(0.5f), vec3(0.5f, 1.0f, 0.0f));
    projection = camera.matrix();

    GLint modelLoc = glGetUniformLocation(shaderCube.prog, "model");
    GLint viewLoc = glGetUniformLocation(shaderCube.prog, "view");
    GLint projLoc = glGetUniformLocation(shaderCube.prog, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));
    //         Draw container
    vaoCube.draw();
    //====================================================================
}
//=====================================================================================================
void OpenglWindow::initShaders(){
    shaderBg = Shader::fromFiles("shaders/bg.vert","shaders/bg.frag" );
    shaderCube = Shader::fromFiles("shaders/cub.vert","shaders/cub.frag" );
}
//=====================================================================================================
void OpenglWindow::initBuffers(){
    //====================================================================
    GLfloat verticesBG[] = {
        1.0f,   1.0f,    0.0f,  1.0,  1.0,  // UR
        1.0f,   -1.0f,   0.0f,  1.0,  0.0,  // LR
        -1.0f,  -1.0f,   0.0f,  0.0,  0.0,  // LL
        -1.0f,   1.0f,   0.0f,  0.0,  1.0   // UL
    };
    GLuint indicesBG[] = {
        0, 1, 3,
        1, 2, 3
    };
    //====================================================================
    GLfloat verticesCube[] = {
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
    vaoBg = Vao::create({3, 0, 2}, verticesBG, sizeof(verticesBG), indicesBG, sizeof(indicesBG));
    vaoCube = Vao::create({3, 0, 2}, verticesCube, sizeof(verticesCube));
    //====================================================================
}
//=====================================================================================================
OpenglWindow::~OpenglWindow(){
}
//=====================================================================================================
void OpenglWindow::on_button_one(){
    st = false;
    updateTexture("common_textures/container.jpg");
}
void OpenglWindow::on_button_two(){
    st = false;
    updateTexture("common_textures/container2.jpg");
}
void OpenglWindow::on_button_three(){
    st = true;
}
//=====================================================================================================
void OpenglWindow::updateTexture(const char* setTexture){
    cv::Mat imgCV = cv::imread(setTexture);
    texCube = Tex::fromMat(imgCV);
}
void OpenglWindow::switchTexture(){
    st == true ? texBg.bind(0) : texCube.bind(0);
}
//=====================================================================================================
