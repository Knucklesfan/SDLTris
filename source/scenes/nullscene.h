#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "../opengl/background.h"
#include "../opengl/font.h"
#include "../engine/gamemode.h"


class nullscene : public Gamemode {
public:
    nullscene();
	void render();
	void input(SDL_Keycode key);
	void logic(double deltatime);
	Transition endLogic();
	void reset();
    bg backg;

};