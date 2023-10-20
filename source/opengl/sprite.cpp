#include "sprite.h"
#include "../utils/defs.h"
#include <glm/gtx/string_cast.hpp>

spriteRenderer::spriteRenderer() {
    //generates the VBO and stuff
    unsigned int VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void spriteRenderer::render(shader* shad, texture* t, glm::vec2 position, glm::vec2 size,
float rotate, glm::vec2 texcoord, glm::vec2 texsize) {
	/*
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f,0.0f, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

	t.activate(0);
	shad.activate();
	shad.setInt("texture1",0);

    shad.setVector("model", glm::value_ptr(model));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
*/
    // prepare transformations
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(COORDINATE_WIDTH), 
    static_cast<float>(COORDINATE_HEIGHT), 0.0f, -1.0f, 1.0f);
	glm::vec2 texOffset = glm::vec2(texcoord[0]/t->w,texcoord[1]/t->h);
	glm::vec2 texScale = glm::vec2(texsize[0]/t->w,texsize[1]/t->h);

    shad->activate();
	shad->setVector("projection",glm::value_ptr(projection));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    shad->setVector("model", glm::value_ptr(model));
	shad->setVec2("texOffset",glm::value_ptr(texOffset));
	shad->setVec2("scale",glm::value_ptr(texScale));
	t->activate(0);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

  }