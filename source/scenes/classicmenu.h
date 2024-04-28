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
#include "../opengl/model.h"
#include "../wireframecube.h"
#define NUMBUTTONS 5
#define ROWWIDTH 2
class classicmenu : public Gamemode { //the main menu of the game
    public:
    Transition t;
    plane* cd;
    bg* background;
    wireframecube* cube;
    int buttonx = 0;
    int selection = 0;
    int currentscreen = 0;
    float transition = 0.0f;
    bool godown = false;
    bool goup = false;
    Uint32 startTime;
    Uint32 currentscreenAge;
    int subselection = 0;
    int subbuttons = 0;
    int savenum = 6;
    int level = 0;
    int lines = 0;
    int savedatatest[480]; //testing code
    classicmenu();
    void render();
    void logic(double deltatime);
    void input(SDL_Keycode);
    Transition endLogic();
    void reset();
};