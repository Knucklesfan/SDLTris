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
    std::vector<SDL_Surface*> surfaces = generateSurfaces("./backgrounds/" + path); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    textures = generateTextures(surfaces, renderer);
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

std::vector<SDL_Surface*> bg::generateSurfaces(std::string path) {
    int i = 0;
    std::vector<SDL_Surface*>textures;
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
            return textures;
        }
        textures.push_back(temp);
        printf("Successfully loaded image at %s\n", string.c_str());
    }
    return textures;

}

std::vector<SDL_Texture*> bg::generateTextures(std::vector<SDL_Surface*> surfaces, SDL_Renderer* renderer) {
    std::vector<SDL_Texture*>textures;
    for (SDL_Surface* surf : surfaces) {
        textures.push_back(SDL_CreateTextureFromSurface(renderer, surf));
    }
    return textures;

}
bool bg::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}
