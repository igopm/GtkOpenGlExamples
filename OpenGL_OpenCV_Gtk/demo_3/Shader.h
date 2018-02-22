////
//// Created by Igor Maschikevich on 10/03/2017.
////
////================================================================================
//#ifndef SHADER_H
//#define SHADER_H

//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iostream>

//#include <GL/glew.h>

//class Shader
//{
//public:

//    Shader() {}
//    GLuint Program = 0;
//    // Constructor generates the shader on the fly
//    static Shader fromFiles(const GLchar* vertexPath, const GLchar* fragmentPath){

//        //    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
//        //    {
//        // 1. Retrieve the vertex/fragment source code from filePath
//        std::string vertexCode;
//        std::string fragmentCode;
//        std::ifstream vShaderFile;
//        std::ifstream fShaderFile;
//        // ensures ifstream objects can throw exceptions:
//        vShaderFile.exceptions (std::ifstream::badbit);
//        fShaderFile.exceptions (std::ifstream::badbit);
//        try
//        {
//            // Open files
//            vShaderFile.open(vertexPath);
//            fShaderFile.open(fragmentPath);
//            std::stringstream vShaderStream, fShaderStream;
//            // Read file's buffer contents into streams
//            vShaderStream << vShaderFile.rdbuf();
//            fShaderStream << fShaderFile.rdbuf();
//            // close file handlers
//            vShaderFile.close();
//            fShaderFile.close();
//            // Convert stream into string
//            vertexCode = vShaderStream.str();
//            fragmentCode = fShaderStream.str();
//        }
//        catch (std::ifstream::failure e)
//        {
//            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
//        }
//        const GLchar* vShaderCode = vertexCode.c_str();
//        const GLchar * fShaderCode = fragmentCode.c_str();
//        // 2. Compile shaders
//        GLuint vertex, fragment;
//        GLint success;
//        GLchar infoLog[512];
//        // Vertex Shader
//        vertex = glCreateShader(GL_VERTEX_SHADER);
//        glShaderSource(vertex, 1, &vShaderCode, NULL);
//        glCompileShader(vertex);
//        // Print compile errors if any
//        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
//        if (!success)
//        {
//            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//        }
//        // Fragment Shader
//        fragment = glCreateShader(GL_FRAGMENT_SHADER);
//        glShaderSource(fragment, 1, &fShaderCode, NULL);
//        glCompileShader(fragment);
//        // Print compile errors if any
//        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
//        if (!success)
//        {
//            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//        }
//        // Shader Program
//        Program = glCreateProgram();
//        glAttachShader(Program, vertex);
//        glAttachShader(Program, fragment);
//        glLinkProgram(Program);
//        // Print linking errors if any
//        glGetProgramiv(Program, GL_LINK_STATUS, &success);
//        if (!success)
//        {
//            glGetProgramInfoLog(Program, 512, NULL, infoLog);
//            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//        }
//        // Delete the shaders as they're linked into our program now and no longer necessery
//        glDeleteShader(vertex);
//        glDeleteShader(fragment);

//    }
//    std::shared_ptr<std::string> ShaderProg::parseFile(const std::string &fileName)
//    {
//        using namespace std;

//        ifstream in(fileName);



//        shared_ptr<string> s = make_shared<string>((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

//        in.close();

//        return s;
//    }

//    // Uses the current shader
//    void Use()
//    {
//        glUseProgram(Program);
//    }
//};

//#endif
