cmake_minimum_required(VERSION 3.1)

project(demo_2)

set(CMAKE_CXX_STANDARD 14)
file(COPY ../shaders DESTINATION .)
file(COPY ../common_textures DESTINATION .)

add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.cpp
    MainWindow.h

    SimpleCamera.h
    Tex.h
    Tex.cpp

    OpenglWindow.cpp
    OpenglWindow.h

    #    glhead.h

    Shader.h
    Shader.cpp
    )
target_link_libraries(${PROJECT_NAME}
    Soil
    ${GTKMM_LIBRARIES}
    ${GLEW_LIB}
    ${OpenCV_LIBS}
    ${OPENGL_gl_LIBRARY}
    )
