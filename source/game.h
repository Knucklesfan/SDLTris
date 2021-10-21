#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "tetriminos.h"
#include "ghostblock.h"

class game
{
	public:
		int testblocks[200], ghostblocks[200],  previousblocks[200];
		double testangles[200], testscale[200], ghostscale[200];
		SDL_Renderer* renderer;
		Mix_Music** music;
		Mix_Chunk** sound;
		tetrimino t;
		ghostblock g;
		double ticks;
		int realtick;
		int nextblocks[16];
		int holdblock;
		bool gameactive;
		std::vector<SDL_Texture*> textures;

		game(SDL_Renderer* renderman, SDL_Window* window, std::vector<SDL_Texture*> texture, Mix_Music* musicVec[], Mix_Chunk* soundVec[]);
		double layerpos[10];
		void keyPressed(SDL_Keycode key);
		void render();
		void logic(double deltatime);
		int endlogic();
		void reset();

	private:
		void shiftarray(int(array)[], int size, int shift);
		void checkLines(int(blocks)[200]);
		bool checkRow(int(blocks)[10]);
		void clearRow(int(blocks)[200], int y);
		void drawCubes(int position[], double angles[], double scale[], int x, int y, int size, int width, std::vector<SDL_Texture*> textures, SDL_Renderer* renderer);
		void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale);
};

