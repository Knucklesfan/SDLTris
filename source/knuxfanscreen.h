#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "background.h"


class knuxfanscreen {
public:
	std::vector<bg> backgrounds;
	int backnum;
    knuxfanscreen(SDL_Renderer* render, std::vector<SDL_Texture*> texture, std::vector<bg>  backg, Mix_Chunk* soundVec[], int background);
	void render();
	void logic(double deltatime);
	int endlogic();
	void reset();
    double alpha = 1.0;
    bool godown = false;
    bool goup = false;
    bool active = false;
    double lifetime = 0.0;
    Mix_Chunk** sound;
	SDL_Renderer* renderer;
	std::vector<SDL_Texture*> textures;


private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);

};