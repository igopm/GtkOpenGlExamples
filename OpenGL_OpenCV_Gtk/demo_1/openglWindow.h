//
// Created by Igor Maschikevich on 10/20/2017.
//
//=====================================================================================================
#pragma once
#include <gtkmm.h>
#define GLEW_STATIC
#include <GL/glew.h>
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
    void init_shaders();
    void draw_model();
    //========================================================================================
    //Signal handlers:
    void on_button_one();
    void on_button_two();
    void on_button_close();
    //========================================================================================
    GLuint VAO {0};
    GLuint VBO {0};
    GLuint m_Program {0};

    //    GLuint VBO, VAO;
    GLuint m_Mvp {0};
    std::vector<float> m_RotationAngles;
    //========================================================================================
    Gtk::Box VBox;
    Gtk::GLArea GLArea;
    Gtk::Box ControlBox;
    Gtk::Button button_close;
    Gtk::Button button_one;
    Gtk::Button button_two;
    //========================================================================================
};
