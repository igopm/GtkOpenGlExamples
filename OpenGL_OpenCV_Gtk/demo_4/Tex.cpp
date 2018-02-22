#include <SOIL.h>
#include "Tex.h"

Tex::Tex() {
    // Create and bind texture objects
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Set wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind me
    glBindTexture(GL_TEXTURE_2D, 0);
}
//==============================================================

Tex Tex::fromFile(const std::string &fileName) {
    Tex t;

    // Bind
    glBindTexture(GL_TEXTURE_2D, t.tex);

    // Load image from a file with SOIL
    unsigned char *image = SOIL_load_image(fileName.c_str(), &t.w, &t.h, 0, SOIL_LOAD_RGB);

    if (image == NULL)
        std::cout << "Tex::oadTexture() : Cannot load image!" << std::endl;

    // Set up the image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.w, t.h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free memory and unbind
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return t;
}
//==============================================================

Tex Tex::fromMat(const cv::Mat &image) {
    Tex t;

    // Check if the image makes sense
    if (image.empty() || !image.isContinuous() || 3 != image.channels())
        std::cout << "Tex::fromMat::BAD OPENCV MATRIX!" << std::endl;

    t.w = image.cols;
    t.h = image.rows;

    // Bind
    glBindTexture(GL_TEXTURE_2D, t.tex);

    // Set up the image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.w, t.h, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //  unbind
    glBindTexture(GL_TEXTURE_2D, 0);

    return t;
}
//==============================================================

void Tex::updateImage(const cv::Mat &image) {
    if (image.empty() || !image.isContinuous() || 3 != image.channels())
        std::cout << "Tex::updateImag::BAD OPENCV MATRIX!" << std::endl;

    if (image.cols != w || image.rows != h)
        std::cout << "Tex::updateImage::WRONG SIZE!" << std::endl;

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, image.data);
    glBindTexture(GL_TEXTURE_2D, 0);

}

//==============================================================
