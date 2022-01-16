#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "options.h"
#include "tetriminos.h"
#include "ghostblock.h"
#include "background.h"
#include "server.h"
#include "ingamemessagebox.h"
#include <random>
#include "font.h"
//TODO: clean all of this nonsense up
class game
{
	public:




		int boardwidth = 10;
		int boardheight = 24;
		font* bodyfont;
		font* header;
		int testblocks[1024], ghostblocks[1024],  previousblocks[1024];
		SDL_Renderer* renderer;
		Mix_Music** music;
		int (activations)[5][6];
		Mix_Chunk** sound;
		tetrimino t;
		ghostblock g;
		double visibility = 1.0;
		double visiblelifetime = 0;
		double ticks = 0.0;
		int realtick = 0;
		int nextblocks = 0;
		int holdblock = 0;
		Uint32 score = 000000000;
		int lines = 0;
		int level = 1;
		int bglevel = 1;
		int currentsong = 0;
		int pauseselection = 0;
		double warningalpha = 0.0;
		double alphalifetime = 0;
		bool goup;
		bool godown;
		bool warningflag = false;
		std::string choices[2] = {
			"RESUME",
			"EXIT"
		};
		double rotval = 0.0;
		int optionsize = 2;
		bool gameactive;
		bool paused;
		std::vector<SDL_Texture*> textures;
    	SDL_Texture* texture;
		std::vector<bg>  backgrounds;
		game(SDL_Renderer* renderman, SDL_Window* window, std::vector<SDL_Texture*> texture, std::vector<bg>  backg, Mix_Music* musicVec[], Mix_Chunk* soundVec[], std::vector<font*> fonts, int(active)[5][6]);
		double layerpos[10];
		void keyPressed(SDL_Keycode key);
		void render();
		void logic(double deltatime);
		int endlogic();
		void reset();
		unsigned int time = 0;
		ingamemessagebox* msg;
		double getspeed();
		double lineclears[25] = {
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		};
		int volume;
	private:
		void changemusic();
		void shiftarray(int(array)[], int size, int shift);
		void checkLines(int(blocks)[240]);
		bool checkRow(int(blocks)[10]);
		void clearRow(int(blocks)[240], int y);
		void drawCubes(int position[],double scale, int x, int y, int size, int width, std::vector<SDL_Texture*> textures, SDL_Renderer* renderer);
		void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale);
		void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
};

