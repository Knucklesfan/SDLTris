#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "../gamemode.h"
#include <array>
#include "../sine.h"
#include <math.h>
#include "../background.h"
#include "../wireframecube.h"
#include "../opengl/skybox.h"
#include "../opengl/plane.h"

class credits : public Gamemode
{
    public:
    credits();
    sine* sineWave;
    plane* p;
    void render();
    void logic(double deltaTime);
    void input(SDL_Keycode);
    Transition endLogic();
    void reset();
    // Font* headerfont;
    // Font* textfont;
    bg* backg;
    skybox* sky;
    wireframecube* cub;
    buffermanager* buff;
    bool loadgame;
    double time = 0.0;
    double speed = 25;
    double posx = 0;
    double posy = 0;
    // std::vector<SDL_Texture*>* textures;
    std::string words[26] = {
        "KNUXFANS TETRIMINOS",
        "CREATED BY",
        "KNUXFAN AKA KNUCKLESFAN",
        "CODE",
        "KNUXFAN",
        "ART",
        "KNUXFAN",
        "MUSIC PROVIDED BY THE FOLLOWING",
        "arachno:coolphat",
        "DROZERiX",
        "Zalza of Tequila",
        "Mikael Eriksson",
        "BabyDee:eFFect",
        "l.restifo@pcg.it",
        "kris0520",
        "Marco Schindler",
        "Apollon",
        "All music obtained from ModArchive",
        "Special thanks",
        "Special thanks to the RetroNX Discord server for providing a ton of code help, especially with the switch port, and just being great guys!",
        "Everyone on the Kekcroc Team discord server for encouraging me and giving feedback",
        "Super big thanks to everyone on the international society of gamers for um. well, you guys know what you did.",
        "And thank you to everyone else who helped me along this way!",
        "AND ONE MORE FINAL",
        "Thank YOU the player",
        "for playing my game!"
    };

    int wordprops[26] = {
        2, //KNUXFANS TETRIMINOS
        1, //CREATED BY
        0, //KNUXFAN
        1, //CODE
        0, //KNUXFAN
        1, //ART
        0, //KNUXFAN
        1, //MUSIC
        0, //COOLPHAT
        0, //DOZERIX
        0, //ZALZA
        0, //MIKAEL
        0, //BAYDEE
        0, //L.RESTIFO
        0, //KRIS0520
        0, //MARCO
        0, //APOLLON
        1, //ALL MUSIC OBTAINED FROM MODARCHIVE.ORG
        1, //SPECIALTHANKS
        0, //THANKS RETRONX
        0, //KEKCROC TEAM THANKS
        0, //PINECONE THANKS
        0, //SCREW YOU TRASH WOMAN
        2, //AND ONE MORE FINAL
        0, //STERIOTYPICAL THANKS TO THE PLAYER
        0, //CONTINUED
    };

    int wordspacing[26] = {
        128,
        32,
        64,
        32,
        64,
        32,
        64,
        32,
        32,
        32,
        32,
        32,
        32,
        32,
        32,
        32,
        32,
        64,
        64,
        64,
        64,
        64,
        256,
        128,
        32
    };
    void drawRotatedBlock(int x, int y, const int position[], double angle, int);
    private:
    	const int tpiece[6] = {
		 0,2,0,
		 2,2,2
        };
        const int lpiece[6] = {
			 2,2,2,
			 2,0,0
		};
        double wordsy = 512;
        double alpha = 0.0;
        bool goup = false;
        bool godown = false;
        int gamemode = 0;
        void drawCubes(const int position[], int x, int y, int size, int width, double angle, double scale, int texture);
};