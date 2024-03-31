#include "plane.h"

plane::plane(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation) {
    
    //generates the VBO and stuff
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
    this->rotation = rotation;
    this->position = pos;
    this->scale = scale;
}

void plane::render(shader* shad, texture* t, glm::mat4 projection, glm::mat4 view) {

    transform = glm::mat4(1.0f); //the actual transform of the model itself
	transform = glm::translate(transform,position);
    
    transform = glm::rotate(transform, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));  
	transform = glm::rotate(transform, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));  
	transform = glm::rotate(transform, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));  

	transform = glm::scale(transform, scale);

    t->activate(0);
	shad->activate();
	shad->setInt("texture1",0);
	shad->setInt("reflectionmap",1);
	shad->setInt("reflectionmask",2);

	glm::vec3 camerapos = {sin(SDL_GetTicks()/1000.0f),cos(SDL_GetTicks()/1000.0f),cos(SDL_GetTicks()/1000.0f)};
    shad->setVector("model", glm::value_ptr(transform));
    shad->setVector("projection", glm::value_ptr(projection));
	shad->setVector("view", glm::value_ptr(view));
	shad->setVec3("cameraPos", glm::value_ptr(camerapos));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}