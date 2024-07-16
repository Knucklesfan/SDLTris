#include "sota.h"
#include "../utils/defs.h"
#include "triangle/tpp_interface.hpp"

sotaRenderer::sotaRenderer(std::string path) {
    //a lot of this is gonna be just ttf renderer done again lol
    //this time with a 255 vertice buffer though!
    projection = glm::ortho(0.0f, static_cast<float>(640), 0.0f, static_cast<float>(480)); //THIS WAS THE STUPID BUG ALL ALONG???

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, NULL, GL_DYNAMIC_DRAW);
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
            memcpy(&index,filecache+offset,sizeof(uint16_t)); //load each of these bad boys from the array
            indexes.push_back(index);
            std::cout << index << "\n";
            offset += sizeof(uint16_t); //offset by the size
        }

        //now we get to have some fun
        int bottomsize = size-offset; //i have no idea if this works
        //update!! it works!
        for(int i = 0; i < bottomsize; i++) {
            uint8_t copied_data = 0; 
            memcpy(&copied_data,filecache+offset,sizeof(uint8_t)); //load each of these bad boys from the array
            offset++; //since we're only operating with bytes, we can just add 1 :)
            data.push_back(copied_data);
        }


        


    }
    //aaaaand thats basically all we have to do for now!

}
void sotaRenderer::render() {
    printf("%d\n",data.at(indexes.at(indexnum%indexes.size())+2));
    std::vector<tpp::Delaunay::Point> delaunayInput; //this may be slow as hell later on
    std::vector<tpp::Delaunay::Point> segments; //this may be slow as hell later on
    int stff = 0;
    for(int i = 1; i < data.at(indexes.at(indexnum%indexes.size())+2);i++) { //size value is at $02
        double x = (data.at(indexes.at(indexnum%indexes.size())+2+i*2))*2;
        double y = (data.at(indexes.at(indexnum%indexes.size())+2+i*2+1))*2;
        printf("%d      %f      %f\n",stff+1,x,y);
        delaunayInput.push_back(tpp::Delaunay::Point(x,y));
        graphics::sprite->render(
        graphics::shaders.at(4),
        graphics::blocks->at(1),
        {
            x,
            y
            },
            {2,2},0,{0,0},{8,8});
        
        stff++;

    }
    printf("%d 0\n",data.at(indexes.at(indexnum%indexes.size())+2)-1);

    
















    if(delaunayInput.size() > 2) {
        int iterLoop;
        for(iterLoop = 0; iterLoop < data.at(indexes.at(indexnum%indexes.size())+2)-2; iterLoop++) { //this second loop may be unnecessary
            segments.push_back(delaunayInput.at(iterLoop));
            segments.push_back(delaunayInput.at(iterLoop+1));

            printf("%d  %d  %d\n",iterLoop,iterLoop,iterLoop+1);
        }
        segments.push_back(delaunayInput.at(iterLoop));
        segments.push_back(delaunayInput.at(0));

        if(segments.size() > 0) {
            segments.push_back(segments.at(0));
        }

        tpp::Delaunay trGenerator(delaunayInput);
        trGenerator.setSegmentConstraint(segments);
        trGenerator.useConvexHullWithSegments(false);

        trGenerator.Triangulate(true, tpp::DebugOutputLevel::Vertex);

        graphics::shaders.at(11)->activate();
        glm::vec4 color = (glm::vec4(0,1,1,1));
        graphics::shaders.at(11)->setVec4("col",glm::value_ptr(color));
        glUniformMatrix4fv(glGetUniformLocation(graphics::shaders.at(11)->id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(VAO);

        for (tpp::FaceIterator fit = trGenerator.fbegin(); fit != trGenerator.fend(); ++fit)
        {
            tpp::Delaunay::Point sp1, sp2, sp3;

            int vertexIdx1 = fit.Org(&sp1);
            int vertexIdx2 = fit.Dest(&sp2);
            int vertexIdx3 = fit.Apex(&sp3);

            // access data
            double x1 = sp1[0]; // an added vertex, it's data copied to sp1
            double y1 = sp1[1];
            double x2 = sp2[0]; // an added vertex, it's data copied to sp1
            double y2 = sp2[1];
            double x3 = sp3[0]; // an added vertex, it's data copied to sp1
            double y3 = sp3[1];

            float vertices[3][4] = {
                { x1,     480-y1,   0.0f, 0.0f },            
                { x2,     480-y2,       0.0f, 1.0f },
                { x3,     480-y3,       1.0f, 1.0f },
            };
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 3);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)

        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        for(int i = 1; i < data.at(indexes.at(indexnum%indexes.size())+2);i++) { //size value is at $02
            double x = (data.at(indexes.at(indexnum%indexes.size())+2+i*2))*2;
            double y = (data.at(indexes.at(indexnum%indexes.size())+2+i*2+1))*2;
            graphics::sprite->render(
            graphics::shaders.at(4),
            graphics::blocks->at(1),
            {
                x,
                y
                },
                {2,2},0,{0,0},{8,8});
            
        }


    }

}