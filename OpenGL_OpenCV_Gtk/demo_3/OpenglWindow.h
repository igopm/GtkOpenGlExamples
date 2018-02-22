//
// Created by Igor Maschikevich on 10/21/2017.
//
//=====================================================================================================
#pragma once
#include <gtkmm.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "Shader.h"
//=====================================================================================================
class OpenglWindow : public Gtk::Window{
public:
    OpenglWindow();
protected:
    //    opengl
    //========================================================================================
    void realize();
    void unrealize();
    bool render(const Glib::RefPtr<Gdk::GLContext>& context);
    //========================================================================================
    void init_buffers();
    void initShader();
    void myinitShader();
    void draw_model();
    //========================================================================================
    //Signal handlers:
    void on_button_one();
    void on_button_two();
    void on_button_close();
    static gboolean frame_callback(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer data);
    //========================================================================================
    GLuint texture;
    GLuint VAO {0};
    GLuint VBO {0};
    GLuint EBO {0};
    GLuint m_Mvp {0};
    GLuint shaderProgram{0};
    //========================================================================================
    Gtk::Box VBox;
    Gtk::GLArea GLArea;
    Gtk::Box ControlBox;
    Gtk::Button button_close;
    Gtk::Button button_one;
    Gtk::Button button_two;
    //========================================================================================
};
