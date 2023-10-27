#include <glad/gl.h>
#include <string>

class Shader {
    public:
        Shader();
        void loadFromString(GLenum shaderType, const std::string source);
        void loadFromFile(GLenum shaderType, const char* filePath);
        void createAndLinkProgram();
        void use();
    
    private:
        enum ShaderType{VERTEX_SHADER,FRAGMENT_SHADER,GEOMETRY_SHADER};
        GLuint _programID;
        GLuint _shaders[3];
};