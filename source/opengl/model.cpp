#include "model.h"
std::vector<std::string> split(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    
    for (char c : input) {
        if (c == delimiter) {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        result.push_back(token);
    }
    
    return result;
}

mesh::mesh(std::vector<vertex> vertices, std::vector<int> indices, std::vector<texture*> textures, Material mat) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, texcoord));

    glBindVertexArray(0);
    std::cout <<"SIZE MATTERS: " << vertices.size() <<"\n";

}
void mesh::render(shader* shad) {
//    unsigned int diffuseNr = 1;
//    unsigned int specularNr = 1;
//    for(unsigned int i = 0; i < textures.size(); i++) {
//        textures[i]->activate(i);
//        // retrieve texture number (the N in diffuse_textureN)
//        std::string number;
//        std::string name = textures[i]->type;
//        if(name == "texture_diffuse")
//            number = std::to_string(diffuseNr++);
//        else if(name == "texture_specular")
//            number = std::to_string(specularNr++);

//        shad->setInt(("material." + name + number), i);
//    }
//    glActiveTexture(GL_TEXTURE0);

//    // draw mesh
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int emmisiveNr = 1; //i have no idea if i spelled this right or not, but its 1:45am and im tired
    //(please dont use timestamps to verify this information i promise its late please believe me im not a coward)

    for(unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i]->type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_emission")
            number = std::to_string(emmisiveNr++);

        shad->setInt(("material." + name).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->id);
    }

    shad->setFloat("material.shininess", 1.0f);

    shad->setVec3("material.diffuseColor", glm::value_ptr(mat.diffuseColor));
    shad->setVec3("material.emissionColor", glm::value_ptr(mat.emissionColor));

    glActiveTexture(GL_TEXTURE0);
    //std::cout << indices.size() << "\n";
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}
model::model(std::string path,glm::vec3 prepos, glm::vec3 scale, glm::vec3 rotation) {
    this->position = prepos;
    this->scale = scale;
    this->rotation = rotation;
    std::streampos size;
    char * memblock;
    std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();

        std::cout << "the entire file content is in memory\n";
        size_t offset = 0;
        size_t meshNum = 0;
        memcpy(&meshNum, memblock+offset, sizeof(size_t)); //very memory unsafe, please do not supply bad savestates...
        offset += sizeof(size_t);
        for(int i = 0; i < meshNum; i++) {
            size_t size = 0;
            memcpy(&size, memblock+offset, sizeof(size_t)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(size_t);
            std::vector<vertex> vertices;
            std::vector<int> indices;
            std::vector<texture*> textures;
            Material mat;
            for(int x = 0; x < size; x++) {
                    glm::vec3 pos = {0,0,0};
                    glm::vec3 norm = {0,0,0};
                    glm::vec2 txcord = {0,0};
                    memcpy(&pos.x, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    memcpy(&pos.y, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    memcpy(&pos.z, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    memcpy(&norm.x, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    memcpy(&norm.y, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    memcpy(&norm.z, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    memcpy(&txcord.x, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    memcpy(&txcord.y, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(float);
                    vertices.push_back({pos,norm,txcord});
            }
            size_t indicesize = 0;
            memcpy(&indicesize, memblock+offset, sizeof(size_t)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(size_t);
            for(int x = 0; x < indicesize; x++) {
                int indice = 0;
                memcpy(&indice, memblock+offset, sizeof(int)); //very memory unsafe, please do not supply bad savestates...
                offset += sizeof(int);
                indices.push_back(indice);
            }
            memcpy(&mat.shininess, memblock+offset, sizeof(float)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(float);
            memcpy(&mat.emissionColor, memblock+offset, sizeof(glm::vec3)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(glm::vec3);
            memcpy(&mat.diffuseColor, memblock+offset, sizeof(glm::vec3)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(glm::vec3);
            size_t texsize = 0;
            memcpy(&texsize, memblock+offset, sizeof(size_t)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(size_t);
            for(int x = 0; x < texsize; x++) {
                unsigned long sizeofPath = 0;
                unsigned long sizeofType = 0;
                std::string path = "";
                std::string type = "";
                memcpy(&sizeofPath, memblock+offset, sizeof(unsigned long)); //very memory unsafe, please do not supply bad savestates...
                offset += sizeof(unsigned long);
                for(int i = 0; i < sizeofPath; i++) {
                    char c = 0;
                    memcpy(&c, memblock+offset, sizeof(char)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(char);
                    path+=c;
                } 
                memcpy(&sizeofType, memblock+offset, sizeof(unsigned long)); //very memory unsafe, please do not supply bad savestates...
                offset += sizeof(unsigned long);

                for(int i = 0; i < sizeofType; i++) {
                    char c = 0;
                    memcpy(&c, memblock+offset, sizeof(char)); //very memory unsafe, please do not supply bad savestates...
                    offset += sizeof(char);
                    type+=c;
                } 
                std::vector<std::string> pathDelim = split(path,'/');
                std::string texturepath = "./models/textures/" + pathDelim.at(pathDelim.size()-1);
                texture* t = new texture(texturepath);
                t->type = type;
                textures.push_back(t);
            }
            meshes.push_back(mesh(vertices,indices,textures,mat));

        }
        delete memblock;
    }

}
void model::render(shader* shad,  glm::mat4 projection, glm::mat4 view) {
    shad->activate();
    shad->setVector("projection", glm::value_ptr(projection));
    shad->setVector("view", glm::value_ptr(view));

    glm::mat4 transform = glm::mat4(1.0f); //the actual transform of the model itself
    transform = glm::translate(transform,position);
    transform = glm::rotate(transform, glm::radians(rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, scale);

    glm::mat3 normal = glm::mat3(1.0f);
    normal = glm::transpose(glm::inverse(transform)); //calculate normals

    shad->setVector("model", glm::value_ptr(transform));
    shad->setMat3("worldspace", glm::value_ptr(normal));

    for(int i = 0; i < meshes.size(); i++) {
        meshes[i].render(shad);
    }
}