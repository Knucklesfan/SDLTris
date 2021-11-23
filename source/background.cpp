#include "background.h"
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>    // std::sort
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <iostream>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <cmath>
#include <SDL2/SDL_mixer.h>
bg::bg() {}
bg::bg(std::string path, bool folder, SDL_Renderer* renderer) {
    std::string p = "./backgrounds/" + path;
    if(folder) {
        p = path;
    }
    generateSurfaces(p, renderer); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    for (int i = 0; i < layers; i++) {
        SDL_Rect sprite;
        SDL_QueryTexture(textures[i], NULL, NULL, &sprite.w, &sprite.h);
        if (sprite.w > maxwidth) {
            maxwidth = sprite.w;
        }
        if (sprite.h > maxheight) {
            maxheight = sprite.h;
        }

    }
    std::cout << "max=" << maxwidth <<"x"<< maxheight << "\n"; 
    std::string filepath = "./backgrounds/" + path + "/theme.xml";
    if(folder) {
        filepath = path + "/theme.xml";
    }

    rapidxml::file<> xmlFile(filepath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    name = doc.first_node("name")->value();
    creator = doc.first_node("creator")->value();
    vers = doc.first_node("vers")->value();
    songname = doc.first_node("musicname")->value();
    artist = doc.first_node("musicartist")->value();
    rotation = 0;

    if (doc.first_node("rotation") != NULL) {
        rotation = atoi(doc.first_node("rotation")->value());
    }

    if (doc.first_node("fglayer") != NULL) {
        std::cout << "fglayer detected\n";
        fglayer = atoi(doc.first_node("fglayer")->value());
    }

    int array[10];
    for(int i = 0; i < layers; i++) {

        std::string sr = "layer";
        sr += std::to_string(i);
        incrementsx[i] = atoi(doc.first_node(sr.c_str())->value());
        std::string sy = "layer";
        sy += std::to_string(i);
        sy += "y";
        incrementsy[i] = atoi(doc.first_node(sy.c_str())->value());
        //std::cout << "INFORMATION!!!: " << atoi(doc.first_node(sy.c_str())->value()) << "\n";
    }

    std::string muspath = "./backgrounds/" + path + "/";
    if(folder) {
        muspath = path + "/";
    }

    muspath += doc.first_node("music")->value();
    music = Mix_LoadMUS(muspath.c_str());
    if (!music) {
        printf("Failed to load music at %s: %s\n", muspath, SDL_GetError());
    }

    //std::cout << "Background name: " << name << "\n";
    doc.clear();
}

void  bg::generateSurfaces(std::string path, SDL_Renderer* renderer) {
    int i = 0;
    std::vector<SDL_Surface*> surfaces;
    SDL_Surface* temp;
    std::vector<std::string> strings;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (i < 64) {
            std::string char_array{entry.path().u8string()}; //i dunno if its because im writing this at 10:55 and im passing out or what, but this code was IMPOSSIBLE to write.
            if (hasEnding(char_array, ".bmp")) {
                strings.push_back(char_array);
            }
        }
        
        else { break; }
    }
    std::sort(strings.begin(),strings.end(),compareFunction);//sort the vector
    for(auto &string : strings) {
        temp = SDL_LoadBMP(string.c_str());
        if (!temp) {
            printf("Failed to load image at %s: %s\n", string, SDL_GetError());
        }
        surfaces.push_back(temp);
        printf("Successfully loaded image at %s\n", string.c_str());
    }
    for (SDL_Surface* surf : surfaces) {
        SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, surf);
        if(temp != NULL) {
            textures.push_back(temp);
            printf("pushed texture!!");
            layers++;
        }
        else {
            fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        }
    }

}
void bg::render(SDL_Renderer* renderer, bool layer) {
    //OKAY EXPLAINATION FOR MY ACTIONS:
    //dear whoever is reading this:
    //this code is a bad practice.

    int addition = 0;
    int max = layers;
    if (fglayer > 0) {
        max = fglayer;
        if (layer) {
            max = layers;
            addition = fglayer;
        }
    }
    else if (layer && fglayer == 0) {
        return;
    }

    for (int i = addition; i < max; i++) {
        int width, height;
        SDL_QueryTexture(textures[i], NULL, NULL, &width, &height);
        double tempx = 0;
        double tempy = 0; //yuck
        int multiplerx = 1; //this is really bad practice but it's currently 11pm and i wanna feel accomplished
        int multiplery = 1;
        //std::cout << incrementsx[i] << i << "\n";
        if (incrementsx[i] != 0) {
            tempx = fmod(layerposx[i], width); //ew
        }
        if (incrementsy[i] != 0) {
            tempy = fmod(layerposy[i], height); //GROSS CODE
        }
        if (layerposx[i] > 0) {
            multiplerx = -1;
        }
        if (layerposy[i] > 0) {
            multiplery = -1;
        }

        drawTexture(renderer, textures[i], tempx, tempy, fmod(angle, 360), 1.0, false);
        drawTexture(renderer, textures[i], tempx + (width * multiplerx), tempy + (height * multiplery), fmod(angle, 360), 1.0, false);
        drawTexture(renderer, textures[i], tempx + 0, tempy + (height * multiplery), fmod(angle, 360), 1.0, false);
        drawTexture(renderer, textures[i], tempx + (width * multiplerx), tempy, fmod(angle, 360), 1.0, false);
        }
}
void bg::logic(double deltatime)
{
    if (angle > 360.0) {
        angle = 0.0;
    }
    if (rotation != 0) {
        angle += deltatime / rotation;
    }
    //std::cout << angle << "\n";

    for(int i = 0; i < layers; i++) {
        if(incrementsx[i] != 0) {
            layerposx[i] -= (deltatime)/(incrementsx[i]);
        }
        if(incrementsy != 0) {
            layerposy[i] -= (deltatime)/(incrementsy[i]);
        }

    }
}

void bg::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
    SDL_Rect sprite;
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;
    if (center) {
        sprite.x = x - oldwidth / 2;
        sprite.y = y - oldheight / 2;
    }
    else {
        sprite.x = x + oldwidth / 2 - sprite.w / 2;
        sprite.y = y + oldheight / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(renderer, texture, NULL, &sprite, 0, NULL, SDL_FLIP_NONE);
}

bool bg::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}
