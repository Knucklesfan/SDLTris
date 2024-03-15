#include "rect.h"
#include "../utils/defs.h"


rectRenderer::rectRenderer() {
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

void rectRenderer::render(shader* shad, glm::vec2 firstcoord, glm::vec2 secondcoord,
float rotate,glm::vec4 color,bool outline, float thickness, glm::vec4 borderColor) {
    // prepare transformations
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(COORDINATE_WIDTH), 
    static_cast<float>(COORDINATE_HEIGHT), 0.0f, -1.0f, 1.0f);

    shad->activate();
	shad->setVector("projection",glm::value_ptr(projection));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(firstcoord, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * (secondcoord.x-firstcoord.x), 0.5f * (secondcoord.y-firstcoord.y), 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * (secondcoord.x-firstcoord.x), -0.5f * (secondcoord.y-firstcoord.y), 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(secondcoord.x-firstcoord.x,secondcoord.y-firstcoord.y, 1.0f)); // last scale
    
    shad->setVector("model", glm::value_ptr(model));
    shad->setVec4("col",glm::value_ptr(color));
    shad->setVec4("bordercol",glm::value_ptr(borderColor));
    shad->setFloat("border_width",(thickness));
    float aspect = (secondcoord.y-firstcoord.y)/(secondcoord.x-firstcoord.x);
    shad->setFloat("aspect",aspect);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    if(outline) {
        graphics::line->render(graphics::shaders.at(1),{firstcoord.x,secondcoord.y},{firstcoord.x,firstcoord.y},thickness,borderColor);
        graphics::line->render(graphics::shaders.at(1),{secondcoord.x,firstcoord.y},{firstcoord.x,firstcoord.y},thickness,borderColor);
        graphics::line->render(graphics::shaders.at(1),{secondcoord.x,firstcoord.y},{secondcoord.x,secondcoord.y},thickness,borderColor);
        graphics::line->render(graphics::shaders.at(1),{firstcoord.x,secondcoord.y},{secondcoord.x,secondcoord.y},thickness,borderColor);

    }
  }