#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "../background.h"
#include "../highscore.h"
#include "../font.h"
#include "../gamemode.h"

class titlescreen : public Gamemode
{
public:
	Font* buttonfont;
	Font* headerfont;
	Font* bodyfont;
	Font* versfont;

	double time = 0;
	int currentsetting = 0;
	int currentscreen = 0;
	int currentselection = 0;
	int bgnum = 0;
	const int selections = 4;
	std::string settings[4] = {
		"START GAME",
		"Options",
		"CREDITS",
		"EXIT"
	};
	bool showerror = false;
	bool loadgame = false;
	bool loadmenu = false;
	bool loadcreds = false;
	const int settingssize = 4;
	std::string settingstitle = "OPTIONS";
	std::string sets[4] = {
	"FULL SCREEN",
	"MUSIC",
	"SOUND",
	"EXIT"
	};
	const int creditssize = 5;
	std::string creditstitle = "CREDITS";
	std::string credits[5] = {
	"Brought to you by:",
	"Programming, Art, basically everything:",
	"Knuxfan (AKA Knucklesfan)",
	"Special Thanks to:",
	"nobody lol",
	};
	bool big[5] = {0,0,1,0,1};
	std::string messagetitle = "UNFINISHED!";
	std::string messagebody = "I swear it'll be here when I'm finished..";
	std::string messagebutton = "EXIT";
    std::vector<bg> background;

	titlescreen();
	double layerpos[10];
	void input(SDL_Keycode key);
	void render();
	void logic(double deltatime);
	Transition endLogic();
	void reset();
private:
	void debuginput(SDL_Keycode key);
};

