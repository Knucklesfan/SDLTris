#include "sprite.h"
#include "../utils/defs.h"


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
    render(shad,t,position,size,{0,0,rotate},texcoord,texsize);
}
void spriteRenderer::render(shader* shad, texture* t, glm::vec2 position, glm::vec2 size,
glm::vec3 rotation, glm::vec2 texcoord, glm::vec2 texsize, glm::vec2 screen, float z) {
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
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screen.x), 
    static_cast<float>(screen.y), 0.0f, -100.0f, 100.0f);
	glm::vec2 texOffset = glm::vec2(texcoord[0]/t->w,texcoord[1]/t->h);
	glm::vec2 texScale = glm::vec2(texsize[0]/t->w,texsize[1]/t->h);
	t->activate(0);

    shad->activate();
	shad->setVector("projection",glm::value_ptr(projection));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotation[0]), glm::vec3(0.5f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation[1]), glm::vec3(0.0f, 0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 0.5f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, z)); // go ahead and move in depth as well

    model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

    shad->setVector("model", glm::value_ptr(model));
	shad->setVec2("texOffset",glm::value_ptr(texOffset));
	shad->setVec2("scale",glm::value_ptr(texScale));
    shad->setInt("image",0);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

  }