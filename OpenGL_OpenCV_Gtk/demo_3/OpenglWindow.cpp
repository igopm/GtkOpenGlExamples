//
// Created by Igor Maschikevich on 10/21/2017.
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
#include "Shader.h"
//=====================================================================================================
using std::cerr;
using std::endl;
using std::string;
//=====================================================================================================
using namespace glm;
using namespace std::chrono;
//=====================================================================================================
using DSeconds = duration<double>;
auto tp0 = high_resolution_clock::now();
//=====================================================================================================
const GLuint WindowWidth = 540;
const GLuint WindowHeight = 380;
//=====================================================================================================
OpenglWindow::OpenglWindow():
    VBox(Gtk::ORIENTATION_VERTICAL, false),
    ControlBox(Gtk::ORIENTATION_HORIZONTAL, false),
    button_close("_Close", /* mnemonic= */ true),
    button_one("_one", true),
    button_two("_two", true)
{
    //========================================================================================
    set_title("demo_3");
    //========================================================================================
    int const margin = 5;
    VBox.property_margin() = margin;
    VBox.set_spacing(margin);
    ControlBox.set_spacing(margin);
    ControlBox.set_hexpand(true);
    button_close.set_hexpand(true);
    button_close.set_halign(Gtk::ALIGN_END);
    button_close.set_valign(Gtk::ALIGN_CENTER);
    //========================================================================================
    GLArea.set_auto_render(true);
    GLArea.set_has_depth_buffer	(true);

    GLArea.set_hexpand();
    GLArea.set_vexpand();
    GLArea.set_halign(Gtk::ALIGN_FILL);
    GLArea.set_valign(Gtk::ALIGN_FILL);
    GLArea.set_size_request(WindowWidth, WindowHeight);
    GLArea.activate();
    GLArea.set_required_version(3, 3); //your desired gl version
    // Connect gl area signals
    GLArea.signal_realize().connect(sigc::mem_fun(*this, &OpenglWindow::realize));
    GLArea.signal_unrealize().connect(sigc::mem_fun(*this, &OpenglWindow::unrealize), false);
    GLArea.signal_render().connect(sigc::mem_fun(*this, &OpenglWindow::render), false);
    //========================================================================================
    add(VBox);
    VBox.add(GLArea);
    VBox.add(ControlBox);
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
void OpenglWindow::realize() {
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
    //    glEnable(GL_DEPTH_TEST);

    init_buffers();
    initShader();
    //    myinitShader();
    gtk_widget_add_tick_callback(
                (GtkWidget *) this->gobj(),
                OpenglWindow::frame_callback,
                this,
                NULL);

}
gboolean OpenglWindow::frame_callback(GtkWidget *widget, GdkFrameClock *frame_clock, gpointer data)
{
    OpenglWindow *self = (OpenglWindow *) data;
    self->GLArea.queue_draw();
    return G_SOURCE_CONTINUE;
}
//=====================================================================================================
void OpenglWindow::unrealize() {
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
        cerr << "An error occured making the context current during unrealize" << endl;
        cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
    }
}
//=====================================================================================================
bool OpenglWindow::render(const Glib::RefPtr<Gdk::GLContext>& context) {

    GLArea.throw_if_error();
    glClearColor(0.5, 1.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_model();
    glFlush();
    return true;
}
//=====================================================================================================
void OpenglWindow::draw_model(){
    // Create transformations
    auto tp = high_resolution_clock::now();
    double t = duration_cast<DSeconds>(tp - tp0).count();

    glClearColor(0.5, 1.0, 0.5, 1.0);

    glUseProgram(shaderProgram);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);
    //    // Create transformations
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    model = glm::rotate(model, ((float) t) * sqrtf(0.5f), glm::vec3(0.5f, 1.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw container
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);
    //====================================================================
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
//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
//=====================================================================================================
void OpenglWindow::initShader(){
    // Build and compile our shader program
    // Vertex shader
    const GLchar* vertexShaderSource = "#version 330 core\n"
                                       "layout (location = 0) in vec3 pos;\n"
                                       "layout (location = 2) in vec2 texCoord;\n"
                                       "out vec2 pTexCoord;\n"
                                       "uniform mat4 model;\n"
                                       "uniform mat4 view;\n"
                                       "uniform mat4 projection;\n"

                                       "void main(){\n"
                                       "gl_Position = projection * view * model * vec4(pos, 1.0f);\n"
                                       "pTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);}\n\0";

    const GLchar* fragmentShaderSource = "#version 330 core\n"
                                         "in vec2 pTexCoord;\n"
                                         "out vec4 color;\n"
                                         "uniform sampler2D ourTexture;\n"
                                         "void main(){\n"
                                         "color = texture(ourTexture, pTexCoord);}\n\0";
    //    Build and compile our shader program
    //            Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
//=====================================================================================================
//void OpenglWindow::myinitShader(){
//    ssss = Shader::fromFiles("shaders/bg.vert", "shaders/bg.frag");
//}
void OpenglWindow::init_buffers(){
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticCub), verticCub, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO

    // Load and create a texture
    //    GLuint texture;
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
    unsigned char* image = SOIL_load_image("common_textures/container2.jpg",
                                           &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily m
}
