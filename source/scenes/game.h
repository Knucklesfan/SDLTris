#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
// #include "options.h"
#include "../tetriminos.h"
#include "../ghostblock.h"
#include "../background.h"
#include "../gamemode.h"
#include "../opengl/cube.h"
#include "../ingamemessagebox.h"
#include <random>
#include "../font.h"
//TODO: clean all of this nonsense up
class game: public Gamemode
{
	public:




		int boardwidth = 10;
		int boardheight = 24;
		Font* bodyfont;
		Font* header;
		int testblocks[1024], ghostblocks[1024],  previousblocks[1024];
		// int (activations)[5][6];
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
		// std::vector<SDL_Texture*> textures;
    	#ifdef __LEGACY_RENDER
			SDL_Texture* texture;
		#else
			cube* cubeRenderer;
			buffermanager* playfield; //the buffer that is used for the playfield
		#endif
		// std::vector<bg>  backgrounds;
		game();
		double layerpos[10];
		void input(SDL_Keycode key);
		void render();
		void logic(double deltatime);
		Transition endLogic();
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
		void drawCubes(int position[],float scale, float x, float y, int size, int width,bool threed=false, glm::vec3 rotation={0,0,0});
};

