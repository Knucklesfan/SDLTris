#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "../opengl/pixfont.h"
#include <array>
#include "../opengl/sine.h"
#include <math.h>
#include "../opengl/background.h"
#include "../engine/gamemode.h"
#include "../opengl/plane.h"
#include "../opengl/model.h"

#define NUMBUTTONS 5
#define ROWWIDTH 2
class gameplaymenu : public Gamemode { //the main menu of the game
    public:
    Transition t;
    plane* cd;
    model* lamp;
    model* ball;
    glm::vec3 cdPos;
    int buttonx = 0;
    int selection = 0;
    int currentscreen = 0;
    float transition = 0.0;
    bool doDownTransition = false;
    bool doDownReturnTransition = false;
    Uint32 startTime;
    Uint32 currentscreenAge;
    int subselection = 0;
    int subbuttons = 0;

    gameplaymenu();
    void render();
    void logic(double deltatime);
    void input(SDL_Keycode);
    Transition endLogic();
    void reset();
};