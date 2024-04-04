#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
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
#include "../opengl/model.h"
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
		int testblocks[480], ghostblocks[480],  previousblocks[480];
		// int (activations)[5][6];
		tetrimino t;
		ghostblock g;
		double visibility = 1.0;
		double visiblelifetime = 0;
		double ticks = 0.0;
		Uint32 realtick = 0;
		int nextblocks = 0;
		int holdblock = 0;
		uint randomIters = 0; // number of times the rand() call has been made
		Uint32 score = 000000000;
		int lines = 0;
		int level = 1;
		int bglevel = 1;
		int currentsong = 0;
		int pauseselection = 0;
		double warningalpha = 0.0;
		double alphalifetime = 0;
		Uint32 gameStart = 0;
		bool goup = false;
		bool godown = false;
		bool warningflag = false;
		model* toad;
		std::string choices[6] = {
			"RESUME",
			"EXIT",
			"RECORD DEMO",
			"PLAY DEMO",
			"SAVE STATE",
			"LOAD STATE"
		};
		double rotval = 0.0;
		int optionsize = 6;
		bool gameactive = true;
		bool paused = false;
		// std::vector<SDL_Texture*> textures;
    	#ifdef __LEGACY_RENDER
			SDL_Texture* texture;
		#else
			cube* cubeRenderer;
			buffermanager* playfield; //the buffer that is used for the playfield
		#endif
		// std::vector<bg>  backgrounds;
		game();
		void input(SDL_Keycode key);
		void inputKey(SDL_Keycode key);
		bool demoPlayback = false; //playing back a demo?
		bool demoRecord = false; //recording a demo?
		bool demoReturn = false;
		std::ofstream demofile;
		char * demo;
		size_t demoOffset = 0;
		Uint32 demotick = 0;
		SDL_Keycode demokey = 0;

		void render();
		void logic(double deltatime);
		int demoEndLogic();
		void setupDemo(std::string demofile);
		Transition endLogic();
		void reset();
		uint time = 0;
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
		void loadDemo(std::string demo);
		void saveState(); //temporary, debug class to save a level's state
		void loadState(); //loads the current level's state.
		void startRecord();
		void stopRecord();
		void changemusic();
		void shiftarray(int(array)[], int size, int shift);
		void checkLines(int(blocks)[240]);
		bool checkRow(int(blocks)[10]);
		void clearRow(int(blocks)[240], int y);
		void drawCubes(int position[],float scale, float x, float y, int size, int width,bool threed=false, glm::vec3 rotation={0,0,0});
};

