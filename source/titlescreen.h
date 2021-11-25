#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "background.h"
#include "highscore.h"
#include "font.h"

class titlescreen
{
public:
	font* buttonfont;
	font* headerfont;
	font* bodyfont;
	font* versfont;

	Mix_Music** music;
	Mix_Chunk** sound;
	double time = 0;
	int currentsetting = 0;
	SDL_Renderer* renderer;
	SDL_Window* window;
	std::vector<SDL_Texture*> textures;
	int currentscreen = 0;
	int currentselection = 0;
	int bgnum = 0;
	const int selections = 5;
	std::string options[5] = {
		"START GAME",
		"THEMES MANAGER",
		"Options",
		"CREDITS",
		"EXIT"
	};
	bool showerror = false;
	bool loadgame = false;
	const int settingssize = 4;
	std::string settingstitle = "OPTIONS";
	std::string settings[4] = {
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

	titlescreen(SDL_Renderer* render, SDL_Window* windows, std::vector<bg>  backg, std::vector<SDL_Texture*> texture, Mix_Music* musicVec[], Mix_Chunk* soundVec[], int backgr, std::vector<font*> fonts);
	double layerpos[10];
	void keyPressed(SDL_Keycode key);
	void render(highscore* score);
	void logic(double deltatime);
	int endlogic();
	void reset();
private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);
	void debuginput(SDL_Keycode key);
};

