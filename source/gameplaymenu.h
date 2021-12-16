#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "font.h"
#include <array>
#include "sine.h"
#include <math.h>
#include "background.h"
#include "cube.h"


class gameplaymenu {
    public:
    gameplaymenu();
    void render(SDL_Renderer* renderer, std::vector<font*> fonts, bg* background, std::vector<SDL_Texture*>* text);
    void logic(double deltatime);
    void keyPressed(SDL_Keycode);
    int endlogic();
    void reset();
};