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
#define SAVE_VERSION 1
#define FILENAME_LENGTH 8
//specifies the specific version of the saving algorithm this uses. Good for futureproofing in case that certain versions do get implemented.
//TODO: clean all of this nonsense up
class game: public Gamemode
{
	public:

		char displayKeys[40] = { //the keys to render to the screen
			'1','2','3','4','5','6','7','8','9','0',
			'Q','W','E','R','T','Y','U','I','O','P',
			'A','S','D','F','G','H','J','K','L','-',
			'Z','X','C','V','B','N','M','<','>','$'
		};
		char keyboardKeys[40] = { //the actual keys that are added to the string
			'1','2','3','4','5','6','7','8','9','0',
			'Q','W','E','R','T','Y','U','I','O','P',
			'A','S','D','F','G','H','J','K','L',32,
			'Z','X','C','V','B','N','M',0,0,0
		};


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
		Uint32 invisScore = 0; //score that is hidden and reset on holdblock, and is only added once falling is done.
		int lines = 0;
		int level = 1;
		int startTime = 0;
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
		std::string choices[3] = {
			"RESUME",
			"EXIT WITHOUT SAVING",
			"SAVE AND EXIT",
		};
		double rotval = 0.0;
		int optionsize = 3;
		bool gameactive = true;
		bool paused = false;
		bool keyboard = false;
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
		int selectedkey = 0; //keyboard's current key
		char keyboardname[8]; //the keyboard's current message
		int currentChar = 0;
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

