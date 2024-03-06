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

class rectRenderer {
    public:
        rectRenderer();
        void render(shader* shad, glm::vec2 firstcoord, glm::vec2 secondcoord,
float rotate,glm::vec4,bool outline, float thickness, glm::vec4 borderColo);
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