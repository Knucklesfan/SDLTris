#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "game.h"
#include "font.h"

class results {
    public:
	bool loadgame = false;
	SDL_Renderer* renderer;
	SDL_Window* window;
	bg background;
	Mix_Music* music;
	Mix_Chunk** sound;

	font* newft;
	font* newhead;

	SDL_Texture* rendertext;
	unsigned int time = 0;
	std::vector<SDL_Texture*> texture;
	double leftangle = 0.0;
	results(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures,  Mix_Music* musicVec, Mix_Chunk** soundVec, std::vector<font*> fonts);
	void keyPressed(SDL_Keycode key);
	void render(game* game);
	void logic(double deltatime);
	int endlogic();
	void reset();
	const int tpiece[6] = {
		 0,2,0,
		 2,2,2
	};
	const int lpiece[6] = {
			 2,2,2,
			 2,0,0
			};
	private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
	void drawCubes(const int position[], int x, int y, int size, int width, std::vector<SDL_Texture*> textures, double angle, double scale, int texture);

    
};