#include <iostream>
#include <fstream>
#include <streambuf>

#include "./fatal.h"

#include "./ShaderProg.h"


namespace MotokoGL{
//==============================================================
ShaderProg ShaderProg::fromMem(const GLchar *vertexSource, const GLchar *fragmentSource)
{
    ShaderProg newProg;
    GLuint & prog = newProg.prog;

    // Create shaders and Compile
    // Vertex
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        fatal2("Cannot compile vertex shader", infoLog);
    }

    // Fragment
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        fatal2("Cannot compile fragment shader", infoLog);
    }

    // Shader program
    prog = glCreateProgram();
    glAttachShader(prog, vertexShader);
    glAttachShader(prog, fragmentShader);
    glLinkProgram(prog);

    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(prog, 512, nullptr, infoLog);
        fatal2("Cannot link shader program", infoLog);
    }

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set uniforms
    newProg.uCam = newProg.loc("uCam");
    newProg.uModel = newProg.loc("uModel");
    newProg.uView = newProg.loc("uView");
    newProg.uProj = newProg.loc("uProj");

    return newProg;
}
//==============================================================
ShaderProg ShaderProg::fromFiles(const std::string & vertexFileName, const std::string & fragmentFileName)
{
    using namespace std;

    auto s1 = parseFile(vertexFileName);
    auto s2 = parseFile(fragmentFileName);

    /*cout << "----------------" << endl;
    cout << s1->c_str();
    cout << "----------------" << endl;
    cout << s2->c_str();
    cout << "----------------" << endl;*/

    ShaderProg newProg = ShaderProg::fromMem(s1->c_str(), s2->c_str());
    return newProg;
}
//==============================================================
std::shared_ptr<std::string> ShaderProg::parseFile(const std::string &fileName)
{
    using namespace std;

    ifstream in(fileName);

    if (!in)
        fatal2("ShaderProg::parseFile() : cannot open file ", fileName.c_str());

    shared_ptr<string> s = make_shared<string>((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    in.close();

    return s;
}
//==============================================================


}
