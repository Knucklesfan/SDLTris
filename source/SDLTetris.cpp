#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
//#include <enet/enet.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <ctime>
#include <chrono>

#include "utils/defs.h"
#include "globalgamemode.h"
#include "gamemode.h"
#include "scenes/knuxfanscreen.h"
#include "scenes/titlescreen.h"
#include "scenes/credits.h"
#include "scenes/game.h"
#include "scenes/options.h"

#include "scenes/white.h"

#ifdef _NETCODE
#include "server.h"
#endif

//TODO Fix RPC on Linux
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

static Uint32 next_time;

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}

bool hasEnding(std::string const& fullString, std::string const& ending);
bool compareFunction (std::string a, std::string b) {return a<b;} 

int main() {
#ifdef __SWITCH__
    consoleInit(NULL);
#endif // __SWITCH__

        srand((unsigned)time(0));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    graphics::window = SDL_CreateWindow("Knuxfan's Tetriminos", 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (graphics::window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

        SDL_Quit();

        return 1;
    }
    graphics::render = SDL_CreateRenderer(graphics::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (graphics::render == nullptr) {
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

        SDL_DestroyWindow(graphics::window);
        SDL_Quit();

        return 1;
    }
    SDL_SetRenderDrawBlendMode(graphics::render, SDL_BLENDMODE_BLEND);

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        SDL_DestroyWindow(graphics::window);
        SDL_Quit();

        return 1;
    }
    //Mix_VolumeMusic(0);

    SDL_Joystick* joystick;

    SDL_Joystick* gGameController = SDL_JoystickOpen(0);

    SDL_SetRenderDrawBlendMode(graphics::render, SDL_BLENDMODE_BLEND);

    SDL_RenderSetLogicalSize(graphics::render, 640, 480);
    std::string prefix = filepath;

    graphics::generatebgs();
    graphics::generatesprites();
    graphics::generatefonts();
    audio::generatemusic();
    audio::generatesound();
    settings::loadSettings();

    GlobalGamemode* global = new GlobalGamemode();
    SDL_Texture* rendertext = SDL_CreateTexture(graphics::render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640,480);

    if(SDL_QueryTexture(rendertext, NULL, NULL, NULL, NULL) < 0) {
        printf("Failed to make render texture... What happened??? %s\n",SDL_GetError());
    };



    SDL_Event event;
    bool quit = false;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    float deltaTime = 0;
    double ticks = 0;
    int realtick = 0;
    double time = 0; //time of current frame
    double oldTime = 0; //time of previous framea
    long long recordticks = 0;
    std::cout << "Finished initializing!\n";
    Gamemode* gamemodes[] = {
        // new white(),
        new knuxfanscreen(), //0
        new titlescreen(), //1
        new game(), //2
        new options(), //3
        new credits() //4
    };
    std::cout << "test2";
    int gamemode = 0;

    int titlebg = std::rand() % graphics::backgrounds->size();
    int knxfnbg = std::rand() % graphics::backgrounds->size();
    if (titlebg == knxfnbg) {
        knxfnbg = std::rand() % graphics::backgrounds->size(); //WHY TF AM I DOING THIS
    }
    next_time = SDL_GetTicks() + TICK_INTERVAL;

    //rpcimplement rpc();
#ifdef _WIN32
    rpcimplement* rpc = new rpcimplement();
    discord::Timestamp time = 0;
    time = std::time(nullptr);
    rpc->update("At the Knuxfan Screen.", "Top high score: " + std::to_string(score->maxscore), "icon1", time);
#endif
    while (!quit) {
        auto t1 = std::chrono::high_resolution_clock::now();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                gamemode[gamemodes]->input(event.key.keysym.sym);
            }
        }

        time = SDL_GetTicks();

        graphics::deltaTime = (time - oldTime); //frameTime is the time this frame has taken, in seconds
        double frameTime = graphics::deltaTime /1000.0;
        double tFps = (1.0 / frameTime);

        if(graphics::deltaTime > 1000/60.0) {
            oldTime = time;
            SDL_PumpEvents();
            SDL_RenderClear(graphics::render);
            SDL_SetRenderTarget(graphics::render,rendertext);
            gamemodes[gamemode]->logic(graphics::deltaTime);
            gamemodes[gamemode]->render();
            Transition endlogic = gamemodes[gamemode]->endLogic();
            if(endlogic.transition) {
                global->setFade(endlogic);
            };
            if(global->logic(graphics::deltaTime)) {
                gamemode=global->currentTransition.gamemode;
                gamemodes[gamemode]->reset();
            }
            global->render();
            SDL_SetRenderTarget(graphics::render,NULL);
            SDL_RenderCopy(graphics::render,rendertext,NULL,NULL);
            std::cout << tFps << "\n";
            graphics::fonts->at(2)->render(16, 16, std::to_string(tFps), false);
            SDL_RenderPresent(graphics::render);
    }
}
}

// int input(int gamemode, titlescreen* title, game* gamer, results* res, options* opt, credits* cred, knuxfanscreen* knfn, SDL_Keycode keycode) { //THIS IS HORRIBLE
//     switch(gamemode) {
//         case 0: {
//             knfn->keyPressed(keycode);
//             break;
//         }
//         case 1: {
//             title->keyPressed(keycode);
//             break;
//         }
//         case 2: {
//             gamer->keyPressed(keycode);
//             break;
//         }
//         case 3: {
//             res->keyPressed(keycode);
//             break;
//         }
//         case 4: {
//             opt->keyPressed(keycode);
//             break;
//         }
//         case 5: {
//             cred->keyPressed(keycode);
//             break;
//         }

//     }
//     return 0;
// }
