#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "../engine/gamemode.h"
#include "../opengl/buffermanager.h"
#include "../opengl/plane.h"

class highscore : public Gamemode {
    public:

	buffermanager* spaceBuff;
    plane* logo;
    plane* pres;
    int animationindex = 0;
    int animationtime = 0;
	float alpha = 0.0;
	highscore();
	void input(SDL_Keycode key);
	void render();
	void logic(double deltatime);
	Transition endLogic();
	void reset();
	private:
        void handleAnimation();
        bool refreshBackdrop = true;
    
};