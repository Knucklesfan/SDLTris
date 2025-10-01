#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "../opengl/background.h"
#include "../opengl/font.h"
#include "../engine/gamemode.h"


class knuxfanscreen : public Gamemode {
public:
	int backnum;
	knuxfanscreen();
	void render();
	void input(SDL_Keycode key);
	void logic(double deltatime);
	Transition endLogic();
	void reset();
	double alpha = 1.0;
	bool godown = false;
	bool goup = false;
	bool active = false;
	double lifetime = 0.0;
	int startTime = 0;

	double knuxfanx = 76;
	double knuxfany = 0.0;

	double presentsx = 306;
	double presentsy = 480.0;

	double yvelocity = -1.0;

	bool explode = false;
	double rotate = 0;
	int splash = 0;

	const std::string splashes[35] = {
	"STILL BETTER THAN LEAF BLOWERS!",
	"GO AHEAD, CRY ABOUT IT",
	"FALLOUT 76, EAT YOUR HEART OUT",
	"TETRIS 99 MORE LIKE",
	"PLAY KEKCROC WORLD 3 MORON",
	"LISTEN TO LEMON DEMON",
	"PLAY KEKCROC WORLD 4 WHENEVER THE HECK IT COMES OUT",
	"FEELS LIKE A T-FLIP ON A THURSDAY",
	"WHO EVEN ARE YOU",
	"STD::STRING SPLASHES BE LIKE:",
	"XML IS OVERRATED",
	"IM LIKE A LESS EDGY TOBY FOX",
	"VISIT THE PINECONE SHRINECONE",
	"THIS GAME HAS LORE",
	"HELLO MR. THOMPSON",
	"SUS",
	"THE PURE SMOOTH TASTE OF MAKEFILE",
	"pinecone pinecone pinecone",
	"Tally hall is a good band change my mind",
	"Half life 3 is tetrimino themed",
	"One time I spent so much time debugging this, my body physically gave out on me",
	"At least it's better than Durango Simulator",

	};
private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);

};