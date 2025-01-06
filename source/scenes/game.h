#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <cstring>

#include "../gameplay/tetriminos.h"
#include "../gameplay/ghostblock.h"
#include "../engine/gamemode.h"
#include "../opengl/cube.h"
#include "../opengl/model.h"
#include "../opengl/plane.h"
#include "../gameplay/ingamemessagebox.h"
#include "../opengl/font.h"
#include "../gameplay/keyboard.h"
#define SAVE_VERSION 2
#define FILENAME_LENGTH 8
//specifies the specific version of the saving algorithm this uses. Good for futureproofing in case that certain versions do get implemented.
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
		Uint64 ticktimer = 0; //the tick of the current time
		int nextblocks = 0;
		int holdblock = 0;
		uint randomIters = 0; // number of times the rand() call has been made
		Uint32 score = 000000000;
		Uint32 invisScore = 0; //score that is hidden and reset on holdblock, and is only added once falling is done.
		int lines = 0;
		int linecounter = 0;
		int level = 1;
		int startTime = 0;
		int bglevel = 1;
		int currentsong = 0;
		int difficulty = 0;
		Uint64 activeMods[8] = {0,0,0,0,0,0,0,0}; //unfortunately hard limit of 512 mods can be activated, although an infinite* number can be loaded.
		int pauseselection = 0;
		double warningalpha = 0.0;
		double alphalifetime = 0;
		Uint32 gameStart = 0;
		bool goup = false;
		bool godown = false;
		bool warningflag = false;
		std::string choices[3] = {
			"RESUME",
			"EXIT WITHOUT SAVING",
			"SAVE AND EXIT",
		};
		double rotval = 0.0;
		int optionsize = 3;
		bool gameactive = true;
		bool paused = false;
		// std::vector<SDL_Texture*> textures;
    	#ifdef __LEGACY_RENDER
			SDL_Texture* texture;
		#else
			cube* cubeRenderer;
			buffermanager* playfield; //the buffer that is used for the playfield
			plane* p;
		#endif
		// std::vector<bg>  backgrounds;
		game();
		void input(SDL_Keycode key);
		void inputKey(SDL_Keycode key);
		void setMods(Uint64 mods[8]);
		bool demoPlayback = false; //playing back a demo?
		bool demoRecord = false; //recording a demo?
		bool demoReturn = false;
		std::ofstream demofile;
		char * demo;
		size_t demoOffset = 0;
		Uint32 demotick = 0;
		SDL_Keycode demokey = 0;
		keyboard* keyb;
		bool keyboardState = false; //true if that ol keyboard is up
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
		void saveState(); //temporary, debug method to save a level's state
		void loadState(std::string); //loads the current level's state.
		void startRecord();
		void stopRecord();
		void changemusic();
		void shiftarray(int(array)[], int size, int shift);
		void checkLines(int(blocks)[480]);
		bool checkRow(int(blocks)[10]);
		void clearRow(int(blocks)[480], int y);
		void drawCubes(int position[],float scale, float x, float y, int size, int width,bool threed=false, glm::vec3 rotation={0,0,0});
		float scoreFlip[11] = { //look, i know only 10 of the digits will flip, but honestly who's getting a score better than 10 digits?
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f //this 11th value is used for the bumper thingy
		};
		float scoreFlipStrength[11] = {
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f //this 11th value is used for the bumper thingy

		};
};

