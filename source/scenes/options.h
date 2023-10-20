#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "game.h"
#include "../font.h"
#include "../cube.h"
#include "../background.h"
#include "../utils/defs.h"
#include "../gamemode.h"
#define FONT_WIDTH 16
#define FONT_GAP (640/FONT_WIDTH)
class options: public Gamemode {
public:
	bool loadgame = false;
	bg background;
	cube* cub;
	cube* bigcub;
	Font* newft;
	Font* newhead;
	Font* tickertext;
	double txtpos = 0;
	double bottompos = 0;
	int settingsx = 320;
	bool mvleft = false;
	bool mvright = false;
	int currenttitle = 0;
	int currentselection = 0;
	int currentscreen = 0;
	int currentmenuselect = 0;
	std::string bottomtck = "GREEZ TO THE FOLLOWING PEOPLE: KK, DURANGO, JOHNNY, BOOMBOOM, FRISBEE, BLAKE, CASPER, DARK PRINCE, KRIS, CONNOR, ELENA, QUOTES, ERIKA, PETERS, KARMEN, ISAIAH, JALEN, EMRETECH, GENERIC, BOTTMINT, M4XW, BEHEMOTH, NATINSULA, TOTALJUSTICE, MIRZAGHALIB, AND EVERYONE ON SMWCENTRAL AND EVERYWHERE ELSE WHO'S HELPED ME THIS FAR.     ";
	std::string tcktxt = " BACK FROM THE DEAD RETURNS A TETRIMINO BASED GAME UNLIKE ANY OTHER. BROUGHT TO YOU BY KNUXFAN, WORLD UNKNOWN DEVELOPER KNOWN FOR SUCH CLASSIC HITS AS CHICKEN SHOOT 3 AND AMONG THEM COMES THE LATEST IN TETRIMINO ACTION. AN AMIGA GAME FROM AN ALTERNATE TIMELINE, BROUGHT TO LIFE WITH CPP, MODERN PROCESSORS AND A WHOLE LOT OF EFFORT COMES A GAME UNLIKE ANY OTHER.";
	SDL_Texture* rendertext;
	unsigned int time = 0;
	std::vector<SDL_Texture*> texture;
	double rot = 0.0;
	options();
	void input(SDL_Keycode key);
	void render();
	void logic(double deltatime);
	Transition endLogic();
	void reset();
	std::string titles[5] = {
		"GAMEPLAY SETTINGS",
		"VISUAL SETTINGS",
		"SYSTEM SETTINGS",
		"EXTRA SETTINGS",
		"DEBUG SETTINGS"
	};
	const int maxes[5] = {
		8,
		6,
		4,
		8,
		1,
	};
	std::string opts[5][12] = {
		{
		"Ghost Piece",
		"Hold Piece",
		"Block Speedup",
		"Fast Drop",
		"Scoring System",
		"Repeat Holding",
		"Hold Scoring",
		"Level Length",
		"Exit Menu",
		"",
		"",
		""
		},
		{
		"Background mode",
		"First Background",
		"Line Clear animation",
		"Moving Backgrounds",
		"Near Top Flash",
		"@2Low Performance Mode",
		"Exit Menu",
		"",
		"",
		"",
		"",
		""
		},
		{
		"Full Screen",
		"Music Volume",
		"Sound Volume",
		"Reset Settings",
		"Exit Menu",
		},
		{
		"Rotating Board",
		"Bigger Board??",
		"Blind Mode",
		"Anti-Gravity",
		"Bomb Block",
		"Mystery Block!!",
		"Mirror Mode",
		"Acid Bath",
		"Exit Menu",
		},
		{
			"Debug",
			"Exit Menu"
		}
	};
	std::string details[5][12] = {
		{
			"Shows a ghost piece near where your block will land",
			"Allows you to hold a block in a container on the side of the screen.",
			"Enables faster blocks as levels increase. Disable for a more casual experience.",
			"Allows dropping blocks faster with the UP key. Recommended to use alongside Ghost Piece.",
			"Select which scoring system to use.",
			"Allows you to hold multiple times in the same turn without dropping a block.",
			"Should score be revoked if a hold is used?",
			"Allows you to customize level length before changing backgrounds",
			"Closes this menu."

		},
		{
			"Enable to cycle through backgrounds as levels increase, Disable to stay on the first background selected.",
			"Selects the first background to start each game with.",
			"Show an animation when clearing a line.",
			"Enables background motion.",
			"Flashes the side of the screen when blocks are near the top.",
			"Disables several graphical effects and uses an older renderer on new versions",
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
			"Blocks fall upside down! Makes things a bit more trickier...",
			"Random chance to get a black, explosive block that destroys the blocks around it!",
			"Covers your block until it hits the ground to make it more mysterious which block you got",
			"Mirrors the entire playfield, including controls. A little trippier when combined with other effects.",
			"ACID BATH! Randomly, acid may drop from an edge and destroy everything in it's path, including your block. Watch out!"
			"Closes this menu."
		}
	};

private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);
	void moveright();
	//void drawCubes(const int position[], int x, int y, int size, int width, std::vector<SDL_Texture*> textures, double angle, double scale, int texture); not sure if i'll need this yet

};