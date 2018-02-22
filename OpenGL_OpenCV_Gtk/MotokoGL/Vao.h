#pragma once
// By Oleksiy Grechnyev

#include "./glheader.h"

#include <vector>

#include "./IVao.h"

namespace MotokoGL {
/// The Vao C++ wrapper
class Vao : public IVao
{
public: // ====== Constructor =====
    /**
     * @brief Vao constructor
     *
     * Signature example : {3, 0, 2}. Means the following arrays, use this for XYZ_ST data:
     * 0: 3 elements
     * 1: none
     * 2: 2 elements
     *
     * Or {3,3,2} for the XYZ_RGB_ST data, {3,3} for XYZ_RGB, {3} for XYZ etc.
     *
     * @param[in] signature         The signature (see above)
     * @param[in] vertices          Vertex array
     * @param[in] verticesSize      sizeof(vertices)
     * @param[in] indices           Index array (nullptr if no EBO)
     * @param[in] indicesSize       sizeof(indices)
     */
    Vao(const std::vector<unsigned> & signature,
        const GLfloat * vertices, size_t verticesSize,
        const GLuint * indices = nullptr, size_t indicesSize = 0);

public: // ====== Methods
    /// Draw this VAO object
    virtual void draw() override;

    /// Destructor
    virtual ~Vao(){
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        if (useEbo)
            glDeleteBuffers(1, &ebo);
    }

    /// No copy costructor, no assignment, otherwise our vao, vbo, ebo get destroyed by the destructor
    Vao(const Vao &) = delete;
    Vao & operator=(const Vao &) = delete;

private: // Fields
    /// Are we using EBO ?
    bool useEbo = false;

    /// Number of elements to draw (vertices or indices)
    GLsizei count = 0;

    /// GL ids of vao, vbo and ebo
    GLuint vao=0, vbo=0, ebo=0;
};
}
