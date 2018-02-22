//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#pragma once
#include <gtkmm/box.h>
#include <gtkmm/glarea.h>
#include <gtkmm/widget.h>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SimpleCamera.h"
#include "Shader.h"
#include "Tex.h"
//=====================================================================================================
class OpenglWindow : public Gtk::Box{
public:
    OpenglWindow();
protected:
    //    opengl
    //========================================================================================
    void onRealize();
    void onUnrealize();
    bool onRender(const Glib::RefPtr<Gdk::GLContext> &context);
    //========================================================================================
    static gboolean frame_callback(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer data);
    void init_buffers();
    void init_shaders();
    void init_opencv();
    void draw_bg();
    //========================================================================================
    GLuint texture;
    Gtk::GLArea GLArea;
    GLuint VBO, VAO, EBO;
    Shader shaderBg;

    //========================================================================================
    //    SimpleCamera camera;
    Tex texBG;
    cv::VideoCapture cap{0};
    cv::Mat frame;
    //========================================================================================
private:
    const GLuint WindowWidth = 540;
    const GLuint WindowHeight = 380;
    //    const float aspectRatio = 1.0f * WindowWidth / WindowHeight;
};
