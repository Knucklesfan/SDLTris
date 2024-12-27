#pragma once
#include "gamemode.h"
#include "../opengl/cube.h"
#include "../opengl/buffermanager.h"
#include "../opengl/model.h"
//Global Acces Gamemode
//This is a somewhat pseudo gamemode. Unlike the scripted gamemodes or the hardcoded gamemodes, this mode runs in the foreground at all times, directly above all other gamemodes.
//Handles certain global rendering functions, debug mode, fading between.
//The Global gamemode accepts some gamemode like functions, like render and logic, but also supports odd calls as well.
//Also, this gamemode should NOT have endlogic, as it shouldn't ever end.

class GlobalGamemode {
    public:
        GlobalGamemode();
        int logic(double); 
        void startRender();
        void render();
        void setFade(Transition);
        Transition currentTransition;
        buffermanager buffer= buffermanager(640,480);
        cube* outer;
        cube* inner;
        bool active = false;

    private:
        model* glass;
        bool fade = false;
        double fadespeed = 0;
        double alpha = 0.0;
        int random[128]; //128 random values that are set every time a transition starts.
        //this is used for the glass shattering effect, and is only filled when necessary.
        //Sorry for using 128 words!
};
