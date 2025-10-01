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
#define PAUSE_OPTIONS 3
//specifies the specific version of the saving algorithm this uses. Good for futureproofing in case that certain versions do get implemented.
//TODO: clean all of this nonsense up
class game: public Gamemode
{
	public:

		game();
		void input(SDL_Keycode key);
		void inputKey(SDL_Keycode key);
		void setMods(Uint64 mods[8]);

		bool keyboardState = false; //true if that ol keyboard is up
		void render();
		void logic(double deltatime);
		int demoEndLogic();
		void setupDemo(std::string demofile);
		Transition endLogic();
		void reset();
		void loadState(std::string); //loads the current level's state.

		int level = 1; //actual level of the game
		int difficulty = 0; //difficulty int!
		bool demoPlayback = false; //playing back a demo?

	private:
		void loadDemo(std::string demo);

		void saveState(); //temporary, debug method to save a level's state

		void startRecord();
		void stopRecord();

		void changemusic();
		void shiftarray(int(array)[], int size, int shift);
		void checkLines(int(blocks)[480]);
		bool checkRow(int(blocks)[10]);
		void clearRow(int(blocks)[480], int y);
		void drawCubes(int position[],float scale, float x, float y, int size, int width,bool threed=false, glm::vec3 rotation={0,0,0});
		double getspeed();

		int addScore(SCORETYPE type, int times, bool invisible); //add a certain value to score, under certain conditions.
		int parsePassive(); //parses passive scores unrelated to the actual ticks of gameplay (ie combo).

		ingamemessagebox* msg;

		uint time = 0; //the time that the game started
		
		Uint64 ticktimer = 0; //the tick of the current time

		int nextblocks = 0;
		int holdblock = 0;
		uint randomIters = 0; // number of times the rand() call has been made
		Uint32 score = 0;
		Uint32 invisScore = 0; //score that is hidden and reset on holdblock, and is only added once falling is done.
		int lines = 0;
		int linecounter = 0; //tally of the lines before progressing to next level

		int bglevel = 1; //the current background level

		const int boardwidth = 10; //seting these as const because im not sure if
		const int boardheight = 24; //big boards are coming back
		Uint64 activeMods[8] = {0,0,0,0,0,0,0,0}; //unfortunately hard limit of 512 mods can be activated, although an infinite* number can be loaded.

		int currentsong = 0; //current background for which music is playing for
		double rotval = 0.0;

		int pauseselection = 0;

		float warningalpha = 0.0;
		float alphalifetime = 0;
		
		bool warningGoRed = false;
		bool warningGoDown = false;
		bool warningflag = false; //should we show a warning?

		bool gameactive = true;
		bool paused = false;

		Uint32 gameStart = 0;

		int gameBlocks[480], ghostblocks[480],  previousblocks[480];

		tetrimino t;
		ghostblock g;

		cube* cubeRenderer;
		buffermanager* playfield; //the buffer that is used for the playfield
		plane* p;
		keyboard* keyb;

		double visibility = 1.0; //this will return as a modifier
		double visiblelifetime = 0;

		char * demoRaw;
		bool demoRecord = false; //recording a demo?
		bool demoReturn = false;
		std::ofstream demofile; //used mainly only when recording a demo
		size_t demoOffset = 0; //the current read offset when playing back a demo
		Uint32 demotick = 0; //the next cached tick of a demo
		SDL_Keycode demokey = 0; //the key to press in a demo, next time its your time to play back

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
		std::string pauseMenu[PAUSE_OPTIONS] = {
			"RESUME",
			"EXIT WITHOUT SAVING",
			"SAVE AND EXIT",
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

		std::vector<ScoreOperation> scoreOperations;
		std::vector<ScoreOperation> gravityOperations;
		std::vector<ComboOperation> comboOperations;
		blockChances chances;
};

