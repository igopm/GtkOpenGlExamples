//
// Created by Igor Maschikevich on 10/20/2017.
//
//=====================================================================================================
#include <iostream>
#include "openglWindow.h"
#include "Shader.h"
//=====================================================================================================
using std::cerr;
using std::endl;
using std::string;
//=====================================================================================================
//=====================================================================================================
OpenglWindow::OpenglWindow():
    VBox(Gtk::ORIENTATION_VERTICAL, false),
    ControlBox(Gtk::ORIENTATION_HORIZONTAL, false),
    button_close("_Close", /* mnemonic= */ true),
    button_one("_one", true),
    button_two("_two", true)
{
    //========================================================================================
    int const margin = 5;
    VBox.property_margin() = margin;
    VBox.set_spacing(margin);
    ControlBox.set_spacing(margin);
    button_close.set_halign(Gtk::ALIGN_END);
    button_close.set_valign(Gtk::ALIGN_CENTER);
    //========================================================================================
    set_title("GL Area");
    set_default_size(640, 480);
    //========================================================================================
    GLArea.set_hexpand(true);
    GLArea.set_vexpand(true);
    GLArea.set_auto_render(true);
    VBox.add(GLArea);

    // Connect gl area signals
    GLArea.signal_realize().connect(sigc::mem_fun(*this, &OpenglWindow::realize));
    GLArea.signal_unrealize().connect(sigc::mem_fun(*this, &OpenglWindow::unrealize), false);
    GLArea.signal_render().connect(sigc::mem_fun(*this, &OpenglWindow::render), false);
    //========================================================================================
    add(VBox);
    VBox.add(ControlBox);
    ControlBox.set_hexpand(true);
    button_close.set_hexpand(true);
    ControlBox.add(button_one);
    ControlBox.add(button_two);
    ControlBox.add(button_close);
    //========================================================================================
    button_one.signal_clicked().connect( sigc::mem_fun(*this,
                                                       &OpenglWindow::on_button_one));
    button_two.signal_clicked().connect( sigc::mem_fun(*this,
                                                       &OpenglWindow::on_button_two));
    button_close.signal_clicked().connect( sigc::mem_fun(*this,
                                                         &OpenglWindow::on_button_close));
    //========================================================================================
    show_all_children();
}
//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
void OpenglWindow::realize() {
    std::cout << "onRealize()" << std::endl;

    GLArea.make_current();

    glewExperimental = true; //GLArea only support Core profile.
    if(glewInit() != GLEW_OK) {
        std::cout <<"ERROR: Failed to initialize GLEW. (Maybe the OpenGL context not exist)"<<std::endl;
    }

    //        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    init_buffers();
    init_shaders();

}
void OpenglWindow::init_buffers(){
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    // VBO, VAO
    //    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO first
    glBindVertexArray(VAO);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // VBO to VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO

}
void OpenglWindow::unrealize() {
    //your cleanUp. Deleting Vao etc.
}
static GLuint create_shader(int type, const char *src)
{
    auto shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        int log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

        string log_space(log_len+1, ' ');
        glGetShaderInfoLog(shader, log_len, nullptr, (GLchar*)log_space.c_str());

        cerr << "Compile failure in " <<
                (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
                " shader: " << log_space << endl;

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}
void OpenglWindow::init_shaders()
{

    const GLchar * vShaderSrc =
            "#version 330 core\n"
            "layout (location=0) in vec3 pos;\n"
            "void main(){\n"
            "gl_Position=vec4(pos.x, pos.y, pos.z, 1.0);}\n\0";

    const GLchar * fShaderSrc =
            "#version 330 core\n"
            "out vec4 color;\n"
            "void main(){\n"
            "color=vec4(1.0f, 0.9f, 0.0f, 1.0f);}\n\0";


    auto vertex = create_shader(GL_VERTEX_SHADER, vShaderSrc);

    if(vertex == 0)
    {
        m_Program = 0;
        return;
    }

    auto fragment = create_shader(GL_FRAGMENT_SHADER, fShaderSrc);

    if(fragment == 0)
    {
        glDeleteShader(vertex);
        m_Program = 0;
        return;
    }

    m_Program = glCreateProgram();
    glAttachShader(m_Program, vertex);
    glAttachShader(m_Program, fragment);

    glLinkProgram(m_Program);

    int status;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        int log_len;
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &log_len);

        string log_space(log_len+1, ' ');
        glGetProgramInfoLog(m_Program, log_len, nullptr, (GLchar*)log_space.c_str());

        cerr << "Linking failure: " << log_space << endl;

        glDeleteProgram(m_Program);
        m_Program = 0;
    }
    else
    {
        /* Get the location of the "mvp" uniform */
        m_Mvp = glGetUniformLocation(m_Program, "mvp");

        glDetachShader(m_Program, vertex);
        glDetachShader(m_Program, fragment);
    }
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}


bool OpenglWindow::render(const Glib::RefPtr<Gdk::GLContext>&  /* context */) {
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_model();

}

void OpenglWindow::draw_model(){
//    glClearColor(0.5, 1.0, 1.0, 1.0);
    glUseProgram(m_Program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
//=====================================================================================================
void OpenglWindow::on_button_one(){
    std::cout << "one" << std::endl;
}
void OpenglWindow::on_button_two(){
    std::cout << "two" << std::endl;
}
void OpenglWindow::on_button_close(){
    hide(); //to close the application.
}
//=====================================================================================================
