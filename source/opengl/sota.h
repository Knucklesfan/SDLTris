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
/*
SOTA IMPLEMENTATION MAP:
$0000-$0001: number of frames in animation uint16_t
$0002-$00XX: Each frame's pointer uint16_t
$00XX-$YYYY: Ater all pointers are done, the raw frame data is after

RAW FRAME (addresses start from frame start)
$00: Header value of $01 (this may not be true)
$01: The command number (will be outlined later when I understand commands better)
$02: The size of the command
$03-$XX: Each point on frame

FRAME POINT FORMAT (addresses start at point start)
$00: Y coordinate of point
$01: X coordinate of point
*/

#define SOTA_BUFFERSIZE 512 //this define sets the size of the buffer that gets used when rendering
//kinda important because im not sure how big the VBO is gonna have to be
#define FRAMEPERMS 50 //the amount of miliseconds that have to pass before rendering another frame.
class sotaRenderer {
    public:
        sotaRenderer(std::string path);
        void render();
        int indexnum = 0;

    private:
        unsigned int VAO,VBO;
        glm::mat4 projection;
        std::vector<uint16_t> indexes; //this contains the index of each frame of animation
        std::vector<uint8_t> data; //this contains the index of each frame of animation
};