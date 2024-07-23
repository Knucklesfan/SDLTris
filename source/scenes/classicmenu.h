#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <math.h>
#include "../background.h"
#include "../gamemode.h"
#include "../opengl/plane.h"
#include "../wireframecube.h"
#define NUMBUTTONS 5
#define ROWWIDTH 2
#define NEWGAMELENGTH 250.0
#define FALLTIME_OTHERS 100.0
#define FALLTIME_OTHERS_INTERVAL 50.0
#define TRANSITION_LENGTH 2000.0
enum MENUMODES { //simple binary enum
    NONE=0,
    CLASSIC=1,
    TRAINING=2,
    MULTIPLAYER=4,
    CHALLENGE=8,
    ALL=15
};
class classicmenu : public Gamemode { //the main menu of the game
    public:
    Transition t;
    plane* cd;
    bg* redbackground;
    bg* bluebackground;
    buffermanager* buff;
    wireframecube* cube;
    int buttonx = 0;
    int selection = 0;
    int currentscreen = 0;
    float transition = 0.0f;
    bool godown = false;
    bool goup = false;
    int screenmode = 0;
    int explosionTime = 0;
    Uint32 startTime = 0;
    Uint32 transitionTime = 0;
    Uint32 currentscreenAge = 0;
    Uint64 newModifiers = 0b10000010; //TODO: Make more than 64 modifiers so we actually have to recode this :)
    Uint64 activeMods = 0b000001;

    int visiblesettings = 0;
    int subselection = 0;
    int newMods = 0;
    int selectedMod = 0;
    int gamemodeSelection = 1;
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
    private:
        const std::string gamemodes[4] = {
            "CLASSIC",
            "TRAINING",
            "MULTIPLAYER",
            "CHALLENGE"
        };
        const int gamemodesVisibility[4] = {
            CLASSIC,
            TRAINING,
            MULTIPLAYER,
            CHALLENGE
        };
        const std::string defaultsettings[7] = {
            "DIFFICULTY",
            "START AT LEVEL",
            "PLAY ANIMATIONS",
            "NO SPEEDUP",
            "NO GRAVITY",
            "BLOCK SEED",
            "STAGE"
        };
        const int defaultsettingVisiblity[7] = {
            ALL,
            CLASSIC|TRAINING|MULTIPLAYER,
            ALL,
            TRAINING,
            TRAINING,
            TRAINING,
            CHALLENGE
        };

};