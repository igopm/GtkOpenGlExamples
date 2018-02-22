//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#pragma once
#include <iostream>
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>

#include <gtkmm/box.h>
#include <gtkmm/glarea.h>
#include <gtkmm/widget.h>
#include <opencv2/opencv.hpp>
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
    void draw_bg();
    void draw_cub();
    static std::shared_ptr<std::string> parseFile(const std::string & fileName);
    static fromFiles(const std::string & vertexFileName, const std::string & fragmentFileName);
    //========================================================================================
    Gtk::GLArea GLArea;
    GLuint texture;
    GLuint VBOs[2], VAOs[2], EBO;
    Shader shaderBg;
    Shader shaderCub;
    //========================================================================================
    Tex texBG;
    void init_opencv();
    cv::VideoCapture cap{0};
    cv::Mat frame;
    //========================================================================================
};
