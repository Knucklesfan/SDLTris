#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
#define MENUSIZE 6
class debugscene : public Gamemode { //literally just renders a white screen to test transitions
    int r = 128 + rand() % (256 - 128);
    int g = 128 + rand() % (256 - 128);
    int b = 128 + rand() % (256 - 128);
    bool advance = false;
    int transition = 0;
    int background = 0;
    int selection = 0;
    int gamemode = 0;
    int* endings[MENUSIZE] = {
        &transition,
        &background,
        &gamemode,
        nullptr,
        nullptr,
        nullptr,
    };
    std::string menu[MENUSIZE] = {
        "Change Transition ",
        "Change Background ",
        "Goto Gamemode ",
        "Enable FPS counter",
        "Boot game normally",
        "List currently loaded assets -unimplemented-"
    };
    std::string transitions[4] = {
        "FADE",
        "BARS",
        "BLOCKS",
        "CIRCLE"
    };
    void render();
    void input(SDL_Keycode keysym);
    Transition endLogic();
    void logic(double deltatime);
    void reset();
};