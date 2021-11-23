#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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
	TTF_Font* ttf;

	font* newft;
	font* newhead;

	TTF_Font* header;
	unsigned int time = 0;
	std::vector<SDL_Texture*> texture;

	results(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures,  Mix_Music* musicVec, Mix_Chunk** soundVec, std::vector<font*> fonts);
	void keyPressed(SDL_Keycode key);
	void render(game* game);
	void logic(double deltatime);
	int endlogic();
	void reset();
private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
	void renderfont(int x, int y, std::string text, bool selected, TTF_Font* size);
    
};