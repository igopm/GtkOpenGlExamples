//
// Created by Igor Maschikevich on 10/25/2017.
//
//=====================================================================================================
#pragma once
#include <gtkmm/box.h>
#include <gtkmm/glarea.h>

#include "GtkGL.h"
#include "VideoThread.h"
//=====================================================================================================
class OpenglWindow : public Gtk::Box {
public:
    OpenglWindow();
    Gtk::Button button_one{"_one", true};
    Gtk::Button button_two{"_two", true};
    Gtk::Button button_three{"_three", true};
    virtual ~OpenglWindow();
    //========================================================================================
    void on_button_one();
    void on_button_two();
    void on_button_three();
    //========================================================================================
protected:
    //========================================================================================
    Gtk::GLArea GLArea;
    void onRealize();
    void onUnrealize();
    bool onRender(const Glib::RefPtr<Gdk::GLContext> &context);
    //========================================================================================
    static gboolean frame_callback(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer data);
    void drawBg();
    void drawCube();
    //========================================================================================
    GtkGL::Vao vaoBg;
    GtkGL::Vao vaoCube;
    GtkGL::Shader shaderBg;
    GtkGL::Shader shaderCube;
    //========================================================================================
    GtkGL::Tex texBg;
    GtkGL::Tex texCube;
    const GLuint WindowWidth = 640;
    const GLuint WindowHeight = 480;
    const float aspectRatio = 1.0f * WindowWidth / WindowHeight;
    GtkGL::SimpleCamera camera;

    void initOpencv();
    //========================================================================================
    VideoThread thread_video;
    //========================================================================================
private:
    void initBuffers();
    void initShaders();
    std::atomic<bool> st{true};
    void switchTexture();
    void updateTexture(const char* setTexture);
};
