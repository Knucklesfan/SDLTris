#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "game.h"
#include "titlescreen.h"

#undef main
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
std::vector<SDL_Texture*> generateTextures(std::vector<SDL_Surface*> surfaces, SDL_Renderer* renderer);
std::vector<SDL_Surface*> generateSurfaces(std::string path);
bool hasEnding(std::string const& fullString, std::string const& ending);
int main() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDLTetris", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

        SDL_Quit();

        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(renderer, 640, 480);

    std::vector<SDL_Surface*> surfaces = generateSurfaces("./sprites/"); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    std::vector<SDL_Texture*> textures = generateTextures(surfaces, renderer);
    SDL_Event event;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double ticks = 0;
    int realtick = 0;
    int gamemode = 0;
    titlescreen title(renderer, window, textures);
    game gamer(renderer, window, textures);
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (gamemode == 0) {
                    title.keyPressed(event.key.keysym.sym);
                }
                else {
                    gamer.keyPressed(event.key.keysym.sym);
                }
            }
        }
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        if (gamemode == 0) {
            printf("titlescreen");
            title.logic(deltaTime);
            title.render();
            if (title.endlogic() == 1) {
                //gamer = game(renderer, window, textures);
                gamemode = 1;
                title.loadgame = false;
            }
        }
        else {
            printf("gaming");
            gamer.logic(deltaTime);
            gamer.render();
            if (gamer.endlogic() == 1 || !gamer.gameactive) {
                gamemode = 0;
                gamer.reset();
            }

        }
    }

    return 0;
}
std::vector<SDL_Surface*> generateSurfaces(std::string path) {
    int i = 0;
    std::vector<SDL_Surface*>textures;
    SDL_Surface* temp;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (i < 64) {
            std::string char_array{entry.path().u8string()}; //i dunno if its because im writing this at 10:55 and im passing out or what, but this code was IMPOSSIBLE to write.
            if (hasEnding(char_array, ".bmp")) {
                temp = SDL_LoadBMP(char_array.c_str());
                if (!temp) {
                    printf("Failed to load image at %s: %s\n", char_array, SDL_GetError());
                    return textures;
                }
                textures.push_back(temp);
                printf("Successfully loaded image at %s\n", char_array.c_str());
            }
        }
        else { break; }
    }
    return textures;

}

std::vector<SDL_Texture*> generateTextures(std::vector<SDL_Surface*> surfaces, SDL_Renderer* renderer) {
    std::vector<SDL_Texture*>textures;
    for (SDL_Surface* surf : surfaces) {
        textures.push_back(SDL_CreateTextureFromSurface(renderer, surf));
    }
    return textures;

}

bool hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}
