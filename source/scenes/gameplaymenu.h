#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "../pixfont.h"
#include <array>
#include "../sine.h"
#include <math.h>
#include "../background.h"
#include "../gamemode.h"
#include "../opengl/plane.h"

#define NUMBUTTONS 5
#define ROWWIDTH 2
class gameplaymenu : public Gamemode { //the main menu of the game
    public:
    plane* cd;
    glm::vec3 cdPos;
    int buttonx = 0;
    int selection = 0;
    int currentscreen = 0;
    int currentdialog = 0;
    Uint32 startTime;
    gameplaymenu();
    void render();
    void logic(double deltatime);
    void input(SDL_Keycode);
    Transition endLogic();
    void reset();
};