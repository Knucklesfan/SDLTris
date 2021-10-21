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


bg::bg() {}
bg::bg(std::string path, SDL_Renderer* renderer) {
    std::vector<SDL_Surface*> surfaces = generateSurfaces("./backgrounds/" + path, renderer); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    std::string filepath = "./backgrounds/" + path + "/theme.xml";
    rapidxml::file<> xmlFile(filepath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    name = doc.first_node("name")->value();
    creator = doc.first_node("creator")->value();
    vers = doc.first_node("vers")->value();
    int array[10];
    for(int i = 0; i < 10; i++) {
        std::string sr = "layer";
        sr += std::to_string(i);
        std::cout << sr.c_str() << "\n";
        incrementsx[i] = atoi(doc.first_node(sr.c_str())->value());
        std::cout << "Value: " << incrementsx[i] << "\n";
    }
    std::cout << "Background name: " << name << "\n";

}

std::vector<SDL_Surface*> bg::generateSurfaces(std::string path, SDL_Renderer* renderer) {
    int i = 0;
    std::vector<SDL_Surface*>surfaces;
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
            return surfaces;
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
void bg::render(SDL_Renderer* renderer) {
    for(int i = 0; i < layers; i++) {
    drawTexture(renderer,textures[i], fmod(layerposx[0],640), fmod(layerposx[0],480), 0.0, 1.0, false);
    drawTexture(renderer,textures[i], fmod(layerposx[0], 640)+640, fmod(layerposx[0],480)+480, 0.0, 1.0, false);
    drawTexture(renderer,textures[i], fmod(layerposx[0],640)+0, fmod(layerposx[0],480)+480, 0.0, 1.0, false);
    drawTexture(renderer,textures[i], fmod(layerposx[0], 640)+640, fmod(layerposx[0],480), 0.0, 1.0, false);
    }
}
void bg::logic(double deltatime)
{
    for(int i = 0; i < layers; i++) {
        std::cout << incrementsx[i] << "\n";
        std::cout << incrementsy[i] << "\n";
        if(incrementsx > 0) {
            layerposx[i] -= (deltatime)/(incrementsx[i]);
        }
        if(incrementsy > 0) {
            layerposy[i] -= (deltatime)/(incrementsy[i]);
        }

    }
}

void bg::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
    SDL_Rect sprite;
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
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
    SDL_RenderCopyEx(renderer, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}

bool bg::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}
