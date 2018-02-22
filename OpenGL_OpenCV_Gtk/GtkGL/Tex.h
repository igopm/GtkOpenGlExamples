#pragma once
//
// Created by Igor Maschikevich on 10/26/2017.
//
//================================================================================
#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <opencv2/opencv.hpp>

namespace GtkGL {
/// Texture operations
class Tex
{
public:
    Tex();

public:  //======= Methods
    /// Load texture from file with SOIL
    static Tex fromFile(const std::string & fileName);

    /// Load texture from OpenCV image CV::Mat
    static Tex fromMat(const cv::Mat & image);

    /// Update the texture with OpenCV::Mat, size unchanged
    void updateImage(const cv::Mat & image);

    /// Bind as texture #index
    void bind(unsigned index) const{
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, tex);
    }

    /// Set the nearest filtering
    void setToNearest(){
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint getTex() const{
        return tex;
    }

public:
    GLuint tex;
    int w = 0, h = 0;
};
}
