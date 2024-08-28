#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "game.h"
#include "../opengl/font.h"
#include "../opengl/buffermanager.h"

class results : public Gamemode {
    public:
	bool loadgame = false;
	bg background;

	Font* newft;
	Font* newhead;

	buffermanager* blockBuff;
	cube* cubeRenderer;
	unsigned int time = 0;
	std::vector<SDL_Texture*> texture;
	double leftangle = 0.0;
	results();
	void input(SDL_Keycode key);
	void render();
	void logic(double deltatime);
	Transition endLogic();
	void reset();
	const int tpiece[6] = {
		 0,2,0,
		 2,2,2
	};
	const int lpiece[6] = {
			 4,4,4,
			 4,0,0
			};
	private:
	void drawCubes(const int position[], float scale, float x, float y, int size, int width, bool threed, glm::vec3 rotation);

    
};