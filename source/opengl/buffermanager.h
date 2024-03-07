#pragma once
#include "shader.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_opengl.h>

class buffermanager {
    public:
    buffermanager(int width, int height, bool alpha = false);
    void enable();
    void disable(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool restore  = false);
    void render(shader* shad,int WINDOW_WIDTH, int WINDOW_HEIGHT, bool aspect);
    const int width;
    const int height;
    texture renderTexture;
    private:
    GLuint framebuffer;
    GLint previousFB;

    unsigned int VBO, VAO, EBO;
    float vertices[20] = {
        // positions          // texture coords
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
        1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3
    };

};