#include "titlescreen.h"
#include <iostream>
#include <SDL2/SDL_mixer_ext.h>
#include <vector>
#include <math.h>
#include "../utils/defs.h"
#ifdef __SWITCH__
#define prefix  "/"
#include <switch.h>

#else
#define prefix  "./"
#endif

titlescreen::titlescreen()
{
    buttonfont = graphics::fonts->at(0);
    bodyfont = graphics::fonts->at(0);;
    versfont = graphics::fonts->at(2);
    headerfont = graphics::fonts->at(1);
    bgnum = std::rand() % graphics::backgrounds->size();
    
}
void titlescreen::reset()
{
    Mix_PlayMusic( audio::music->at(0), -1 );
    //audio::playMusic(0); //plays the first song in the list
}

void titlescreen::input(SDL_Keycode key)
{
    switch (currentscreen) {

    case 0: {

        switch (key) {
        case(SDLK_UP): {
            if (currentselection > 0) {
                currentselection = (currentselection - 1);
                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
            }
            break;
        }
        case(SDLK_DOWN): {
            if (currentselection < selections - 1) {
                currentselection = (currentselection + 1);
                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
            }
            break;
        }
        case(SDLK_z): {
            Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
            switch (currentselection) {
            case 0:
                loadgame = true;
                break;
            case 1:
                loadmenu = true;
                break;
            case 2:
                loadcreds = true;
                break;
            case 3:
                exit(0);
                break;
            }
            break;
        }

        }
        break;
    }

    case 1: {
        switch (key) {
        case(SDLK_z): {
            Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
            currentscreen = 0;
        }
                    break;
        }
        break;
    }
    case 2: {
        switch (key) {
        case(SDLK_UP): {
            if (currentselection > 0) {
                currentselection = (currentselection - 1);
            }
            Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
            break;
        }
        case(SDLK_DOWN): {
            if (currentselection < settingssize - 1) {
                currentselection = (currentselection + 1);
            }
            Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
            break;
        }
        case(SDLK_LEFT): {
            switch(currentsetting) {
                case 0:
                break;
                case 1: {
                    int currentvol = Mix_VolumeMusic(-1);
                    if(currentvol > 0) {
                        Mix_VolumeMusic(currentvol-1);
                    }
                    break;
                }
                case 2: {
                    int currentvol = Mix_Volume(-1,-1);
                    if(currentvol > 0) {
                        Mix_Volume(-1, currentvol-1);
                    }
                    break;

                }
            }
            break;
        }
        case(SDLK_RIGHT): {
            switch(currentsetting) {
                case 0:
                break;
                case 1: {
                    int currentvol = Mix_VolumeMusic(-1);
                    if(currentvol < MIX_MAX_VOLUME) {
                        Mix_VolumeMusic(currentvol+1);
                    }
                    break;
                }
                case 2: {
                    int currentvol = Mix_Volume(-1,-1);
                    if(currentvol < MIX_MAX_VOLUME) {
                        Mix_Volume(-1, currentvol+1);
                    }
                    break;

                }
            }
            break;
        }

        case(SDLK_z): {
            Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
            if (showerror || currentsetting > 0) {
                currentsetting = 0;                    
                showerror = false;
                
            }
            else {
                switch (currentselection) {
                case 0: {
                    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
                    bool IsFullscreen = SDL_GetWindowFlags(graphics::window) & FullscreenFlag;
                    SDL_SetWindowFullscreen(graphics::window, IsFullscreen ? 0 : FullscreenFlag);
                    SDL_ShowCursor(IsFullscreen);
                    break;
                }
                case 1:
                    currentsetting = 1;                    
                    break;
                case 2:
                    currentsetting = 2;                    
                    break;
                case 3:
                    currentscreen = 0;
                    currentselection = 2;
                    break;
                }
                break;
            }
        }

        }
        break;
    }

    case 3: {
        switch (key) {
        case(SDLK_z): {
            Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
            currentscreen = 0;
                }
            break;
            }
        break;
        }
    }
    debuginput(key);
}

void titlescreen::render()
{
#ifdef __LEGACY_RENDER
    SDL_RenderClear(graphics::render);
#else
#endif
    graphics::backgrounds->at(bgnum).render();

#ifdef __LEGACY_RENDER
    SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
    SDL_Rect bx = { 160, 250, 320, 210 };
    SDL_RenderFillRect(graphics::render, &bx);
    SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
    SDL_RenderDrawRect(graphics::render, &bx);
    SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
    graphics::drawTexture(graphics::sprites.at("knfnlogo"), 0, 0, 0.0, 1.0, false);
#else
    graphics::rect->render(graphics::shaders.at(1),{160,250},{160+320,250+210},0,{0,0,0,1},true,1,glm::vec4(1,1,1,1));
    graphics::sprite->render(graphics::shaders[4],graphics::sprites.at("knfnlogo"),glm::vec2(0,0),glm::vec2(640,480),0,glm::vec2(0,0),glm::vec2(640,480));
#endif

    for (int i = 0; i < selections; i++) {
        graphics::fonts->at(1)->render(320, 300 + (i * 32),settings[i],  true, 255, 0, ((i == currentselection && currentscreen == 0)?0:255),-1,false,0,0,0);
    }

    graphics::fonts->at(2)->render(490, 165, "Return of the Block!", true,255,255,255,-1,true,time/100,1,5);
    // graphics::fonts->at(0)->render("VERS 1.0 BABY!", 490, 165, true, 0, 0, 0, 0, true, time/100, 1, 5,1);
    versfont->render(320, 280, "High Score: " + std::to_string(settings::maxscore), true);
    versfont->render(320, 265, "Previous Score: " + std::to_string(settings::previousscore), true);
    switch (currentscreen) {
        case(1): {
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
            // SDL_Rect splashbox = { 0, 0, 640, 480 };
            // SDL_RenderFillRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
            // splashbox = { 320 - 240, 240 - 120, 480, 240 };
            // SDL_RenderFillRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
            // SDL_RenderDrawRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

            // headerfont->render(320, 140, messagetitle, true, renderer);
            // bodyfont->render(320, 180, messagebody, true, renderer,448);
            // buttonfont->render(320, 300, messagebutton, true, renderer);
            break;
        }
        case(2): {
            //None, and i mean none of this is used anymore! it was basically only in there for legacy sake anyways
            //that, and nostalgia value
            //so if you're nostalgic for the old SDLTris that nobody played, you can reimplement it yourself
            //have fun.
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
            // SDL_Rect splashbox = { 0, 0, 640, 480 };
            // SDL_RenderFillRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
            // splashbox = { 320 - 240, 80, 480, 320 };
            // SDL_RenderFillRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
            // SDL_RenderDrawRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

            // // headerfont->render(320, 100, settingstitle, true, renderer);
            // for (int i = 0; i < settingssize; i++) {
            //     // buttonfont->render(renderer, sets[i], 320, 130 + (i * 16),  true, 255, ((i == currentselection)?0:255), 255);
            // }
            // if (showerror) {
            //     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
            //     SDL_Rect splashbox = { 0, 0, 640, 480 };
            //     SDL_RenderFillRect(graphics::render, &splashbox);
            //     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
            //     splashbox = { 320 - 240, 240 - 120, 480, 240 };
            //     SDL_RenderFillRect(graphics::render, &splashbox);
            //     SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
            //     SDL_RenderDrawRect(graphics::render, &splashbox);
            //     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

            //     // headerfont->render(320, 140, messagetitle, true, renderer);
            //     // bodyfont->render(320, 180, messagebody, true, renderer);
            //     // buttonfont->render(320, 300, messagebutton, true, renderer);
            // }
            // else switch(currentsetting) {
            //     case 0:
            //     break;
            //     case 1: { //these didnt work anyways, might as well not keep them around

            //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
            //         // SDL_Rect splashbox = { 0, 0, 640, 480 };
            //         // SDL_RenderFillRect(graphics::render, &splashbox);
            //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
            //         // splashbox = { 320 - 240, 240 - 120, 480, 240 };
            //         // SDL_RenderFillRect(graphics::render, &splashbox);
            //         // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
            //         // SDL_RenderDrawRect(graphics::render, &splashbox);
            //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
            //         // double var = ((double) Mix_VolumeMusic(-1) / 128) * 58.0 + std::sin(time / 50) * 10;
            //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 80,  360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
            //         // double coz = ((double)Mix_VolumeMusic(-1) / 128) * 58.0 + std::cos(time / 50) * 10;
            //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);
            //         // headerfont->render(320, 140, "MUSIC VOLUME", true, renderer);
            //         // bodyfont->render(renderer, std::to_string(Mix_VolumeMusic(-1)), 320, 180, true, 255, 0, 255);
            //         // buttonfont->render(320, 300, messagebutton, true, renderer);
            //         break;
            //     }
            //     case 2: {
            //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
            //         // SDL_Rect splashbox = { 0, 0, 640, 480 };
            //         // SDL_RenderFillRect(graphics::render, &splashbox);
            //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
            //         // splashbox = { 320 - 240, 240 - 120, 480, 240 };
            //         // SDL_RenderFillRect(graphics::render, &splashbox);
            //         // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
            //         // SDL_RenderDrawRect(graphics::render, &splashbox);
            //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
            //         // double var = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + std::sin(time / 50) * 10;
            //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 80, 360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
            //         // double coz = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + std::cos(time / 50) * 10;
            //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);

            //         // headerfont->render(320, 140, "SOUND VOLUME", true, renderer);
            //         // bodyfont->render(renderer, std::to_string(Mix_Volume(-1, -1)), 320, 180, true, 255, 0, 255);
            //         // buttonfont->render(320, 300, messagebutton, true, renderer);


            //         break;
            //     }

            // }
            break;
        }
        case(3): {
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
            // SDL_Rect splashbox = { 0, 0, 640, 480 };
            // SDL_RenderFillRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
            // splashbox = { 320 - 240, 80, 480, 320 };
            // SDL_RenderFillRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
            // SDL_RenderDrawRect(graphics::render, &splashbox);
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
            // headerfont->render(320, 100, creditstitle, true);
            // for (int i = 0; i < creditssize; i++) {
            //     if (big[i] > 0) {
            //         headerfont->render(credits[i], 320, 130 + (i * 32), true, 255, ((i == currentselection) ? 0 : 255), 255,-1,false,0,0,0,1);
            //     }
            //     else {
            //         bodyfont->render(credits[i], 320, 130 + (i * 32), true, 255, ((i == currentselection) ? 0 : 255), 255,448,false,0,0,0,1);
            //     }
            // }
            // buttonfont->render(320, 300, messagebutton, true);
        }

    }
    //SDL_RenderPresent(renderer);

}

void titlescreen::logic(double deltatime)
{
    graphics::backgrounds->at(bgnum).logic(deltatime);
    // background[bgnum].logic(deltatime);
    time += deltatime;
}

Transition titlescreen::endLogic()
{
    Transition t = Transition();
    if (loadgame) {
        t.gamemode = 2;
        t.transition = true;
        loadgame = false;
    }
    if (loadmenu) {
        t.gamemode = 3;
        t.transition = true;
        loadmenu = false;
    }
    if (loadcreds) {
        t.gamemode = 4;
        t.transition = true;
        loadcreds = false;
    }
    return t;
}


void titlescreen::debuginput(SDL_Keycode key) {
    switch(key) {
        case SDLK_F1: {
            if(bgnum < background.size()-1) {
                bgnum++;
            }
            break;
        }

        case SDLK_F2: {
            if(bgnum > 0) {
            bgnum--;
            }
            break;
        }

    }
}