#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "../opengl/background.h"
#include "../opengl/buffermanager.h"
#include "../opengl/font.h"
#include "../engine/gamemode.h"
#include "../opengl/plane.h"
#include "../engine/globalgamemode.h"
#include "game.h"

class titlescreen : public Gamemode
{
public:
	
	double time = 0;
	int startTime = 0;
	int currentscreen = 0;
	int bgnum = 0;
	int rotationx = 0;
	bool active = true;
	bool refreshActive = false;
	game* frontGame;
	titlescreen();
	GlobalGamemode* gg;
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

