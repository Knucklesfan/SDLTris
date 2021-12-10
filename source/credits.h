#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include "font.h"
#include <array>
#include "sine.h"
#include <math.h>
class credits 
{
    public:
    credits(SDL_Renderer* renderer, std::vector<font*> fonts);
    sine* sineWave;
    void render(SDL_Renderer* renderer);
    void logic(double deltaTime);
    void keyPress(SDL_KeyCode);
    font* headerfont;
    font* textfont;

    std::string words[26] = {
        "KNUXFANS TETRIMINOS",
        "CREATED BY",
        "KNUXFAN AKA KNUCKLESFAN",
        "CODE",
        "KNUXFAN",
        "ART",
        "KNUXFAN, KRIS0520",
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
        "Huge thanks to puddle, without your help I wouldn't have made this game",
        "AND ONE MORE FINAL",
        "Thank YOU the player",
        "for playing my game!"
    };

    int wordprops[26] = {
        1, //KNUXFANS TETRIMINOS
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
        0, //AND ONE MORE FINAL
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
        32,
        32
    };
    double wordsy = 512;

    private:
};