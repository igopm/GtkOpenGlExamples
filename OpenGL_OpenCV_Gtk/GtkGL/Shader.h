#pragma once
//
// Created by Igor Maschikevich on 10/03/2017.
//
//================================================================================
#include <string>
#include <memory>
#include "glheader.h"
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

    GLint loc(const std::string & uniformName) const{
        return glGetUniformLocation(prog, uniformName.c_str());
    }
//    void setMatMVP(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & proj){
//        glUniformMatrix4fv(uModel, 1, GL_FALSE, value_ptr(model));
//        glUniformMatrix4fv(uView, 1, GL_FALSE, value_ptr(view));
//        glUniformMatrix4fv(uProj, 1, GL_FALSE, value_ptr(proj));
//    }

    /// Set cam and model  matrices (uniforms uCam, uModel)
//    void setMatCM(const glm::mat4 & cam, const glm::mat4 & model){
//        glUniformMatrix4fv(uCam, 1, GL_FALSE, value_ptr(cam));
//        glUniformMatrix4fv(uModel, 1, GL_FALSE, value_ptr(model));
//    }


public: //==== Methods
    void use() const{
        glUseProgram(prog);
    }

private: //==== Methods
    static std::shared_ptr<std::string> parseFile(const std::string & fileName);
private:
//    GLint uModel, uCam, uView, uProj;
public: //==== Fields
    GLuint prog;
};
}

