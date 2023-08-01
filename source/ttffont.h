#pragma once

#include "font.h"
#include "SDL_FontCache/SDL_FontCache.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
//basic wrapper for the font class to support sdl_ttf, mainly to make it easier i guess?

class ttffont : public Font
{
	public:
        FC_Font* font;
        SDL_Texture* texture;
        void render(std::string words, int x, int y, bool center);
        void render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout, double scale);
        void render(int x, int y, std::string strg, bool center);
        void render(int x, int y, std::string strg, bool center, bool sine, double pos, double multiplyin, double multiplyout);
        void render(int x, int y, std::string strg, bool center, int wordwrap);
        void render(std::string words, int x, int y, bool center, int red, int blue, int green);
        void render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap);
        void logic(double deltatime) {};
        ttffont(std::string name, int size);
        
};
