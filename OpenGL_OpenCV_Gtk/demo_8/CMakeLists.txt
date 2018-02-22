cmake_minimum_required(VERSION 3.1)

project(demo_8)

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

#    examplewindow.h
#    examplewindow.cpp

#    OtherThreads.h
#    OtherThreads.cpp

#    MarkerDataBlock.h
#    VDecoder.h
#    VDecoder.cpp
#    fatal.h
    )
target_link_libraries(${PROJECT_NAME}
    GtkGL
    Soil
    ${GTKMM_LIBRARIES}
    ${GLEW_LIB}
    ${OPENGL_gl_LIBRARY}
    ${OpenCV_LIBS}
    )
