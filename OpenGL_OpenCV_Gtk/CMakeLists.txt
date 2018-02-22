cmake_minimum_required(VERSION 3.1)
project(OpenGL_OpenCV_Gtk)

set(CMAKE_CXX_STANDARD 14)

# OpenGL, GLEW, GLFW :
FIND_LIBRARY(GLEW_LIB NAMES glew GLEW glew32)
find_package(OpenGL REQUIRED)
set(GL_LIBS  ${GLEW_LIB} ${OPENGL_gl_LIBRARY})

# Set include dirs
include_directories(
    Soil
    GtkGL
    )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
FIND_LIBRARY(GLEW_LIB NAMES glew GLEW glew32)

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS} )
# Threads
find_package(Threads)
find_package(Glib)

# Build SOIL library
add_subdirectory(Soil)

# Build GtkGL library
add_subdirectory(GtkGL)

add_subdirectory(demo_1)
add_subdirectory(demo_2)
add_subdirectory(demo_3)
add_subdirectory(demo_4)
add_subdirectory(demo_5)
add_subdirectory(demo_6)
add_subdirectory(demo_7)
add_subdirectory(demo_8)
