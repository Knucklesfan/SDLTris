#include "titlescreen.h"
#include <iostream>
#include <filesystem>
#include <SDL2/SDL_mixer.h>
#include <vector>

#ifdef __SWITCH__
#define prefix  "/"
#include <switch.h>

#else
#define prefix  "./"
#endif

titlescreen::titlescreen(SDL_Renderer* render, SDL_Window* windows, std::vector<bg>  backg, std::vector<SDL_Texture*> texture, Mix_Music* musicVec[], Mix_Chunk* soundVec[], int backgr, std::vector<font*> fonts)
{
    //std::filesystem::current_path().u8string()
    buttonfont = fonts.at(0);
    bodyfont = fonts.at(0);
    versfont = fonts.at(2);
    headerfont = fonts.at(1);



    window = windows;
    background = backg;
    bgnum = backgr;
	renderer = render;
	textures = texture;
    music = musicVec;
    sound = soundVec;

}
void titlescreen::reset()
{
    Mix_HaltMusic();
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( music[0], -1 );
    }
    //If music is being played
    else
    {
        //If the music is paused
        if( Mix_PausedMusic() == 1 )
        {
            //Resume the music
        Mix_ResumeMusic();
        }
        //If the music is playing
        else
        {
        //Pause the music
            Mix_PauseMusic();
        }
    }

}

void titlescreen::keyPressed(SDL_Keycode key)
{
    switch (currentscreen) {

    case 0: {

        switch (key) {
        case(SDLK_UP): {
            if (currentselection > 0) {
                currentselection = (currentselection - 1);
                Mix_PlayChannel( -1, sound[1], 0 );
            }
            break;
        }
        case(SDLK_DOWN): {
            if (currentselection < selections - 1) {
                currentselection = (currentselection + 1);
                Mix_PlayChannel( -1, sound[1], 0 );
            }
            break;
        }
        case(SDLK_z): {
            Mix_PlayChannel( -1, sound[0], 0 );
            switch (currentselection) {
            case 0:
                loadgame = true;
                break;
            case 1:
                currentscreen = 1;
                break;
            case 2:
                currentselection = 0;
                currentscreen = 2;
                break;
            case 3:
                currentscreen = 3;
                break;
            case 4:
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
            Mix_PlayChannel( -1, sound[0], 0 );
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
            Mix_PlayChannel( -1, sound[1], 0 );
            break;
        }
        case(SDLK_DOWN): {
            if (currentselection < settingssize - 1) {
                currentselection = (currentselection + 1);
            }
            Mix_PlayChannel( -1, sound[1], 0 );
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
            Mix_PlayChannel( -1, sound[0], 0 );
            if (showerror || currentsetting > 0) {
                currentsetting = 0;                    
                showerror = false;
                
            }
            else {
                switch (currentselection) {
                case 0: {
                    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
                    bool IsFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;
                    SDL_SetWindowFullscreen(window, IsFullscreen ? 0 : FullscreenFlag);
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
            Mix_PlayChannel( -1, sound[0], 0 );
            currentscreen = 0;
                }
            break;
            }
        break;
        }
    }
    debuginput(key);
}

void titlescreen::render(highscore* score)
{
    SDL_RenderClear(renderer);

    background[bgnum].render(renderer, false);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_Rect bx = { 160, 250, 320, 210 };
    SDL_RenderFillRect(renderer, &bx);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bx);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);

    drawTexture(textures[5], 0, 0, 0.0, 1.0, false);

    background[bgnum].render(renderer, true);

    for (int i = 0; i < selections; i++) {
        headerfont->render(renderer, options[i], 320, 300 + (i * 32),  true, 255, ((i == currentselection && currentscreen == 0)?0:255), 255);
    }
    versfont->render(renderer, "VERSION 0.2 DEMO", 490, 165, true, 0, 0, 0, 0, true, time/100, 1, 5);
    versfont->render(320, 280, "High Score: " + std::to_string(score->maxscore), true,renderer);
    versfont->render(320, 265, "Previous Score: " + std::to_string(score->previousscore), true, renderer);
    switch (currentscreen) {
        case(1): {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            splashbox = { 320 - 240, 240 - 120, 480, 240 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);

            headerfont->render(320, 140, messagetitle, true, renderer);
            bodyfont->render(320, 180, messagebody, true, renderer,448);
            buttonfont->render(320, 300, messagebutton, true, renderer);
            break;
        }
        case(2): {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            splashbox = { 320 - 240, 80, 480, 320 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);

            headerfont->render(320, 100, settingstitle, true, renderer);
            for (int i = 0; i < settingssize; i++) {
                buttonfont->render(renderer, settings[i], 320, 130 + (i * 16),  true, 255, ((i == currentselection)?0:255), 255);
            }
            if (showerror) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_Rect splashbox = { 0, 0, 640, 480 };
                SDL_RenderFillRect(renderer, &splashbox);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                splashbox = { 320 - 240, 240 - 120, 480, 240 };
                SDL_RenderFillRect(renderer, &splashbox);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &splashbox);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);

                headerfont->render(320, 140, messagetitle, true, renderer);
                bodyfont->render(320, 180, messagebody, true, renderer);
                buttonfont->render(320, 300, messagebutton, true, renderer);
            }
            else switch(currentsetting) {
                case 0:
                break;
                case 1: {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                    SDL_Rect splashbox = { 0, 0, 640, 480 };
                    SDL_RenderFillRect(renderer, &splashbox);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                    splashbox = { 320 - 240, 240 - 120, 480, 240 };
                    SDL_RenderFillRect(renderer, &splashbox);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &splashbox);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);
                    double var = ((double) Mix_VolumeMusic(-1) / 128) * 58.0 + sin(time / 50) * 10;
                    drawTexture(renderer, textures.at(7), 80,  360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
                    double coz = ((double)Mix_VolumeMusic(-1) / 128) * 58.0 + cos(time / 50) * 10;
                    drawTexture(renderer, textures.at(7), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);
                    headerfont->render(320, 140, "MUSIC VOLUME", true, renderer);
                    bodyfont->render(renderer, std::to_string(Mix_VolumeMusic(-1)), 320, 180, true, 255, 0, 255);
                    buttonfont->render(320, 300, messagebutton, true, renderer);
                    break;
                }
                case 2: {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                    SDL_Rect splashbox = { 0, 0, 640, 480 };
                    SDL_RenderFillRect(renderer, &splashbox);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
                    splashbox = { 320 - 240, 240 - 120, 480, 240 };
                    SDL_RenderFillRect(renderer, &splashbox);
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderDrawRect(renderer, &splashbox);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);
                    double var = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + sin(time / 50) * 10;
                    drawTexture(renderer, textures.at(7), 80, 360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
                    double coz = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + cos(time / 50) * 10;
                    drawTexture(renderer, textures.at(7), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);

                    headerfont->render(320, 140, "SOUND VOLUME", true, renderer);
                    bodyfont->render(renderer, std::to_string(Mix_Volume(-1, -1)), 320, 180, true, 255, 0, 255);
                    buttonfont->render(320, 300, messagebutton, true, renderer);


                    break;
                }

            }
            break;
        }
        case(3): {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            splashbox = { 320 - 240, 80, 480, 320 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);
            headerfont->render(320, 100, creditstitle, true, renderer);
            for (int i = 0; i < creditssize; i++) {
                if (big[i] > 0) {
                    headerfont->render(renderer, credits[i], 320, 130 + (i * 32), true, 255, ((i == currentselection) ? 0 : 255), 255);
                }
                else {
                    bodyfont->render(renderer, credits[i], 320, 130 + (i * 32), true, 255, ((i == currentselection) ? 0 : 255), 255,448);
                }
            }
            buttonfont->render(320, 300, messagebutton, true, renderer);
        }

    }
    SDL_RenderPresent(renderer);

}

void titlescreen::logic(double deltatime)
{
    background[bgnum].logic(deltatime);
    time += deltatime;
}

int titlescreen::endlogic()
{
    if (loadgame) {
        return 1;
    }
	return 0;
}

//this is the legacy drawtexture, for drawing sprites of whom's width and height are unknown
void titlescreen::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
    SDL_Rect sprite;
    SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h);
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
//this is the new drawtexture, for drawing sprite sheets and whatnot
void titlescreen::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
    SDL_Rect sprite;
    SDL_Rect srcrect = { srcx, srcy, srcw, srch };
    if (SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    sprite.w = srcw * scale;
    sprite.h = srch * scale;
    if (center) {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else {
        sprite.x = x + srcw / 2 - sprite.w / 2;
        sprite.y = y + srch / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(renderer, texture, &srcrect, &sprite, 0, NULL, SDL_FLIP_NONE);
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