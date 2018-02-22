#pragma once
//
// Created by Igor Maschikevich on 10/03/2017.
//
//================================================================================
#include <string>
#include <memory>
#define GLEW_STATIC
#include <GL/glew.h>
namespace GtkGL {
class Shader
{
public:
    /// Default
    Shader() {}

public: //==== Static factory methods
    /// Create shader program from memory strings
    static Shader fromMem(const GLchar *vertexSource, const GLchar *fragmentSource);

    /// Create shader program from files
    static Shader fromFiles(const std::string & vertexFileName, const std::string & fragmentFileName);

public: //==== Methods
    void use() const{
        glUseProgram(prog);
    }

private: //==== Methods
    static std::shared_ptr<std::string> parseFile(const std::string & fileName);
public: //==== Fields
    GLuint prog;
};
}
