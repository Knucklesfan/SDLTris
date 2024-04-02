#pragma once
//a badass, real-ass skybox baby
#include "shader.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_opengl.h>

class skybox {
    public:
        skybox();
        void render(int);
    private:
        unsigned int VBO, VAO, EBO;
};