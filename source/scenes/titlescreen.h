#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "../background.h"
#include "../opengl/buffermanager.h"
#include "../font.h"
#include "../gamemode.h"
#include "../opengl/plane.h"

class titlescreen : public Gamemode
{
public:
	
	double time = 0;
	int currentscreen = 0;
	int bgnum = 0;
	int rotationx = 0;
	titlescreen();
	buffermanager* buff;
	plane* p;
	void input(SDL_Keycode key);
	void render();
	void logic(double deltatime);
	Transition endLogic();
	void reset();
private:
	void debuginput(SDL_Keycode key);
};

