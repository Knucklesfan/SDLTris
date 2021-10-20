#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "tetriminos.h"
#undef main
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
std::vector<SDL_Texture*> generateTextures(std::vector<SDL_Surface*> surfaces, SDL_Renderer* renderer);
std::vector<SDL_Surface*> generateSurfaces(std::string path);
void drawCubes(int position[], double angle[], double scale[], int x, int y,int size, int width, std::vector<SDL_Texture*> textures, SDL_Renderer* renderer);
void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale);
void shiftarray(int (array)[],int size, int shift);
void checkLines(int (blocks)[200]);
bool checkRow(int (blocks)[10]);
void clearRow(int (blocks)[200], int y);

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
    SDL_RenderSetLogicalSize(renderer, 640, 480);
    int testblocks[200];
    std::fill_n(testblocks, 200, 0);
    double testangles[200];
    std::fill_n(testangles, 200, 0);
    double testscale[200];
    std::fill_n(testscale, 200, 1);

    std::vector<SDL_Surface*> surfaces = generateSurfaces("./sprites/"); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    std::vector<SDL_Texture*> textures = generateTextures(surfaces, renderer);
    SDL_Event event;
    SDL_Surface game;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double angle = 0;
    tetrimino t(0,0, testblocks,10,20);
    double ticks = 0;
    int realtick = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_UP) {
                    //shiftarray(testblocks, 200,10);
                }
                if(event.key.keysym.sym == SDLK_DOWN) {
                    t.movedown();

                    //shiftarray(testblocks, 200,-10);
                }
                if(event.key.keysym.sym == SDLK_LEFT) {
                    t.moveleft();
                }
                if(event.key.keysym.sym == SDLK_RIGHT) {
                    t.moveright();

                }
                if(event.key.keysym.sym == SDLK_z) {
                    t.rotate();
                }
                if(event.key.keysym.sym == SDLK_x) {
                    std::fill_n(testscale, 200, testscale[0]+0.1);
                }

            }
        }
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();

        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        if(realtick%100 == 0) {
            t.movedown();
        }
        ticks+= deltaTime;
        if(ticks >= 1) {
            ticks = 0;
            realtick++;
        }
        //std::fill_n(testangles, 200, testangles[0]+deltaTime*0.1);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textures.at(0), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        t.draw();
        std::memcpy(testblocks, testblocks, sizeof t.array);
        drawCubes(testblocks,testangles, testscale, 240,80,200,10,textures,renderer);
        SDL_RenderPresent(renderer);
        if(!t.alive) {
            checkLines(testblocks);
            t.rebirth(0,0);
        }

    }

    return 0;
}
void shiftarray(int (array)[],int size, int shift) {
    for(int i = 0; i < abs(shift); i++) {
        int newarray[size];
        if(shift < 0) { //negative
            for(int i = 0; i < size-1; i++) {
                newarray[i+1] = array[i];
            }
            newarray[0] = array[size-1];
        }
        else { //positive
            for(int i = 1; i < size; i++) {
                newarray[i-1] = array[i];
            }
            newarray[size-1] = array[0];
        }
        std::memcpy(array,newarray, sizeof newarray);
    }
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

void drawCubes(int position[], double angles[], double scale[], int x, int y, int size, int width, std::vector<SDL_Texture*> textures, SDL_Renderer* renderer) {
    for(int i = 0; i < size; i++ ){
        if(position[i] > 0) {
        drawTexture(renderer,textures.at(position[i]), x+(i%width)*16, y+(i/width)*16,angles[i], scale[i]);
        }
    }
}

void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale) {
    SDL_Rect sprite;
    SDL_QueryTexture(texture, NULL,NULL, &sprite.w, &sprite.h);
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w*scale;
    sprite.h = sprite.h*scale;

    sprite.x = x + oldwidth/2 - sprite.w/2;
    sprite.y = y + oldheight/2 - sprite.h/2;
    SDL_RenderCopyEx(renderer, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}

void checkLines(int (blocks)[200]) {
    for(int i = 0; i < 20; i++) {
        int temp[10];
        for(int j = 0; j < 10; j++) {
            temp[j] = blocks[i*10+j];
        }
        if(checkRow(temp)) {
            printf("FULL ROW DETECTED. %i",i);
            clearRow(blocks, i);
        }
    }
}
bool checkRow(int (blocks)[10]) {
    for(int i = 0; i < 10; i++) {
        if(blocks[i] == 0) {
            return false;
        }
    }
    return true;
}

void clearRow(int (blocks)[200], int y) {
            int newarray[200];
            std::fill_n(newarray, 200, 0);
            for(int j = 0; j < 10; j++) {
                blocks[(y*10)+j]=0;
            }
            for(int j = 0; j < (y*10); j++) {
                newarray[j+10] = blocks[j];
            }
            for(int j = (y*10)+10; j < 200; j++) {
                newarray[j] = blocks[j];
            }
            memcpy(blocks,newarray, sizeof newarray);
            //shiftarray(blocks, 200, -10);

}