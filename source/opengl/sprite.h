#pragma once
//Not truly a skybox, because that wouldn't really be needed for this project, but rather a sky image!
//Skyboxes could be implemented here, though....
#include "shader.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_opengl.h>

class spriteRenderer {
    public:
        spriteRenderer();
        void render(shader* shad, texture* t, glm::vec2 position, glm::vec2 size,
float rotate, glm::vec2 texcoord, glm::vec2 texsize);
        void render(shader* shad, texture* t, glm::vec2 position, glm::vec2 size,
glm::vec3 rotate, glm::vec2 texcoord, glm::vec2 texsize, glm::vec2 screen={640,480});

    private:
        unsigned int quadVAO;
        float vertices[24] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };


};