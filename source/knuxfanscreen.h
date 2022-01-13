#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "background.h"
#include "font.h"


class knuxfanscreen {
public:
	std::vector<bg> backgrounds;
	int backnum;
    knuxfanscreen(SDL_Renderer* render, std::vector<SDL_Texture*> texture, std::vector<bg>  backg, Mix_Chunk* soundVec[], int background, font* foont);
	void render();
	void keyPressed(SDL_Keycode key);
	void logic(double deltatime);
	int endlogic();
	void reset();
    double alpha = 1.0;
    bool godown = false;
    bool goup = false;
    bool active = false;
    double lifetime = 0.0;
	int splash = 0;
	font* front;
    Mix_Chunk** sound;
	SDL_Renderer* renderer;
	std::vector<SDL_Texture*> textures;

	std::string splashes[35] = {
	"STILL BETTER THAN LEAF BLOWERS!",
	"GO AHEAD, CRY ABOUT IT",
	"FALLOUT 76, EAT YOUR HEART OUT",
	"TETRIS 99 MORE LIKE",
	"PLAY KEKCROC WORLD 3 MORON",
	"bepis",
	"TRY TO KNOCK ME DOWN AGAIN, I DARE YOU",
	"THIS GAME IS DEDICATED TO SOMEONE, YOU KNOW WHO YOU ARE",
	"LISTEN TO LEMON DEMON",
	"PLAY KEKCROC WORLD 4 WHENEVER THE HECK IT COMES OUT",
	"SUBSCRIBE TO KNUCKLESFAN VIDEOS",
	"FEELS LIKE A T-FLIP ON A THURSDAY",
	"WHO EVEN ARE YOU",
	"STD::STRING SPLASHES BE LIKE:",
	"XML IS OVERRATED",
	"NEON SEGA GENESIS EVANGELION",
	"IM LIKE A LESS EDGY TOBY FOX",
	"PLAY DURANGO SIMULATOR- OH WAIT I CANCELLED THAT",
	"VISIT THE PINECONE SHRINECONE",
	"ADD ME ON VRCHAT, MY NAMES KNUXFAN",
	"TODAYS WORD OF THE DAY: ALIEN",
	"UNDERRATED COMMENT.",
	"THIS GAME HAS LORE",
	"WHO IS THE AUTHOR OF JOURNALS",
	"I WISH FAYE VALENTINE WAS REAL",
	"HELLO MR. THOMPSON",
	"WATCH COWBOY BEBOP",
	"SUS",
	"THE PURE SMOOTH TASTE OF :CPP:",
	"pinecone pinecone pinecone",
	"Tally hall is a good band change my mind",
	"Half life 3 is tetrimino themed",
	"One time I spent so much time debugging this, my body physically gave out on me",
	"At least it's better than Durango Simulator",
	"PLAY FF7 KK",
	};
private:
	void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);

};