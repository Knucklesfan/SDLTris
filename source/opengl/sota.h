//A really cool, really quick renderer for State of the Art, that cool amiga demo
//Based on this, but reimplemented https://github.com/nfd/sota.git

#include "shader.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_opengl.h>
#include <vector>

#define SOTA_BUFFERSIZE 512 //this define sets the size of the buffer that gets used when rendering
//kinda important because im not sure how big the VBO is gonna have to be
#define FRAMEPERMS 50 //the amount of miliseconds that have to pass before rendering another frame.
class sotaRenderer {
    public:
        sotaRenderer(std::string path);
        void render(shader* shad, texture* tex);
    private:
        unsigned int VAO,VBO;
        glm::mat4 projection;
        std::vector<uint16_t> indexes; //this contains the index of each frame of animation
        std::vector<uint8_t> data; //this contains the index of each frame of animation
};