#include <numeric>
#include <iostream>
#include "./Vao.h"



namespace GtkGL {

//=================================================================
//Vao::Vao(const std::vector<unsigned> & signature,
Vao Vao::create(const std::vector<unsigned> & signature,
                const GLfloat * vertices, size_t verticesSize,
                const GLuint * indices, size_t indicesSize)
{
    using namespace std;
    Vao v1;
    v1.isEmpty = false;
    v1.useEbo = (indices != nullptr);  // Do we use EBO ?

    // Sum of all elements in signature
    unsigned sigSum = accumulate(signature.begin(), signature.end(), 0);

    if (sigSum ==0)
        cout << "Vao::Vao() : Wrong signature." << endl;

    if (verticesSize % sigSum !=0)
        cout << "Vao::Vao() : verticesSize % sigSum != 0." << endl;

    // Calculate count
    if (v1.useEbo)
        v1.count = indicesSize;
    else
        v1.count = verticesSize / (sigSum*sizeof(GLfloat));

    // ===== Create VAO, VBO, EBO

    // Create VBO, VAO

    glGenVertexArrays(1, &v1.vao);
    glGenBuffers(1, &v1.vbo);

    // Bind VAO
    glBindVertexArray(v1.vao);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, v1.vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    // Create and bind EBO if needed
    if (v1.useEbo){
        glGenBuffers(1, &v1.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, v1.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    }

    // Loop over all signature elements (aka GL array indices)
    unsigned pos = 0; // Current position in the data
    for (GLuint i = 0; i < signature.size(); ++i){
        unsigned n = signature[i]; // Number of elements

        if (n) {  // Skip if n==0
            glVertexAttribPointer(i, n, GL_FLOAT, GL_FALSE,
                                  sigSum*sizeof(GLfloat), (GLvoid *)(pos*sizeof(GLfloat)));
            glEnableVertexAttribArray(i); // Enable this index
        }

        pos += n; // Move pos
    }

    // Unbind VBO+VAO, but not EBO !!!
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return v1;
}

//=================================================================
void Vao::draw() {
    glBindVertexArray(vao);
    if (useEbo) {
        // Draw with ebo
//        glDepthMask(GL_FALSE);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    } else {
        // Draw w/o ebo
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    glBindVertexArray(0);
}
//=================================================================

}
