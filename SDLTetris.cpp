#include <SDL.h>
#include <string>
#include <iostream>
#include <filesystem>
#undef main
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

std::vector<SDL_Texture*> generateTextures(std::vector<SDL_Surface*> surfaces, SDL_Renderer* renderer);
std::vector<SDL_Surface*> generateSurfaces(std::string path);

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("SDLTetris", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
    std::vector<SDL_Surface*> surfaces = generateSurfaces("C:\\Users\\bobca\\source\\repos\\SDLTetris\\SDLTetris\\sprites\\"); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    std::vector<SDL_Texture*> textures = generateTextures(surfaces, renderer);
    SDL_Event event;
    SDL_Surface game;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textures.at(0), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        SDL_RenderPresent(renderer);
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
            temp = SDL_LoadBMP(char_array.c_str());
            if (!temp) {
                printf("Failed to load image at %s: %s\n", char_array, SDL_GetError());
                return textures;
            }
            textures.push_back(temp);
            printf("Successfully loaded image at %s\n", char_array.c_str());

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