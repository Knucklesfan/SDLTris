#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>

class bg
{
    
	public:
        double incrementsx[10];
        double incrementsy[10];
        int layers = 0;

        std::string name;
        std::string path;
        std::string creator;
        std::string vers;

        std::vector<SDL_Texture*> textures;
        bg(std::string path, SDL_Renderer* renderer);
        bg();

    private:
        bool hasEnding(std::string const& fullString, std::string const& ending);
        std::vector<SDL_Surface*> generateSurfaces(std::string path);
        std::vector<SDL_Texture*> generateTextures(std::vector<SDL_Surface*> surfaces, SDL_Renderer* renderer);
        static bool compareFunction (std::string a, std::string b) {return a<b;} 
};
