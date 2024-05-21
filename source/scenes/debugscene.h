#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
#define MENUSIZE 7
class debugscene : public Gamemode { //literally just renders a white screen to test transitions
    public:
        void render();
        void input(SDL_Keycode keysym);
        Transition endLogic();
        void logic(double deltatime);
        void reset();
        debugscene();
    private:
        int r = 128 + rand() % (256 - 128);
        int g = 128 + rand() % (256 - 128);
        int b = 128 + rand() % (256 - 128);
        bool advance = false;
        int transition = 0;
        int background = 0;
        int selection = 0;
        int gamemode = 0;
        float starX[255];
        float starY[255];
        float starZ[255];
        float starBlock[255];

        int* endings[MENUSIZE] = {
            &background,
            &transition,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        };
        std::string menu[MENUSIZE] = {
            "Current Background #",
            "Transition to Transition #",
            "Goto Gamemode #",
            "Hide this menu",
            "Load game normally",
            "Toggle Framerate",
            "Close game"
        };
        std::string transitions[4] = {
            "FADE",
            "BARS",
            "BLOCKS",
            "CIRCLE"
        };

};