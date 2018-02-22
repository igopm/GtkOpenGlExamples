cmake_minimum_required(VERSION 3.0)
project (GtkGL)

set (CMAKE_CXX_STANDARD 14)

# Source files
set (SOURCE_FILES
    #      ./glheader.h
    #      ./fatal.h
    ./Tex.h ./Tex.cpp
    ./Shader.h ./Shader.cpp
    ./Vao.h ./Vao.cpp
    ./IVao.h
    ./SimpleCamera.h
    ./GtkGL.h
    ./glheader.h
    )

add_library(${PROJECT_NAME} ${SOURCE_FILES})
