cmake_minimum_required(VERSION 3.1)

project(demo_3)

set(CMAKE_CXX_STANDARD 14)
file(COPY ../shaders DESTINATION .)
file(COPY ../common_textures DESTINATION .)

add_executable(${PROJECT_NAME}
    OpenglWindow.h
    OpenglWindow.cpp
    Shader.h
    main.cpp
    )
target_link_libraries(${PROJECT_NAME}
    Soil
    ${GTKMM_LIBRARIES}
    ${GLEW_LIB}
    ${OPENGL_gl_LIBRARY}
    )
