cmake_minimum_required(VERSION 3.1)

project(demo_7)

set(CMAKE_CXX_STANDARD 14)
file(COPY ../shaders DESTINATION .)
file(COPY ../common_textures DESTINATION .)

find_package(PkgConfig)

pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(GTKTHREAD gthread-2.0)

link_directories(
    ${GTKMM_LIBRARY_DIRS}
    ${GTKTHREAD_LIBRARY_DIRS})

include_directories(
    ${GTKMM_INCLUDE_DIRS}
    ${GTKTHREAD_INCLUDE_DIRS}
    )

add_executable(${PROJECT_NAME}
    main.cpp
   examplewindow.cpp
   examplewindow.h
   exampleworker.cpp
   exampleworker.h
    )
target_link_libraries(${PROJECT_NAME}
    ${GTKMM_LIBRARIES}
    ${GLEW_LIB}
    ${OPENGL_gl_LIBRARY}
    ${OpenCV_LIBS}
    #    ${GTKTHREAD_LIBRARIES}
    #    ${CMAKE_THREAD_LIBS_INIT}
    )
