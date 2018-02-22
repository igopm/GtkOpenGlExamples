#pragma once
// By Oleksiy Grechnyev

#include "./glheader.h"

namespace MotokoGL{
/// OpenGL Window
class Window
{
public: //======== Methods
    /// Constructor
    Window(int w, int h, const char *title);

    /// Swap the buffers
    void swapBuffers() {
        glfwSwapBuffers(win);
    }

    /// Should this window close?
    bool shouldClose() const{
        return glfwWindowShouldClose(win);
    }

    int getWidth() const
    {
        return width;
    }

    int getHeight() const
    {
        return height;
    }

private: //========= Fields
    /// The Window pointer
//    GLFWwindow * win = nullptr;

    /// Viewport Width and height
    int width=0, height=0;
};



}
