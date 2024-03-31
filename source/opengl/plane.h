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

class plane {
    public:
        plane(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
        void render(shader* shad, texture* t, glm::mat4 projection, glm::mat4 view);
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
    private:
        glm::mat4 transform;
        unsigned int VBO, VAO, EBO;
        float vertices[32] = {
            // positions        //normal       // texture coords
            1.0f,  1.0f, 0.0f,  0.0f,1.0f,0.0f,    1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,  0.0f,1.0f,0.0f,    1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f,1.0f,0.0f,    0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f, 0.0f,1.0f,0.0f,    0.0f, 1.0f  // top left 
        };
        unsigned int indices[6] = {
            0, 1, 3, // first triangle
            1, 2, 3
        };


};