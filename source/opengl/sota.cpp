#include "sota.h"
#include "../utils/defs.h"

sotaRenderer::sotaRenderer(std::string path) {
    //a lot of this is gonna be just ttf renderer done again lol
    //this time with a 255 vertice buffer though!
    projection = glm::ortho(0.0f, static_cast<float>(640), 0.0f, static_cast<float>(480)); //THIS WAS THE STUPID BUG ALL ALONG???

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SOTA_BUFFERSIZE * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::streampos size;
    std::cout << "loading ksta" << path <<"\n";
    std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
    if(file.is_open()) {
        size = file.tellg();
        char* filecache = new char[size];
        file.seekg (0, std::ios::beg);
        file.read (filecache, size);
        file.close();
        std::cout << "entire file in memory\n";
        uint16_t indexSize = 0; //first, we load the number of indexes
        memcpy(&indexSize,filecache,sizeof(uint16_t));
        int offset = sizeof(uint16_t); //since we already offset it by one 16bit value anyways, 
                                        //we can just initialize it with this value
        for(int i = 0; i < indexSize; i++) { //now we can iterate
            uint16_t index = 0; 
            memcpy(&index,filecache,sizeof(uint16_t)); //load each of these bad boys from the array
            indexes.push_back(index);
            offset += sizeof(uint16_t); //offset by the size
        }

        //now we get to have some fun
        int bottomsize = size-offset; //i have no idea if this works
        std::cout << bottomsize << "\n";
        


    }
    //aaaaand thats basically all we have to do for now!

}
void sotaRenderer::render(shader* shad, texture* tex) {

}