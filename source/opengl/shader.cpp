#include "shader.h"
#include "../utils/defs.h"
#ifdef CLIENT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h> // otherwise we want to use OpenGL
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include <stdio.h>
#include <string>
shader::shader(std::string vertPath, std::string fragPath) //TODO: fix weird spelling thing (im lazy so this is a TODO)
{
    success = true;
    int suc; //debug stuff
    char infoLog[512];

    //compile shaders
    std::string vertStr = utils::loadFile(vertPath);
    const char* vertex = vertStr.c_str();
    //std::cout << vertStr << "\n";
    id = __glewCreateProgram();

    GLuint vertexShader = __glewCreateShader(GL_VERTEX_SHADER);
    __glewShaderSource(vertexShader, 1, &vertex, NULL);
    __glewCompileShader(vertexShader);

    __glewGetShaderiv(vertexShader, GL_COMPILE_STATUS, &suc);
    if (!suc)
    {
        __glewGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        error += "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog) + "\n";
        success = false;

    }
    //fragment
    std::string fragStr = utils::loadFile(fragPath);
    const char* fragment = fragStr.c_str();
    //std::cout << fragment << "\n";
    GLuint fragmentShader = __glewCreateShader(GL_FRAGMENT_SHADER);
    __glewShaderSource(fragmentShader, 1, &fragment, NULL);
    __glewCompileShader(fragmentShader);

    __glewGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &suc);
    if (!suc)
    {
        __glewGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        error += "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog) + "\n";
        success = false;
        

    }

    //make shader program
    __glewAttachShader(id, vertexShader);
    __glewAttachShader(id, fragmentShader);
    __glewLinkProgram(id); // you can absolutely set this up to auto-configure from an xml file
    __glewGetProgramiv(id, GL_LINK_STATUS, &suc);
    if (!suc) {
        __glewGetProgramInfoLog(id, 512, NULL, infoLog);
        error += "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog) + "\n";
        success = false;
    }
    std::cout << error << "\n";
    __glewDeleteShader(vertexShader);
    __glewDeleteShader(fragmentShader);
    vert = vertPath;
    frag = fragPath;
    if(!success) {
        std::cerr << "Error in shader compilation has occurred, crashing...";
        exit(-1);
    }
}

void shader::activate()
{
    __glewUseProgram(id);
}

void shader::setBool(const std::string& name, bool value) const
{
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniform1i(location, (int)value);

}

void shader::setInt(const std::string& name, int value) const
{
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniform1i(location, value);

}

void shader::setFloat(const std::string& name, float value) const
{
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniform1f(location, value);

}

void shader::setVector(const std::string& name, float* value) const {
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniformMatrix4fv(location,1, GL_FALSE, value);

}
void shader::setVec2(const std::string& name, float* value) const {
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniform2fv(location,1, value);

}
void shader::setVec3(const std::string& name, float* value) const {
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniform3fv(location,1, value);

}

void shader::setVec4(const std::string& name, float* value) const {
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniform4fv(location,1, value);

}
void shader::setMat3(const std::string& name, float* value) const {
    int location = __glewGetUniformLocation(id, name.c_str());
    __glewUniformMatrix3fv(location,1,GL_FALSE, value);

}
