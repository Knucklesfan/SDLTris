#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer_ext.h>
//#include <enet/enet.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <ctime>
#include <chrono>
#include <thread>
#include "utils/defs.h"
#include "globalgamemode.h"
#include "gamemode.h"
#include "scenes/debugscene.h"
// #include "scenes/options.h"
#ifndef __LEGACY_RENDER
#include "opengl/buffermanager.h"
#endif

#ifdef _NETCODE
#include "server.h"
#endif

//TODO Fix RPC on Linux
#ifdef _WIN32
    #include "rpcimplement.h"
#endif
//TODO: ALL NETCODE HAS BEEN DISABLED

//i did this for a number of reasons:
// 1. I don't have the server code working just yet
// 2. I want to make a release and as such including an unfinished, unsafe server in the code is a bad idea to send to people
// 3. Windows Firewall freaking hates this thing rn
// 4. I'm lazy.
// If for some reason you want to compile with the netcode, then add _NETCODE to your definitions. Other than that, sorry!


/*
SDLTetris.cpp or how I learned to stop worrying and love the OpenGL
Today, my friends, it begins. The full recode and port of SDLTetris to use... well, not SDL2! Instead,
we are bringing it over to a new OpenGL Renderer that's been cooking for a while,
unfinished but soon to be finished!

*/
#undef main

void preciseSleep(double seconds) {
    static double estimate = 5e-3;
    static double mean = 5e-3;
    static double m2 = 0;
    static int64_t count = 1;
    while (seconds > estimate) {
        auto start = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        auto end = std::chrono::high_resolution_clock::now();
        double observed = (end - start).count() / 1e9;
        seconds -= observed;
        ++count;
        double delta = observed - mean;
        mean += delta / count;
        m2 += delta * (observed - mean);
        double stddev = sqrt(m2 / (count - 1));
        estimate = mean + stddev;
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto spinNs = (int64_t)(seconds * 1e9);
    auto delay = std::chrono::nanoseconds(spinNs);
    while (std::chrono::high_resolution_clock::now() - start < delay);
}
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
int WINDOW_WIDTH = INTERNAL_WIDTH;
int WINDOW_HEIGHT = INTERNAL_HEIGHT;
int main(int argc, char **argv) {
#ifdef __SWITCH__
    consoleInit(NULL);
#endif // __SWITCH__

        srand((unsigned)time(0));

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
#ifdef __LEGACY_RENDER
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
#else
	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Create an SDL window
	SDL_Window*  window = SDL_CreateWindow("SDLTetris Rewritten", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
		// we'll print an error message and exit
		std::cerr << "Error failed to create window!\n";
		return 1;
	}


	// Create an OpenGL context (so we can use OpenGL functions)
	SDL_GLContext context = SDL_GL_CreateContext(window);

	// if we failed to create a context
	if (!context) {
		// we'll print out an error message and exit
		std::cerr << "Error failed to create a context\n!";
		return 2;
	}
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(-1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
    graphics::rect = new rectRenderer();
    graphics::line = new lineRenderer();
    graphics::sprite = new spriteRenderer();
#endif
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
#ifdef __LEGACY_RENDER
    SDL_SetRenderDrawBlendMode(graphics::render, SDL_BLENDMODE_BLEND);

    SDL_RenderSetLogicalSize(graphics::render, 640, 480);
#else

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// glDepthFunc(GL_LESS);
	// glEnable(GL_DEPTH_TEST);  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	graphics::globalbuffer = new buffermanager(INTERNAL_WIDTH,INTERNAL_HEIGHT);
    graphics::generateshaders();

#endif
    std::string prefix = filepath;
    graphics::generatebgs();
    graphics::generatesprites();
    graphics::generatefonts();
    graphics::generatecubemaps();
    audio::generatemusic();
    audio::generatesound();
    settings::loadSettings();
    settings::loadDemos();
    GlobalGamemode* global = new GlobalGamemode();
    gameplay::loadGamemodes();
    #ifdef __LEGACY_RENDER
    SDL_Texture* rendertext = SDL_CreateTexture(graphics::render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640,480);
    
    if(SDL_QueryTexture(rendertext, NULL, NULL, NULL, NULL) < 0) {
        printf("Failed to make render texture... What happened??? %s\n",SDL_GetError());
    };

    #else

    #endif



    SDL_Event event;
    bool quit = false;
    float NOW = ((1000.0f * (float)SDL_GetPerformanceCounter()) / SDL_GetPerformanceFrequency());
    float LAST = ((1000.0f * (float)SDL_GetPerformanceCounter()) / SDL_GetPerformanceFrequency());
    float deltaTime = 0;
    double ticks = 0;
    int realtick = 0;
    double time = 0; //time of current frame
    double oldTime = SDL_GetTicks(); //time of previous framea
    long long recordticks = 0;
    std::string argument = "";
    if(argc > 1) {
        argument = argv[1];
    }
    if(argument == "--debug") {
        gameplay::gamemodes.push_back(new debugscene());
        gameplay::gamemode = gameplay::gamemodes.size()-1;
    }

    std::cout << "Finished initializing!\n";

    int titlebg = std::rand() % graphics::backgrounds->size();
    int knxfnbg = std::rand() % graphics::backgrounds->size();
    if (titlebg == knxfnbg) {
        knxfnbg = std::rand() % graphics::backgrounds->size(); //WHY TF AM I DOING THIS
    }
    next_time = SDL_GetTicks() + TICK_INTERVAL;
    for(int i = 0; i<gameplay::gamemodes.size(); i++) {
        std::cout << gameplay::gamemodes[i]->name << "\n";
    }
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
            if (event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_F12) {
                    graphics::screenshot();
                }
                gameplay::gamemodes[gameplay::gamemode]->input(event.key.keysym.sym);
            }
            if(event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					WINDOW_WIDTH =(event.window.data1);
					WINDOW_HEIGHT = (event.window.data2);

        		}
			}
        }

        time = SDL_GetTicks();
        NOW = ((1000.0f * (double)SDL_GetPerformanceCounter()) / SDL_GetPerformanceFrequency());
        graphics::deltaTime = (NOW - LAST); //frameTime is the time this frame has taken, in seconds
        double frameTime = graphics::deltaTime /1000.0;
        double tFps = (1.0 / frameTime);
        oldTime = time;
        LAST = NOW;
        SDL_PumpEvents();
        #ifdef __LEGACY_RENDER
        SDL_RenderClear(graphics::render);
        SDL_SetRenderTarget(graphics::render,rendertext);
        #else
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            graphics::globalbuffer->enable();
            global->startRender();
        #endif
        gameplay::gamemodes[gameplay::gamemode]->logic(graphics::deltaTime);
        gameplay::gamemodes[gameplay::gamemode]->render();
        Transition endlogic = gameplay::gamemodes[gameplay::gamemode]->endLogic();
        if(endlogic.transition) {
            global->setFade(endlogic);
        };
        
        if(global->logic(graphics::deltaTime)) {
            gameplay::gamemode=global->currentTransition.gamemode;
            gameplay::gamemodes[gameplay::gamemode]->reset();
        }
        global->render();
        #ifdef __LEGACY_RENDER

        SDL_SetRenderTarget(graphics::render,NULL);
        SDL_RenderCopy(graphics::render,rendertext,NULL,NULL);
        std::cout << tFps << "\n";
        SDL_RenderPresent(graphics::render);

        #else

        graphics::fonts->at(2)->render(16, 16, std::to_string(tFps), false);

        graphics::globalbuffer->disable(WINDOW_WIDTH,WINDOW_HEIGHT);
        graphics::globalbuffer->render(graphics::shaders[3],WINDOW_WIDTH,WINDOW_HEIGHT,true);
        // preciseSleep(floor(1000.0f/60.0f - deltaTime)/1000.0f);
        SDL_GL_SwapWindow(window);
        #endif

}
	SDL_GL_DeleteContext(context);

	// Destroy the window
	SDL_DestroyWindow(window);

	// And quit SDL
	SDL_Quit();

}
