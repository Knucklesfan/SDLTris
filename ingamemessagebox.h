#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <SDL2/SDL_ttf.h>

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
        TTF_Font* font;
        ingamemessagebox(std::string name, std::string desc, SDL_Renderer* renderer, TTF_Font* font, int loc);
        ingamemessagebox();

        void renderfont(int x, int y, std::string strg, bool selected, TTF_Font* size, SDL_Renderer* renderer);

        void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center);

    private:

    };
