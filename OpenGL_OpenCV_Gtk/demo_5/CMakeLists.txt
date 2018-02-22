cmake_minimum_required(VERSION 3.1)

project(demo_5)

set(CMAKE_CXX_STANDARD 14)
file(COPY ../shaders DESTINATION .)
file(COPY ../common_textures DESTINATION .)

add_executable(${PROJECT_NAME}
    main.cpp
    MainWindow.cpp
    MainWindow.h

    OpenglWindow.cpp
    OpenglWindow.h

    SyncBuf.h

    VideoThread.h
    VideoThread.cpp
    )
target_link_libraries(${PROJECT_NAME}
    Soil
    GtkGL
    ${GTKMM_LIBRARIES}
    ${GLEW_LIB}
    ${OPENGL_gl_LIBRARY}
    ${OpenCV_LIBS}
    )
