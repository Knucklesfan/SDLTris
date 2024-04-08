#pragma once
#include "../utils/defs.h"
//i wrote a lot of this code a long time ago
//so im just gonna copy the header from that...
//SO WITHOUT FURTHER ADO, I PRESENT TO YOU:::::
//MODEL.H! IN ALL ITS GLORY!

struct vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texcoord;
};
struct Material {
    float shininess;
    glm::vec3 diffuseColor;
    glm::vec3 emissionColor;
};
class mesh {
    public:
        // mesh data
        std::vector<vertex> vertices;
        std::vector<int> indices;
        std::vector<texture*> textures;
        Material mat;
        mesh(std::vector<vertex> vertices, std::vector<int> indices, std::vector<texture*> textures, Material mat);
        void render(shader* shad);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;
};

class model  {
    public:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
        model(std::string path,glm::vec3 prepos, glm::vec3 scale, glm::vec3 rotation);
        void render(shader* shad, glm::mat4 projection, glm::mat4 view);
    private:
        // model data
        std::vector<mesh> meshes;
};
