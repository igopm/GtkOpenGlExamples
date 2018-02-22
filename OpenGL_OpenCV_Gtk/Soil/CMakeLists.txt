cmake_minimum_required(VERSION 3.0)
project (Soil)

set (CMAKE_CXX_STANDARD 14)

# Source files
set (SOURCE_FILES
        ./SOIL.c
        ./image_DXT.c
        ./image_helper.c
        ./stb_image_aug.c
      )

add_library(Soil ${SOURCE_FILES})
