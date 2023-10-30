#include "cube.h"
#ifdef CLIENT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h> // otherwise we want to use OpenGL
#include <iostream>
#else
#include <GL/glew.h>
#include <GL/gl.h>

#endif

cube::cube(glm::vec3 prepos, glm::vec3 postpos, glm::vec3 scale, glm::vec3 rotation) {
        float vertices[216] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //final side
            0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  -1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  -1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //original side
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //demo side
            -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //unused by transition
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        };
    __glewGenVertexArrays(1, &VAO);
    __glewGenBuffers(1, &VBO);
    __glewGenBuffers(1, &EBO);
    __glewBindVertexArray(VAO);

    __glewBindBuffer(GL_ARRAY_BUFFER, VBO);
    __glewBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// position attribute
    __glewVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    __glewEnableVertexAttribArray(0);
	// texture coord attribute
    __glewVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    __glewEnableVertexAttribArray(2);
    // texture to use
    __glewVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));
    __glewEnableVertexAttribArray(3);

    this->rotation = rotation;
    this->position = prepos;
	this->postposition = postpos;
    this->scale = scale;

}
cube::cube(glm::vec3 prepos, glm::vec3 postpos, glm::vec3 scale, glm::vec3 rotation,float vertices[], int size) {

    __glewGenVertexArrays(1, &VAO);
    __glewGenBuffers(1, &VBO);
    __glewGenBuffers(1, &EBO);
    __glewBindVertexArray(VAO);

    __glewBindBuffer(GL_ARRAY_BUFFER, VBO);
    __glewBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, vertices, GL_STATIC_DRAW);

    std::cout << sizeof(vertices) << " <-SIZE OF VERTICES!\n";
	// position attribute
    __glewVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    __glewEnableVertexAttribArray(0);
	// texture coord attribute
    __glewVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    __glewEnableVertexAttribArray(2);
    // texture to use

    this->rotation = rotation;
    this->position = prepos;
	this->postposition = postpos;
    this->scale = scale;

}


//please define a texture before using this function. WARNING: DEPRECIATED (kinda)
void cube::render(shader* shad, texture* t, glm::mat4 projection, glm::mat4 view) {
		
    t->activate(0);

    shad->activate();
    shad->setInt("texture1",0);
    glm::mat4 transform = glm::mat4(0.5f); //the actual transform of the model itself
    transform = glm::translate(transform,position);
    transform = glm::rotate(transform, glm::radians(rotation[0]), glm::vec3(0.5f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation[1]), glm::vec3(0.0f, 0.5f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 0.5f));
    transform = glm::scale(transform, scale);
    transform = glm::translate(transform,postposition);

    shad->setVector("model", glm::value_ptr(transform));
    shad->setVector("projection", glm::value_ptr(projection));
    shad->setVector("view", glm::value_ptr(view));
    __glewBindVertexArray(VAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
