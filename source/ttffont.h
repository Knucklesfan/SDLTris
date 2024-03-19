#pragma once

#include "font.h"
#include "utils/defs.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <map>

//basic wrapper for the font class to support sdl_ttf, mainly to make it easier i guess?
struct ttfchar {
    unsigned int textureid;  // ID handle of the glyph texture
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

class ttffont : public Font
{
	public:
        shader* shad;
        unsigned int VAO,VBO;
        glm::mat4 projection;
        std::map<char, ttfchar> characters;

        void render(std::string words, int x, int y, bool center);
        void render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout, double scale);
        void render(int x, int y, std::string strg, bool center);
        void render(int x, int y, std::string strg, bool center, bool sine, double pos, double multiplyin, double multiplyout);
        void render(int x, int y, std::string strg, bool center, int wordwrap);
        void render(std::string words, int x, int y, bool center, int red, int blue, int green);
        void render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap);
        void logic(double deltatime) {};
        ttffont(std::string name);

        
};
