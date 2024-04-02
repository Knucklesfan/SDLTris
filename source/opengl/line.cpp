#include "line.h"
#include "../utils/defs.h"


lineRenderer::lineRenderer() {
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

float distance(int x1, int y1, int x2, int y2) {
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

void lineRenderer::render(shader* shad, glm::vec2 firstcoord, glm::vec2 secondcoord, int thickness,glm::vec4 color, glm::vec2 screensize) {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screensize.x), 
    static_cast<float>(screensize.y), 0.0f, -1.0f, 1.0f);

    shad->activate();
	shad->setVector("projection",glm::value_ptr(projection));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(firstcoord, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::rotate(model, ((atan2(secondcoord.y-firstcoord.y,secondcoord.x-firstcoord.x))), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate

    model = glm::scale(model, glm::vec3(distance(firstcoord.x,firstcoord.y,secondcoord.x,secondcoord.y),thickness, 1.0f)); // last scale
    
    shad->setVector("model", glm::value_ptr(model));
    shad->setVec4("col",glm::value_ptr(color));
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

  }