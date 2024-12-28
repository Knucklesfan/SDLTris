#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "game.h"
#include "../opengl/buffermanager.h"

class highscore : public Gamemode {
    public:

	buffermanager* spaceBuff;
    int animationindex = 0;
    int animationtime = 0;
	highscore();
	void input(SDL_Keycode key);
	void render();
	void logic(double deltatime);
	Transition endLogic();
	void reset();
	private:
        glm::vec3 logo_pos;
        glm::vec3 logo_rot;
        glm::vec3 logo_scl;

        glm::vec3 pres_pos;
        glm::vec3 pres_rot;
        glm::vec3 pres_scl;
        void handleAnimation();
        bool refreshBackdrop = true;
    
};