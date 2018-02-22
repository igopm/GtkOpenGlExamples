#include <numeric>

#include "./fatal.h"

#include "./Vao.h"



namespace MotokoGL {

//=================================================================
Vao::Vao(const std::vector<unsigned> & signature,
         const GLfloat * vertices, size_t verticesSize,
         const GLuint * indices, size_t indicesSize)
{
    using namespace std;

    useEbo = (indices != nullptr);  // Do we use EBO ?

    // Sum of all elements in signature
    unsigned sigSum = accumulate(signature.begin(), signature.end(), 0);

    if (sigSum ==0)
        fatal("Vao::Vao() : Wrong signature.");

    if (verticesSize % sigSum !=0)
        fatal("Vao::Vao() : verticesSize % sigSum != 0.");

    // Calculate count
    if (useEbo)
        count = indicesSize;
    else
        count = verticesSize / sigSum;

    // ===== Create VAO, VBO, EBO

    // Create VBO, VAO

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind VAO
    glBindVertexArray(vao);

    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    // Create and bind EBO if needed
    if (useEbo){
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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
}

//=================================================================
void Vao::draw() {
    glBindVertexArray(vao);

    if (useEbo) {
        // Draw with ebo
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    } else {
        // Draw w/o ebo
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    glBindVertexArray(0);
}
//=================================================================

}
