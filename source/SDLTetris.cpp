#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <enet/enet.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <ctime>

#include "game.h"
#include "titlescreen.h"
#include "background.h"
#include "knuxfanscreen.h"
#include "replay.h"
#include "results.h"
#include "credits.h"

#ifdef _NETCODE
#include "server.h"
#endif

#ifdef _WIN32
    #include "rpcimplement.h"
#endif
#include "highscore.h"
//TODO: ALL NETCODE HAS BEEN DISABLED

//i did this for a number of reasons:
// 1. I don't have the server code working just yet
// 2. I want to make a release and as such including an unfinished, unsafe server in the code is a bad idea to send to people
// 3. Windows Firewall freaking hates this thing rn
// 4. I'm lazy.
// If for some reason you want to compile with the netcode, then add _NETCODE to your definitions. Other than that, sorry!

#undef main
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#ifdef __SWITCH__
    #define filepath  "/"
    #include <switch.h>

#else
    #define filepath  "./"
#endif
#include "options.h"

#define JOY_A     0
#define JOY_B     1
#define JOY_X     2
#define JOY_Y     3
#define JOY_PLUS  10
#define JOY_L  6

#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15


std::vector<Mix_Music*> generateMusic(std::string path);
std::vector<Mix_Chunk*> generateSounds(std::string path);
std::vector<SDL_Texture*> generateSurfaces(std::string path, SDL_Renderer* renderer, int sprites);
bool hasEnding(std::string const& fullString, std::string const& ending);
bool compareFunction (std::string a, std::string b) {return a<b;} 
bool bgCompare (bg a, bg b) {return a.name<b.name;} 
int input(int gamemode, titlescreen* title, game* gamer, results* res, options* opt, SDL_Keycode keycode);

int main() {
#ifdef __SWITCH__
    consoleInit(NULL);
#endif // __SWITCH__

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
    //Mix_VolumeMusic(0);

    SDL_Joystick* joystick;

    SDL_Joystick* gGameController = SDL_JoystickOpen(0);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(renderer, 640, 480);
    std::string prefix = filepath;

    rapidxml::file<> spriteFile((prefix+"sprites/sprites.xml").c_str());
    rapidxml::xml_document<> spriteDoc;
    spriteDoc.parse<0>(spriteFile.data());
    int sprites = atoi(spriteDoc.first_node("sprites")->value());

    std::vector<SDL_Texture*> textures = generateSurfaces(prefix + "sprites/", renderer, sprites); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!
    std::vector<Mix_Music*> music = generateMusic(prefix + "music/");
    std::vector<Mix_Chunk*> sound = generateSounds(prefix + "sound/");
    SDL_Texture* rendertext = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640,480);

    if(SDL_QueryTexture(rendertext, NULL, NULL, NULL, NULL) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };



    std::vector<bg> backgrounds;
    SDL_Event event;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double deltaTime = 0;
    double ticks = 0;
    int realtick = 0;
    int gamemode = 5;
    long long recordticks = 0;
    rapidxml::file<> bgFile((prefix+"backgrounds/backgrounds.xml").c_str());
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    int bgs = atoi(bgDoc.first_node("bgs")->value());
    for(int i = 0; i < bgs; i++) {
            rapidxml::file<> bgFile((prefix+"backgrounds/backgrounds.xml").c_str());
            rapidxml::xml_document<> bgDoc;
            bgDoc.parse<0>(bgFile.data());
            std::string tmp = "bg" + std::to_string(i);
            std::cout << tmp << "\n";
            std::string bgPath = bgDoc.first_node(tmp.c_str())->value();
            std::cout << "loading background " << bgPath << "\n";
            //std::cout << "HELP ME:" << p.path().filename() << "\n";
            bg backg(bgPath, false, renderer);
            backgrounds.push_back(backg);
    }
    bg optionsbg(prefix + "sprites/resultsbg", true, renderer);    
    bg configbg(prefix + "sprites/optionsbg", true, renderer);
    std::sort(backgrounds.begin(),backgrounds.end(),bgCompare);//sort the vector
    int titlebg = std::rand() % backgrounds.size();
    int knxfnbg = std::rand() % backgrounds.size();
    if (titlebg == knxfnbg) {
        knxfnbg = std::rand() % backgrounds.size(); //WHY TF AM I DOING THIS
    }

    //time to load fonts
    //HERES HOW I WANTED THIS TO WORK:
    //each object generates its own fonts, but since that wastes a ton of memory, i guess we're doing this now!
    //The code is a bit more messy, mostly because I feel like i'm passing so much crap as pointers to these objects
    //but who cares as long as it works

    std::vector<font*> fonts;
    fonts.push_back(new font("8x8font",renderer));
    fonts.push_back(new font("8x16font",renderer));
    fonts.push_back(new font("small8x8font",renderer));
    options* opt = new options(renderer, window, configbg, textures, configbg.music, sound.data(), fonts, backgrounds);
    titlescreen* title = new titlescreen(renderer, window, backgrounds, textures, music.data(), sound.data(), titlebg, fonts);
    game* gamer = new game(renderer, window, textures, backgrounds, music.data(), sound.data(), fonts, opt->activations);
    knuxfanscreen* screen = new knuxfanscreen(renderer, textures, backgrounds, sound.data(),knxfnbg, fonts[2]);
    results* res = new results(renderer, window, optionsbg, textures, optionsbg.music, sound.data(), fonts);
    credits* crd = new credits(renderer,fonts);
#ifdef _NETCODE
    server* srver = new server();
    srver->start();
#endif
    highscore* score = new highscore();


    //rpcimplement rpc();
#ifdef _WIN32
    rpcimplement* rpc = new rpcimplement();
    discord::Timestamp time = 0;
    time = std::time(nullptr);
    rpc->update("At the Knuxfan Screen.", "Top high score: " + std::to_string(score->maxscore), "icon1", time);

#endif
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_JOYBUTTONDOWN) {
                switch (event.jbutton.button) {
                case JOY_B:
                case JOY_A: {
                    input(gamemode, title, gamer, res, opt, SDLK_z);
                    break;
                }
                case JOY_DOWN: {
                    input(gamemode, title, gamer, res, opt, SDLK_DOWN);
                    break;
                }
                case JOY_UP: {
                    input(gamemode, title, gamer, res, opt, SDLK_UP);
                    break;
                }
                case JOY_LEFT: {
                    input(gamemode, title, gamer, res, opt, SDLK_LEFT);
                    break;
                }
                case JOY_RIGHT: {
                    input(gamemode, title, gamer, res, opt, SDLK_RIGHT);
                    break;
                }
                case JOY_L: {
                    input(gamemode, title, gamer, res, opt, SDLK_x);
                    break;
                }
                case JOY_PLUS: {
                    input(gamemode, title, gamer, res, opt, SDLK_ESCAPE);
                    break;
                }

                }
            }
            if (event.type == SDL_KEYDOWN) {
                input(gamemode, title, gamer, res, opt, event.key.keysym.sym);
            }
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
        SDL_SetRenderTarget(renderer, rendertext);
        SDL_RenderClear(renderer);
#ifdef _NETCODE
        srver->logic();
#endif
        switch (gamemode) {
        default:
        case 0: {
            screen->logic(deltaTime);
            screen->render();
            if (screen->endlogic() == 1) {
                gamemode = 1;
                title->reset();
#ifdef _WIN32
                time = std::time(nullptr);
                rpc->update("On the Title Screen.", "Top high score: " + std::to_string(score->maxscore), "icon2", time);
#endif

            }
            break;
        }
        case 1: {
            //printf("titlescreen");
            title->logic(deltaTime);
            title->render(score);
            int logicret = title->endlogic();
            if (logicret == 1) {
                gamer = new game(renderer, window, textures, backgrounds, music.data(), sound.data(), fonts, opt->activations);
                gamer->reset();
                gamemode = 2;
                title->loadgame = false;
                recordticks = 0;
#ifdef _WIN32
                time = std::time(nullptr);
                rpc->update("Droppin' some blocks", "LN: 0 LV: 0", "mainicon", time);
#endif
            }
            if (logicret == 2) {
                opt->reset();
                gamemode = 4;
#ifdef _WIN32
                rpc->update("Configuring the game.", "Top high score: " + std::to_string(score->maxscore), "icon2", time);
#endif
                title->loadmenu = false;
            }
            break;
        }
        case 2: {
            //printf("gaming");
#ifdef _NETCODE
            srver->sendBlockArray(gamer->testblocks);
#endif
            gamer->logic(deltaTime);

#ifdef _WIN32
            if (!gamer->paused) {
                std::string scoretxt = "LN: " + std::to_string(gamer->lines) + " LV: " + std::to_string(gamer->level);
                rpc->update("Droppin' some blocks", scoretxt, "mainicon", time);
            }
            else {
                std::string scoretxt = "LN: " + std::to_string(gamer->lines) + " LV: " + std::to_string(gamer->level);
                rpc->update("Paused..", scoretxt, "mainicon", time);

            }
#endif
            gamer->render();
            int logic = gamer->endlogic();
            std::cout << logic;
            if (logic == 1 || !gamer->gameactive) {
                res->reset();
                gamemode = 3;
#ifdef _WIN32
                score->update(gamer->score);
                time = std::time(nullptr);
                rpc->update("Game over!", "Top high score: " + std::to_string(score->maxscore), "icon2", time);
#endif

            }
            break;
        }
        case 3: {
            res->logic(deltaTime);
            res->render(gamer);
            if (res->endlogic() > 0) {
                title->reset();
                gamemode = 1;
#ifdef _WIN32
                score->update(gamer->score);
                time = std::time(nullptr);
                rpc->update("In the menu.", "Top high score: " + std::to_string(score->maxscore), "icon2", time);
#endif

            }
            break;
        }
        case 4: {
            opt->logic(deltaTime);
            opt->render();
            if (opt->endlogic() > 0) {
                title->reset();
                gamemode = 1;
#ifdef _WIN32
                rpc->update("In the menu.", "Top high score: " + std::to_string(score->maxscore), "icon2", time);
#endif
                opt->loadgame = false;
            }
            break;
        }
        case 5: {
            crd->logic(deltaTime);
            crd->render(renderer);
            break;
        }
        }
    #ifdef _WIN32
            rpc->logic();
    #endif
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, rendertext, NULL,NULL);
    SDL_RenderPresent(renderer);
    }
    return 0;
}
std::vector<Mix_Music*> generateMusic(std::string path) {
    rapidxml::file<> musFile((path+"/music.xml").c_str());
    rapidxml::xml_document<> musDoc;
    musDoc.parse<0>(musFile.data());
    std::vector<Mix_Music*> music;
    Mix_Music* temp;
    int musics = atoi(musDoc.first_node("songs")->value());
    for(int i = 0; i < musics; i++) {
            std::string tmp = "mus" + std::to_string(i);
            std::cout << tmp << "\n";
            std::string musPath = path + "/" + musDoc.first_node(tmp.c_str())->value();
            temp = Mix_LoadMUS(musPath.c_str());
            if (!temp) {
                printf("Failed to load music at %s: %s\n", musPath, SDL_GetError());
                return music;
            }
            music.push_back(temp);
            printf("Successfully loaded music at %s\n", musPath.c_str());

    }
    return music;
}


std::vector<Mix_Chunk*> generateSounds(std::string path) {
    rapidxml::file<> musFile((path+"/sound.xml").c_str());
    rapidxml::xml_document<> musDoc;
    musDoc.parse<0>(musFile.data());
    std::vector<Mix_Chunk*> music;
    Mix_Chunk* temp;
    int musics = atoi(musDoc.first_node("sounds")->value());
    for(int i = 0; i < musics; i++) {
            std::string tmp = "snd" + std::to_string(i);
            std::cout << tmp << "\n";
            std::string musPath = path + "/" + musDoc.first_node(tmp.c_str())->value();
            temp = Mix_LoadWAV(musPath.c_str());
            if (!temp) {
                printf("Failed to load music at %s: %s\n", musPath, SDL_GetError());
                return music;
            }
            music.push_back(temp);
            printf("Successfully loaded music at %s\n", musPath.c_str());

    }
    return music;
}

bool hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

int input(int gamemode, titlescreen* title, game* gamer, results* res, options* opt, SDL_Keycode keycode) {
    switch(gamemode) {
        case 1: {
            title->keyPressed(keycode);
            break;
        }
        case 2: {
            gamer->keyPressed(keycode);
            break;
        }
        case 3: {
            res->keyPressed(keycode);
            break;
        }
        case 4: {
            opt->keyPressed(keycode);
            break;
        }

    }
    return 0;
}

std::vector<SDL_Texture*> generateSurfaces(std::string path, SDL_Renderer* renderer, int sprites) {
    std::vector<SDL_Surface*> surfaces;
    std::vector<SDL_Texture*> textures;
    for(int i = 0; i < sprites; i++) {
        char buff[12];
        snprintf(buff, sizeof(buff), "%02d", i);
        std::string temppath = path + "/" + buff + ".bmp";

        SDL_Surface* temp = SDL_LoadBMP(temppath.c_str());
        if (!temp) {
            printf("Failed to load image at %s: %s\n", temppath, SDL_GetError());
        }
        surfaces.push_back(temp);
        printf("Successfully loaded sprite at %s\n", temppath.c_str());
    }
    for (SDL_Surface* surf : surfaces) {
        SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, surf);
        if(temp != NULL) {
            textures.push_back(temp);
            printf("pushed texture!!\n");
            SDL_FreeSurface(surf);
        }
        else {
            fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        }
    }
    return textures;

}