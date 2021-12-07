#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <map>
#include <utility>      // std::pair, std::make_pair


//so, why am I doing this?
//several reasons, actually.
//Mostly, I just hate SDL_TTF with an undying passion, 
//I feel the current text system to create a texture each time is very inefficient (SDL_TTF is slower than the bg rendering!)
//I know this is mostly bad programming and not *technically* SDL_TTF's fault, but honestly TTF fonts just really don't fit with the game at this state
//I'm going for some kind of aesthetic, and TTF just ruins that with the fonts looking jank as hell, especially on lower resolution screens
//(ofc most of this game looks weird upscaled but meh i wanna implement dynamic resolutions eventually anyways)
//the main issue is that the game is currently hard locked to 640x480, which is dumb
//hope to make that better in the future.
//this also means that text can have effects maybe? I might wanna implement a sin wave or something
//rotatable text?
//ofc to have rotatable text i'd have to implement a rendererer similar to the awful one i have for the backgrounds rn

class font
{
    
	public:
        double incrementsx[10];
        double incrementsy[10];
        double layerposx[10];
        double layerposy[10];
        double angle = 0.0;
        double rotation;
        
	    void logic(double deltatime);
        int size = 0;
        int width = 8;
        int height = 8;
        int numberpos = 0;
        int alphabetpos = 10;
        int specialpos = 36;
        std::string name;
        std::string path;
        std::map<char, int> mapping;
        void render(SDL_Renderer* renderer, std::string words, int x, int y, bool center);
        void render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout);
        void render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer);
        void render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer, bool sine, double pos, double multiplyin, double multiplyout);
        void render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer, int wordwrap);
        void render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green);
        void render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap);
        SDL_Texture* texture;
        font(std::string path, SDL_Renderer* renderer);
        font();

    private:
    	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);
        bool hasEnding(std::string const& fullString, std::string const& ending);
        void generateSurfaces(std::string path, SDL_Renderer* renderer);
        static bool compareFunction (std::string a, std::string b) {return a<b;} 
        std::vector<std::string> seperateWords(std::string string);
        std::string wrap(std::string str, int pixels);
};
