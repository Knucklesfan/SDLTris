#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "game.h"
#include "font.h"
#include "cube.h"
#include "background.h"

enum OPTIONTYPE
{
	GAMEPLAY = 0,
	DISPLAY = 1,
	SYSTEM = 2,
	EXTRA = 3,
	DEBUG = 4
};

enum GAMEPLAYOPTIONS
{
	GHOSTPIECE = 0,
	HOLDPIECE = 1,
	BLOCKSPEED = 2,
	FASTDROP = 3,
	SCORING = 4
};

enum DISPLAYOPTIONS
{
	BGMODE = 0,
	FIRSTBG = 1,
	LINECLEAR = 2,
	MOVINGBG = 3,
	NEARTOPFLASH = 4
};
enum SYSTEMOPTIONS
{
	FULLSCREEN = 0,
	MUSIC = 1,
	SOUNDS = 2,
	RESET = 3,
};

enum EXTRAOPTIONS
{
	ROTATEBOARD = 0,
	BIGGERBOARD = 1,
	BLINDMODE = 2,
};

enum DEBUGOPTIONS {
	DEBUGENABLED = 0
};

class options {
public:
	bool loadgame = false;
	SDL_Renderer* renderer;
	SDL_Window* window;
	bg background;
	std::vector<bg>  backgrounds;
	Mix_Music* music;
	Mix_Chunk** sound;
	cube* cub;
	cube* bigcub;
	font* newft;
	font* newhead;
	font* tickertext;
	double txtpos = 0;
	double bottompos = 0;
	int settingsx = 320;
	bool mvleft = false;
	bool mvright = false;
	int currenttitle = 0;
	int currentselection = 0;
	int currentscreen = 0;
	int currentmenuselect = 0;
	std::string bottomtck = "GREEZ TO THE FOLLOWING PEOPLE: KK, DURANGO, JOHNNY, BOOMBOOM, FRISBEE, BLAKE, CASPER, DARK PRINCE, KRIS, CONNOR, ELENA, QUOTES, ERIKA, BRE, PETERS, EMRETECH, GENERIC, BOTTMINT, M4XW, BEHEMOTH, NATINSULA, TOTALJUSTICE, MIRZAGHALIB, AND EVERYONE ON SMWCENTRAL AND EVERYWHERE ELSE WHO'S HELPED ME THIS FAR.     ";
	std::string tcktxt = "LEGENDARY DEVELOPER KNUXFAN PRESENTS HIS LATEST GAME- KNUXFANS TETRIMINOS. THEY MUST CREATE NEW DREAMS AND FILMS BY BREAKING TRADITIONAL STYLES. THE WORK, WHICH BECOMES A NEW GENRE ITSELF WILL BE CALLED... KNUXFAN'S TETRIMINOS. A TETRIS CLONE UNLIKE ANY OTHER WITH ASPIRATIONS FOR A HIGHER GAMEPLAY. USING CPP, SDL2 AND A WHOLE LOT OF POWER FROM MODERN PROCESSORS COMES AN AMIGA GAME FROM ANOTHER TIMELINE";
	SDL_Texture* rendertext;
	unsigned int time = 0;
	std::vector<SDL_Texture*> texture;
	double rot = 0.0;
	options(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures, Mix_Music* musicVec, Mix_Chunk** soundVec, std::vector<font*> fonts, std::vector<bg> backgcollection);
	void keyPressed(SDL_Keycode key);
	void render();
	void logic(float deltatime);
	int endlogic();
	void reset();
	std::string titles[4] = {
		"GAMEPLAY SETTINGS",
		"VISUAL SETTINGS",
		"SYSTEM SETTINGS",
		"EXTRA SETTINGS",
	};
	std::string opts[4][6] = {
		{
		"Ghost Piece",
		"Hold Piece",
		"Block Speedup",
		"Fast Drop",
		"",
		"Exit Menu",
		},
		{
		"Background mode",
		"First Background",
		"Line Clear animation",
		"Moving Backgrounds",
		"Near Top Flash",
		"Exit Menu",
		},
		{
		"Full Screen",
		"Music Volume",
		"Sound Volume",
		"Reset Settings",
		"",
		"Exit Menu",
		},
		{
		"Rotating Board",
		"Bigger Board??",
		"Blind Mode",
		"More coming in future updates!",
		"",
		"Exit Menu",
		},
	};
	std::string details[4][6] = {
		{
			"Shows a ghost piece near where your block will land.",
			"Allows you to hold a block in a container on the side of the screen.",
			"Enables faster blocks as levels increase. Disable for a more casual experience.",
			"Allows dropping blocks faster with the UP key. Recommended to use alongside Ghost Piece.",
			"Select which scoring system to use.",
			"Closes this menu."

		},
		{
			"Enable to cycle through backgrounds as levels increase, Disable to stay on the first background selected.",
			"Selects the first background to start each game with.",
			"Show an animation when clearing a line.",
			"Enables background motion.",
			"Flashes the side of the screen when blocks are near the top.",
			"Closes this menu."

		},
		{
			"Puts game in full screen.",
			"Adjust game music volume.",
			"Adjust game sound volume.",
			"Resets ALL SETTINGS back to factory.",
			"Deletes your save file, removing all high scores and achievements.",
			"Closes this menu."

		},
		{
			"Basically, motion sickness simulator. Gotta see it to believe it.",
			"Expands the board to an agonizing 40x20. Why would anyone play this????",
			"The board fades in and out, making it very hard to play.",
			"More coming soon!!",
			"",
			"Closes this menu."
		}
	};

	const int defaults[5][6] = {
		{
		1,
		1,
		0,
		1,
		1,
		0
		},

		{
		1,
		0,
		1,
		1,
		1,
		0
		},

		{
		0,
		0,
		0,
		0,
		0,
		0
		},

		{
		0,
		0,
		0,
		0,
		0,
		0
		},
		{
		0,
		0,
		0,
		0,
		0,
		0
		}

	};

	int activations[5][6] = {
		{
		1,
		1,
		0,
		1,
		1,
		0
		},

		{
		1,
		0,
		1,
		1,
		1,
		0
		},

		{
		0,
		0,
		0,
		0,
		0,
		0
		},

		{
		0,
		0,
		0,
		0,
		0,
		0
		},

		{
		0,
		0,
		0,
		0,
		0,
		0
		}

	};
private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);
	void moveright();
	//void drawCubes(const int position[], int x, int y, int size, int width, std::vector<SDL_Texture*> textures, double angle, double scale, int texture); not sure if i'll need this yet

};