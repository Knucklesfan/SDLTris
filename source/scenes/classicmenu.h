#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <math.h>
#include "../opengl/background.h"
#include "../engine/gamemode.h"
#include "../opengl/plane.h"
#include "../opengl/wireframecube.h"
#include "../gameplay/keyboard.h"

#define NUMGAMEMODES 4
#define NUMSETTINGS 11
#define NUMDIFFICULTIES 4
#define RIGHTBUTTONS 3
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
    classicmenu();
    void render();
    void logic(double deltatime);
    void input(SDL_Keycode);
    Transition endLogic();
    void reset();
    private:
        Transition t;
        plane* cd;
        bg* redbackground;
        bg* bluebackground;
        buffermanager* buff;
        buffermanager* modtext;
        wireframecube* cube;
        keyboard* keyb;
        const std::string gamemodes[NUMGAMEMODES] = {
            " CLASSIC",
            " TRAINING",
            " MULTIPLAYER",
            " CHALLENGE"
        };
        const int gamemodesVisibility[NUMGAMEMODES] = {
            CLASSIC,
            TRAINING,
            MULTIPLAYER,
            CHALLENGE
        };
        const std::string defaultsettings[NUMSETTINGS] = {
            " DIFFICULTY",
            " START AT LEVEL",
            " PLAY ANIMATIONS",
            " NO SPEEDUP",
            " NO GRAVITY",
            " BLOCK SEED",
            " STAGE",
        };
        const int defaultsettingVisiblity[NUMSETTINGS] = {
            ALL,
            CLASSIC|TRAINING|MULTIPLAYER,
            ALL,
            TRAINING,
            TRAINING,
            TRAINING,
            CHALLENGE,
        };
        const std::string difficulties[NUMDIFFICULTIES] = {
            "EASY",
            "MEDIUM",
            "HARD",
            "INSANE"
        };
        const std::string difficultyDesc[NUMDIFFICULTIES] = {
            "Blocks fall rather slowly, levels transition after 20 lines, and all story mode objectives are set to 1x.",
            "Blocks fall somewhat quickly, levels transition after 40 lines, and all story mode objectives are set to 1.5x.",
            "Blocks fall fast, levels transition after 50 lines, and all story mode objectives are set to 2.0x.",
            "Blocks fall VERY fast, levels transition after 100 lines, and all story mode objectives are set to 5.0x."
        };
        const std::string selectionDesc[NUMSETTINGS] = {
            "The classic Tetrmino gameplay! Blocks are falling, so make sure you don't reach the top, and get that high score!",
            "Practice for the big game, with save-states, infinite lives, and an undo feature!",
            "Play online with two or more people! Choose between many different gamemodes. This mode will take you to it's own screen, so any settings and modules configured here are used during the queueing process.\n@4WARNING! @0You will only be able to change @4ONE@0 module once you find a match!",
            "Cut through some preset block shapes and try to clear the boards!",
            "Set your difficulty, between EASY, MEDIUM, HARD, and INSANE!",
            "Start your game at a set level number.",
            "Play background animations, or disable background animations",
            "Disable blocks from speeding up as levels increase.",
            "Disable gravity on blocks that are falling",
            "Set the random seed that's used for the blocks, to ensure RNG stays the same between trials.",
            "Sets which stage to play out of the challenge stages!"
        };
        const std::string selectionDescRight[3] = {
            "Select your modifiers here!",
            "Set up any texture packs you may have installed or may be interested in!",
            "Start your game!"
        };
        int selection = 0;
        float transition = 0.0f;
        float subTransition = 0.0f;
        int lastOffset = 0; //stupid variable, that is set to the last modifier list length of last time. a REAAAAALLLY slow size algorithm that waits a whole frame until it updates, but should be fine?
        bool rightSide = false; //true if on right side, false if on left
        bool modifierTab = false; //true if modifying data in modifierTab
        int screenmode = 0;
        int explosionTime = 0;
        int subscreen = 0;
        Uint64 startTime = 0;
        Uint64 transitionTime = 0;
        Uint64 currentscreenAge = 0;
        Uint64 subscreenAge = 0;
        Uint64 newModifiers = 0b10000000; //TODO: Make more than 64 modifiers so we actually have to recode this :)
        Uint64 activeMods = 0b000000;
        Uint8 activeToggles = 0b00000100; //number of active settings that can be toggled (play animations, no speedup etc)
        std::string gameseed = "";
        int visiblesettings = 0;
        int subselection = 0;

        int difficultySelection = 0;
        int levelStart = 0;
        int challengeStage = 0;
        int newMods = 0;
        int selectedMod = 0;
        int gamemodeSelection = 1;
        int subbuttons = 0;
        private:
            void startGame();
            int getCurrentOption();

};