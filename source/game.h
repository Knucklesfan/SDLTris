#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "tetriminos.h"
#include "ghostblock.h"
#include "background.h"
#include "server.h"
#include "ingamemessagebox.h"
#include <random>

class game
{
	public:
		TTF_Font* font;
		TTF_Font* header;
		int testblocks[200], ghostblocks[200],  previousblocks[200];
		double testangles[200], testscale[200], ghostscale[200];
		SDL_Renderer* renderer;
		Mix_Music** music;
		Mix_Chunk** sound;
		tetrimino t;
		ghostblock g;
		double ticks = 0.0;
		int realtick = 0;
		int nextblocks = 0;
		int holdblock = 0;
		Uint32 score = 000000000;
		int lines = 0;
		int level = 1;
		int currentsong = 0;
		int pauseselection = 0;
		std::string options[2] = {
			"RESUME",
			"EXIT"
		};
		int optionsize = 2;
		bool gameactive;
		bool paused;
		std::vector<SDL_Texture*> textures;
		std::vector<bg>  backgrounds;
		game(SDL_Renderer* renderman, SDL_Window* window, std::vector<SDL_Texture*> texture, std::vector<bg>  backg, Mix_Music* musicVec[], Mix_Chunk* soundVec[]);
		double layerpos[10];
		void keyPressed(SDL_Keycode key);
		void render();
		void logic(double deltatime);
		int endlogic();
		void reset();

		ingamemessagebox msg;

	private:
		void changemusic();
		void shiftarray(int(array)[], int size, int shift);
		void checkLines(int(blocks)[200]);
		bool checkRow(int(blocks)[10]);
		void clearRow(int(blocks)[200], int y);
		void drawCubes(int position[], double angles[], double scale[], int x, int y, int size, int width, std::vector<SDL_Texture*> textures, SDL_Renderer* renderer);
		void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale);
		void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
		void renderfont(int x, int y, std::string text, bool selected, TTF_Font* size);
};

