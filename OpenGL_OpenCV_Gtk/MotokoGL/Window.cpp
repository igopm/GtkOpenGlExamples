#include <iostream>

#include "fatal.h"

#include "Window.h"

namespace MotokoGL{
Window::Window(int w, int h, const char *title)
{
    // Init GLFW
    glfwInit();

    // Error callback should be set early
    glfwSetErrorCallback([](int e, const char *msg) {
        std::cerr << "GLFW error : " << e << msg << std::endl;
    });

    // Set parameters
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Do we need it ?

    // Create GLFW window
    win = glfwCreateWindow(w, h, title, nullptr, nullptr);
    if (win == nullptr)
        fatal("Cannot create GLFW window !");

    glfwMakeContextCurrent(win); // Doesn't work without it ;)

    // Init GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        fatal("Cannot initialize GLEW !");

    // From now on
    // Set viewport (should be equal to the window size)
    glfwGetFramebufferSize(win, &width, &height);

    // The first OpenGL function call: init viewport
    glViewport(0, 0, width, height);
}

}
