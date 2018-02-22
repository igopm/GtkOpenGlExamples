#pragma once
// By Oleksiy Grechnyev

#include "./glheader.h"

namespace MotokoGL {

/// Texture operations
class Tex
{
    //private: //====== Constructor
    /// Private constructor
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
    //    void setToNearest(){
    //        glBindTexture(GL_TEXTURE_2D, tex);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //        glBindTexture(GL_TEXTURE_2D, 0);
    //    }

    //    GLuint getTex() const{
    //        return tex;
    //    }

private: //======= Data
    /// The texture
    GLuint tex;

    /// Width and height
    int w = 0, h = 0;
};

}
