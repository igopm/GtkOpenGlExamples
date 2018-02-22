cmake_minimum_required(VERSION 3.1)

project(demo_6)

set(CMAKE_CXX_STANDARD 14)
file(COPY ../shaders DESTINATION .)
file(COPY ../common_textures DESTINATION .)

add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.cpp
    MainWindow.h
    VideoArea.h
    VideoArea.cpp
    )
target_link_libraries(${PROJECT_NAME}
    Soil
    ${GTKMM_LIBRARIES}
    ${GLEW_LIB}
    ${OPENGL_gl_LIBRARY}
    ${OpenCV_LIBS}
    #    ${GTKTHREAD_LIBRARIES}
    #    ${CMAKE_THREAD_LIBS_INIT}
    )
