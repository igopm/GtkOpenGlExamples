//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#pragma once
//#include <iostream>
//#include <string>
//#define GLEW_STATIC
//#include <GL/glew.h>
#include "GtkGL.h"

#include <gtkmm/box.h>
#include <gtkmm/glarea.h>
//#include <gtkmm/widget.h>
//#include <gtkmm/button.h>
//#include <opencv2/opencv.hpp>
#include "VideoThread.h"

//=====================================================================================================
class OpenglWindow : public Gtk::Box {
public:
    OpenglWindow();
    virtual ~OpenglWindow();
protected:
    //    opengl
    //========================================================================================
    Gtk::GLArea GLArea;
    void onRealize();
    void onUnrealize();
    bool onRender(const Glib::RefPtr<Gdk::GLContext> &context);
    //========================================================================================
    static gboolean frame_callback(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer data);
    void init_buffers();
    void init_shaders();
    void draw_bg();
    void draw_cub();
    //========================================================================================
    GLuint texture;
    const char* setImage = "common_textures/container2.jpg";
    GLuint VBOs[2], VAOs[2], EBO;
    GtkGL::Shader shader_bg;
    GtkGL::Shader shader_cub;
    //========================================================================================
    //    GtkGL::Tex texBG;
    void init_opencv();
    //    cv::VideoCapture cap{0};
    //    cv::Mat frame;
    //========================================================================================
    VideoThread thread_video;
    void pong();
    //========================================================================================
};
