#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.hpp"

Shader::Shader() {
    _shaders[VERTEX_SHADER] = 0;
    _shaders[FRAGMENT_SHADER] = 0;
    _shaders[GEOMETRY_SHADER] = 0;
};

void Shader::loadFromFile(GLenum shaderType, const char* filePath) {
    std::ifstream file;
    std::string fileContents;

    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filePath);

        std::stringstream fileStream;
        fileStream << file.rdbuf();
        fileStream << "\0";
        file.close();

        fileContents = fileStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    
    loadFromString(shaderType, fileContents);
}

void Shader::loadFromString(GLenum shaderType, const std::string source) {
    unsigned int shader;
    const char *sourceStr = source.c_str();
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceStr, NULL);
    glCompileShader(shader);

    switch(shaderType) {
        case GL_VERTEX_SHADER:
            _shaders[VERTEX_SHADER] = shader;
        case GL_FRAGMENT_SHADER:
            _shaders[FRAGMENT_SHADER] = shader;
        case GL_GEOMETRY_SHADER:
            _shaders[GEOMETRY_SHADER] = shader;
    }
}

void Shader::createAndLinkProgram() {
    GLint isLinked;
    _programID = glCreateProgram();
    for (int i = 0; i < 3; i++) {
        if (_shaders[i] != 0) {
            glAttachShader(_programID, _shaders[i]);
        }
    }
    
    glLinkProgram(_programID);
    glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

        // The program is useless now. So delete it.
        glDeleteProgram(_programID);

        // Provide the infolog in whatever manner you deem best.
        // Exit with failure.
        std::cout << infoLog[0] << std::endl;
        return;
    }

}

void Shader::use() {
    glUseProgram(_programID);
}