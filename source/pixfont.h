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
#include <unordered_map>
#include <utility>      // std::pair, std::make_pair
#include "utils/defs.h"
#include "font.h"
#define MAXSTRING 256 //this is the max size that a string can be, increase if needed more!
//so, why am I doing this?
//several reasons, actually.
//Mostly, I just hate SDL_TTF with an undying passion, (this is untrue)
//I feel the current text system to create a texture each time is very inefficient (SDL_TTF is slower than the bg rendering!) (this was also untrue)
//I know this is mostly bad programming and not *technically* SDL_TTF's fault, but honestly TTF fonts just really don't fit with the game at this state (this was true for SDLTetris)
//I'm going for some kind of aesthetic, and TTF just ruins that with the fonts looking jank as hell, especially on lower resolution screens (this is true)
//(ofc most of this game looks weird upscaled but meh i wanna implement dynamic resolutions eventually anyways) (this was untrue)
//the main issue is that the game is currently hard locked to 640x480, which is dumb (this is something i never fixed)
//hope to make that better in the future. (future never came)
//this also means that text can have effects maybe? I might wanna implement a sin wave or something (you implemented this)
//rotatable text? (never added this)
//ofc to have rotatable text i'd have to implement a rendererer similar to the awful one i have for the backgrounds rn (background.h is a masterpiece of coding)


//this is effectively just a struct wrapper for what i consider a font. (who is reading this)
//might change later! (it never changed)
class pixfont : public Font
{
	public:
        color colors[10] = {
            color(255,255,255),
            color(255,0,0), //1
            color(0,255,0), //2
            color(0,0,255), //3
            color(255,255,0), //4
            color(255,0,255), //5
            color(0,255,255), //6
            color(10,10,10), //7
            color(0,0,0), //8
            color(0,0,0) //9
        };

        double incrementsx[10];
        double incrementsy[10];
        double layerposx[10];
        double layerposy[10];
        double angle = 0.0;
        double rotation;
        glm::vec2 translations[MAXSTRING];
        glm::vec2 texcoords[MAXSTRING];
	    void logic(double deltatime) {};
        int width = 8;
        int height = 8;
        int numberpos = 0;
        int alphabetpos = 10;
        int specialpos = 36;
        std::string name;
        std::string path;
        std::unordered_map<char, letter> mapping;
        void render(std::string words, int x, int y, bool center, 
        int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout, double scale);
        void render(int x, int y,std::string words, bool center, 
        int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout);
        
        void render(int x, int y, std::string strg, bool center);
        #ifdef __LEGACY_RENDER
        SDL_Texture* txt;
        #else
        texture* txt;
        shader* shad;
        #endif
        pixfont(std::string path);
        pixfont();

    private:
    	#ifdef __LEGACY_RENDER
    	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);
        #endif
        bool hasEnding(std::string const& fullString, std::string const& ending);
        void generateSurfaces(std::string path);
        static bool compareFunction (std::string a, std::string b) {return a<b;} 
        std::vector<std::string> seperateWords(std::string string);
        std::vector<std::string> split(const std::string& input, char delimiter);
        std::string wrap(std::string str, int pixels);
        color tintColor(const color& baseColor, const color& tintColor);
        glm::mat4 projection;
        unsigned int quadVAO;
        unsigned int instanceVBO;

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
