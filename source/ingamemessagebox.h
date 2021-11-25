#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include "font.h"

class ingamemessagebox
{
    public:
        double x = 0;
        double y = 0;
        bool goup = false;
        bool godown = false;
        bool active = false;
        double uptime = 0;
        void logic(double deltatime);
        void activate(std::string title, std::string desc);
        std::string name;
        std::string content;
        void render(SDL_Renderer* renderer);
        std::vector<SDL_Texture*> textures;
        SDL_Renderer* renderer;
        font* letterfont;
        ingamemessagebox(std::string name, std::string desc, SDL_Renderer* renderer, std::vector<SDL_Texture*> texture, font* font, int loc);
        ingamemessagebox();

        void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);

    private:

    };
