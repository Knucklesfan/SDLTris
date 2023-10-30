#pragma once

//Not truly a skybox, because that wouldn't really be needed for this project, but rather a sky image!
//Skyboxes could be implemented here, though....
#ifdef CLIENT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h> // otherwise we want to use OpenGL
#else
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "texture.h"

#ifdef CLIENT
class cube
#else
class cube
#endif
{
    public:
        cube(glm::vec3 prepos,glm::vec3 postpos, glm::vec3 scale, glm::vec3 rotation);
        cube(glm::vec3 prepos,glm::vec3 postpos, glm::vec3 scale, glm::vec3 rotation,float verts[], int size);
        void render(shader* shad, texture* t, glm::mat4 projection, glm::mat4 view);
        glm::vec3 position;
        glm::vec3 postposition;
        glm::vec3 scale;
        glm::vec3 rotation;
        unsigned int VBO, VAO, EBO;

};
