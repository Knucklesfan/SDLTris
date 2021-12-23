#include "font.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <map>
#include <utility>      // std::pair, std::make_pair
#include <math.h>


char alphabet[] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z'
};
char numbers[] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
};
char symbols[] = {
    '.',
    ',',
    '*',
    '_',
    '-',
    '!',
    '$',
    '%',
    '=',
    ':',
    '\''
};

#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif


font::font() {

}

//this is um, also copied from background.h and modified lmao
//I sWeAr I'm A gOoD cOdEr gUyS

font::font(std::string path, SDL_Renderer* renderer) {
    std::string p = "fonts/" + path;
    generateSurfaces(p, renderer); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!

    std::string pth = filepath "fonts/" + path + "/fontdef.xml";

    rapidxml::file<> xmlFile(pth.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    name = doc.first_node("name")->value();
    numberpos = atoi(doc.first_node("numberpos")->value());
    alphabetpos = atoi(doc.first_node("alphabetpos")->value());
    specialpos = atoi(doc.first_node("specialpos")->value());
    width = atoi(doc.first_node("charx")->value());
    height = atoi(doc.first_node("chary")->value());
    size = atoi(doc.first_node("chary")->value());

    for(int i = 0; i < 26; i++) {
        mapping[alphabet[i]] = (alphabetpos+i)*width;

    }
    for(int i = 0; i < 10; i++) {
        mapping[numbers[i]] = (numberpos+i)*width;
        
    }
    for(int i = 0; i < 10; i++) {
        mapping[symbols[i]] = (specialpos+i)*width;
        
    }



    //DEBUG ONLY STUFF, YKNOW HOW IT IS
    // thanks to geeks4geeks for being a great code source, i might or might not have copied this to save time but whatever it's literally just a debug function so who cares
    
    
    std::map<char, int>::iterator itr;
    std::cout << "\nThe map is : \n";
    std::cout << "\tKEY\tELEMENT\n";
    for (itr = mapping.begin(); itr != mapping.end(); ++itr) {
        std::cout << '\t' << itr->first
             << '\t' << itr->second << '\n';
    }
    std::cout << "\n";
    
    doc.clear();
}
void font::render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer) {
    render(renderer, strg, x, y, center, 0, 0, 0);
}
void font::render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer, bool sine, double pos, double multiplyin, double multiplyout) {
    render(renderer, strg, x, y, center, 0, 0, 0, 0, sine, pos, multiplyin, multiplyout);
}

void font::render(int x, int y, std::string strg, bool center, SDL_Renderer* renderer, int wordwrap) {
    render(renderer, strg, x, y, center, 0, 0, 0, wordwrap);
}
void font::render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green) {
    render(renderer, words, x, y, center, red, blue, green, 0);
}
void font::render(SDL_Renderer* renderer, std::string strg, int x, int y, bool center) {
    render(renderer, strg, x, y, center, 0, 0, 0);
}
void font::render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap) {
    render(renderer, words, x, y, center, red, blue, green, wordwrap, false, 0, 0, 0);
}
void font::render(SDL_Renderer* renderer, std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout) {
    int finalwidth = 0;
    if(center) {
        if (wordwrap > 0 && words.length()*width > wordwrap) {
            finalwidth = wordwrap;
        }
        else {
            finalwidth = words.length() * width;
        }
    }
    if(red > 0 || blue > 0 || green > 0) {
        SDL_SetTextureColorMod( texture, red, blue, green );

    }
    std::transform(words.begin(), words.end(),words.begin(), ::toupper);
    if(wordwrap > 0) {
        words = wrap(words, wordwrap/width);
    }

    double tmpy = y;
    int tmpx = x-finalwidth/2;
    int i = 0;
    for(char& c : words) {
        if (mapping.find(c) != mapping.end()) {
            if (sine) {
                tmpy = y + (sin((pos + i) * multiplyin) * multiplyout);
            }
            drawTexture(renderer, texture, tmpx, tmpy, 0, 1.0, false, mapping.at(c), 0, width, height);
            tmpx += width;
        }
        else {
            //std::cout << "LOADED BAD CHAR!!\n";
            if (c == '\n') {
                tmpy += height;
                tmpx = x - finalwidth / 2;
                std::string everythingelse = words.substr(i);
            }
            else {
                tmpx += width;
            }
        }
        i++;
    }
    SDL_SetTextureColorMod(texture, 255,255,255);

}
//uses the modern drawtexture from background.h, hopefully there's no bugs?

//had to add a few more parameters becauuuuuse rendering sprite sheets is our new intention with drawTexture

void font::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
    SDL_Rect sprite;
    SDL_Rect srcrect = {srcx, srcy, srcw, srch};
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    sprite.w = srcw * scale;
    sprite.h = srch * scale;
    if (center) {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else {
        sprite.x = x + srcw / 2 - sprite.w / 2;
        sprite.y = y + srch / 2 - sprite.h / 2;
    }
    SDL_RenderCopy(renderer, texture, &srcrect, &sprite);
}

bool font::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}


//ALSO STOLEN FROM BACKGROUND.H LMAOOOO

//i still dont understand how this works, and that's terrifying because i entirely wrote this myself

//update: stealing from background.h is morally incorrect so i decided instead to recode this since fonts can literally only have one texture

//ADD MORE TEXTURES TO A FONT AND I'LL BREAK YOU LEGS


//now that i mention it, adding more than one texture to a font could be cool? but i'm not really designing an rpg text box, am i?

//...or am i?


void  font::generateSurfaces(std::string path, SDL_Renderer* renderer) {
    std::string tmppth = path + "/font.bmp";
    SDL_Surface* tmpsurf = SDL_LoadBMP(tmppth.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, tmpsurf);
    SDL_FreeSurface(tmpsurf);
}

// i seriously considered this a rewrite lmao ^^^



//THE FOLLOWING IS ESPECIALLY THANKS TO ICR ON STACKOVERFLOW
//thank you so much dude for the explaination..
//translated from C# to C++ by me tho
//great read: https://stackoverflow.com/questions/17586/best-word-wrap-algorithm

std::string font::wrap(std::string str, int pixels) {
    
    std::vector<std::string> words = seperateWords(str);
    int currentline = 0;
    std::string output = "";
    
    for(int i = 0; i < words.size(); i++) {
        std::string word = words.at(i);
        if(currentline + word.length() > pixels) {
            if(currentline > 0) {
                output += "\n";
                currentline = 0;
            }
            /*while(word.length() > pixels) {
                output += word.substr(0, pixels-1) + ".";
                word = word.substr(pixels-1);
                output += "\n";
            }
            word = word.substr(word.find_first_of(' ')+1);
            */
        }
        output += word;
        currentline += word.length();
    }
    
    return output;
}
std::vector<std::string> font::seperateWords(std::string string) {
    std::vector<std::string> parts;
    int startindex = 0;
    while(true) {
        int index = string.find_first_of(' ', startindex);
        if(index == -1) {
            parts.push_back(string.substr(startindex));
            return parts;
        }

        std::string word = string.substr(startindex,index-startindex);
        char nextchar = string.substr(index,1).at(0);
        if(nextchar = ' ') {

            parts.push_back(word);
            parts.push_back(std::string(1, nextchar));
        }
        startindex = index + 1;
    }
}