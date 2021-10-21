#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "background.h"

class titlescreen
{
public:
	TTF_Font* buttonfont;
	TTF_Font* headerfont;
	TTF_Font* bodyfont;

	Mix_Music** music;
	Mix_Chunk** sound;

	SDL_Renderer* renderer;
	SDL_Window* window;
	std::vector<SDL_Texture*> textures;
	int currentscreen = 0;
	int currentselection = 0;
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
	"SOUND",
	"MUSIC",
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

	std::string messagetitle = "UNFINISHED!";
	std::string messagebody = "I swear it'll be here when I'm finished..";
	std::string messagebutton = "EXIT";
    bg background;

	titlescreen(SDL_Renderer* render, SDL_Window* window, bg backg, std::vector<SDL_Texture*>,  Mix_Music* musicVec[], Mix_Chunk* soundVec[]);
	double layerpos[10];
	void keyPressed(SDL_Keycode key);
	void render();
	void logic(double deltatime);
	int endlogic();
	void reset();
private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
	void renderfont(int x, int y, std::string text, bool selected, TTF_Font* size);
};

