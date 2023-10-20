#pragma once
#ifdef __LEGACY_RENDER
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <SDL2/SDL_mixer.h>

class bg
{
    
	public:
        double incrementsx[10];
        double incrementsy[10];
        double layerposx[10];
        double layerposy[10];
        int layers = 0;
        int sinelayer = 0;
        int snwidth = 0;
        int snheight = 0;
        int snwid = 0;
        int rate = 0;
        bool sine = 0;
        int fglayer = 0;
        double angle = 0.0;
        double rotation;
	    void logic(double deltatime);
        std::string name;
        std::string path;
        std::string creator;
        std::string vers;
        Mix_Music* music;
        std::string songname;
        std::string artist;
        void render(SDL_Renderer* renderer, bool layer);
        std::vector<SDL_Texture*> textures;
        SDL_Texture* thumbnail;
        bg(std::string path, bool folder, SDL_Renderer* renderer);
        bg();

    private:
        int maxwidth, maxheight;
    	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
        void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);
        bool hasEnding(std::string const& fullString, std::string const& ending);
        void generateSurfaces(std::string path, SDL_Renderer* renderer);
        static bool compareFunction (std::string a, std::string b) {return a<b;} 
        void drawLayer(SDL_Renderer* renderer, SDL_Texture* texture, int tempx, int tempy, int multiplerx, int multiplery, int width, int height, bool wavy, int wavywidth, int sinewidth, int sineheight, double sinepos);
        
};
#else
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <map>
#ifdef CLIENT
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h> // otherwise we want to use OpenGL
#else
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "opengl/texture.h"
#include "opengl/shader.h"
class layer { 
    public:
    //this is the way the system should've always been done, but of course that's not how it was done.
        virtual void render(){};
        virtual void logic(double deltatime){};
        int depth;
};
class staticlayer : public layer { //a flat, static image. Rendered very quickly because it doesn't need much.
    public:
        staticlayer(std::string path);
        void render();
        texture t;
    private:
        unsigned int VBO, VAO, EBO;
        float vertices[20] = {
            // positions             // texture coords
            1.0f,  1.0f, 0.0f,      1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,      1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,      0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,      0.0f, 1.0f  // top left 
        };
        unsigned int indices[6] = {
            0, 1, 3, // first triangle
            1, 2, 3
        };
};
class legacylayer : public layer { //Basically, functions identical to the old system, and takes a vec2 of movement.
    public:
        legacylayer(std::string, glm::vec2, glm::vec4);
        void render();
        void logic(double deltatime);
        texture t;
        shader* shad;
        glm::vec2 velocity;
        glm::vec2 position;
        glm::vec4 sine;
        double angle = 0;

    private:
        unsigned int quadVAO;
        float vertices[24] = { 
            // pos      // tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
        };

};
class flatlayer : layer { //a 2D layer, that can be manipulated in many ways, including paralax.

};
class depthlayer : layer { //an advanced 3D layer, supporting OBJ loading for models, positioning and whatnot. Blender addon included to generate this type of object, from blender animations.

};
class bg     
{
    
	public:
	    void logic(double deltatime);
        std::string name;
        std::string path;
        std::string creator;
        std::string vers;
#ifdef CLIENT
        Mix_Music* music;
#endif
        std::string songname;
        std::string artist;
        void render(shader* shad);
        std::vector<layer*> layers;
        bg(std::string path, bool folder);
        bg();  
};

#endif