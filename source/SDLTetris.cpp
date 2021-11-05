#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <enet/enet.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include "game.h"
#include "titlescreen.h"
#include "background.h"
#include "knuxfanscreen.h"
#include "server.h"

#undef main
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#ifdef __SWITCH__
    #define filepath  "/"
#else
    #define filepath  "./"
#endif

#define JOY_A     0
#define JOY_B     1
#define JOY_X     2
#define JOY_Y     3
#define JOY_PLUS  6
#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15


std::vector<Mix_Music*> generateMusic(std::string path);
std::vector<Mix_Chunk*> generateSounds(std::string path);
std::vector<SDL_Texture*> generateTextures(std::vector<SDL_Surface*> surfaces, SDL_Renderer* renderer);
std::vector<SDL_Surface*> generateSurfaces(std::string path);
bool hasEnding(std::string const& fullString, std::string const& ending);
bool compareFunction (std::string a, std::string b) {return a<b;} 
bool bgCompare (bg a, bg b) {return a.name<b.name;} 
int input(int gamemode, titlescreen* title, game* gamer, SDL_Keycode keycode);

int main() {
        srand((unsigned)time(0));

        if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) != 0) {
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
    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();

        return 1;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    SDL_Joystick* joystick;

    SDL_Joystick* gGameController = SDL_JoystickOpen(0);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(renderer, 640, 480);
    TTF_Init();
    std::string prefix = filepath;
    std::vector<SDL_Surface*> surfaces = generateSurfaces(prefix + "sprites/"); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    std::vector<SDL_Texture*> textures = generateTextures(surfaces, renderer);
    std::vector<Mix_Music*> music = generateMusic(prefix + "music/");
    std::vector<Mix_Chunk*> sound = generateSounds(prefix + "sound/");
    std::vector<bg> backgrounds;
    SDL_Event event;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double ticks = 0;
    int realtick = 0;
    int gamemode = 0;
    
    for(auto& p : std::filesystem::recursive_directory_iterator(prefix + "backgrounds/")) {
        if (p.is_directory()) {
            //std::cout << "HELP ME:" << p.path().filename() << "\n";
            bg backg(p.path().filename().u8string() , renderer);
            backgrounds.push_back(backg);

        }
    }
    std::sort(backgrounds.begin(),backgrounds.end(),bgCompare);//sort the vector
    for(auto& p : backgrounds) {
        //std::cout << p.name << "\n";
    }
    int titlebg = std::rand() % backgrounds.size();
    int knxfnbg = std::rand() % backgrounds.size();
    if (titlebg == knxfnbg) {
        knxfnbg = std::rand() % backgrounds.size(); //WHY TF AM I DOING THIS
    }

    titlescreen* title = new titlescreen(renderer, window, backgrounds, textures, music.data(), sound.data(), titlebg);
    game* gamer = new game(renderer, window, textures, backgrounds, music.data(), sound.data());
    knuxfanscreen* screen = new knuxfanscreen(renderer, textures, backgrounds, sound.data(),knxfnbg);
    server* srver = new server();
    srver->start();
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_JOYBUTTONDOWN) {
                switch (event.jbutton.button) {
                case JOY_B:
                case JOY_A: {
                    input(gamemode, title, gamer, SDLK_z);
                    break;
                    }
                case JOY_DOWN: {
                    input(gamemode, title, gamer, SDLK_DOWN);
                    break;
                }
                case JOY_UP: {
                    input(gamemode, title, gamer, SDLK_UP);
                    break;
                }
                case JOY_LEFT: {
                    input(gamemode, title, gamer, SDLK_LEFT);
                    break;
                }
                case JOY_RIGHT: {
                    input(gamemode, title, gamer, SDLK_RIGHT);
                    break;
                }
                case JOY_PLUS: {
                    input(gamemode, title, gamer, SDLK_x);
                    break;
                }

                }
            }
            if (event.type == SDL_KEYDOWN) {
                input(gamemode, title, gamer, event.key.keysym.sym);
            }
        }
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        srver->logic();
        deltaTime = (double)((NOW - LAST)*1000 / (double)SDL_GetPerformanceFrequency() );
        if (gamemode == 0) {
            screen->logic(deltaTime);
            screen->render();
            if (screen->endlogic() == 1) {
                gamemode = 1;
                title->reset();
            }

        }
        else if(gamemode == 1) {
            //printf("titlescreen");
            title->logic(deltaTime);
            title->render();
            if (title->endlogic() == 1) {
                gamer = new game(renderer, window, textures, backgrounds, music.data(), sound.data());
                gamer->reset();
                gamemode = 2;
                title->loadgame = false;
            }
        }
        else {
            //printf("gaming");
            srver->sendBlockArray(gamer->testblocks);
            gamer->logic(deltaTime);
            gamer->render();
            int logic = gamer->endlogic();
            if (logic == 1 || !gamer->gameactive) {
                title->reset();
                gamemode = 1;
            }

        }
    }

    return 0;
}
std::vector<SDL_Surface*> generateSurfaces(std::string path) {
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
std::vector<Mix_Music*> generateMusic(std::string path) {
    int i = 0;
    std::vector<Mix_Music*> music;
    Mix_Music* temp;
    std::vector<std::string> strings;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (i < 64) {
            std::string char_array{entry.path().u8string()}; //i dunno if its because im writing this at 10:55 and im passing out or what, but this code was IMPOSSIBLE to write.
            strings.push_back(char_array);
        }
        
        else { break; }
    }
    std::sort(strings.begin(),strings.end(),compareFunction);//sort the vector
    for(auto &string : strings) {
        temp = Mix_LoadMUS(string.c_str());
        if (!temp) {
            printf("Failed to load music at %s: %s\n", string, SDL_GetError());
            return music;
        }
        music.push_back(temp);
        printf("Successfully loaded music at %s\n", string.c_str());
    }
    return music;
}
std::vector<Mix_Chunk*> generateSounds(std::string path) {
    int i = 0;
    std::vector<Mix_Chunk*> sounds;
    Mix_Chunk* temp;
    std::vector<std::string> strings;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (i < 64) {
            std::string char_array{entry.path().u8string()}; //i dunno if its because im writing this at 10:55 and im passing out or what, but this code was IMPOSSIBLE to write.
            strings.push_back(char_array);
        }
        
        else { break; }
    }
    std::sort(strings.begin(),strings.end());//sort the vector
    for(auto &string : strings) {
        temp = Mix_LoadWAV(string.c_str());
        if (!temp) {
            printf("Failed to load sound at %s: %s\n", string, SDL_GetError());
            return sounds;
        }
        sounds.push_back(temp);
        printf("Successfully loaded sound at %s\n", string.c_str());
    }
    return sounds;
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

int input(int gamemode, titlescreen* title, game* gamer, SDL_Keycode keycode) {
    switch(gamemode) {
        case 1: {
            title->keyPressed(keycode);
            break;
        }
        case 2: {
            gamer->keyPressed(keycode);
            break;
        }
    }
    return 0;
}