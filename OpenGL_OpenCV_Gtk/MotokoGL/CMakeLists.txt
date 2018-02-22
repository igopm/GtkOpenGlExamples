cmake_minimum_required(VERSION 3.0)
project (MotokoGL)

set (CMAKE_CXX_STANDARD 14)
# OpenGL, GLEW, GLFW :
FIND_LIBRARY(GLEW_LIB NAMES glew GLEW glew32)
find_package(OpenGL REQUIRED)
set(GL_LIBS  ${GLEW_LIB} ${OPENGL_gl_LIBRARY})

# Source files
set (SOURCE_FILES
    ./glheader.h
    ./fatal.h
    #      ./Window.cpp ./Window.h
    ./ShaderProg.cpp ./ShaderProg.h
    ./IVao.h
    ./Vao.h ./Vao.cpp
    ./Tex.h ./Tex.cpp
    ./SimpleCamera.h
    #      ./Material.h
    #      ./MaterialT.h
    ./MotokoGL.h
    #      ./Light.h
    )

add_library(MotokoGL ${SOURCE_FILES})
