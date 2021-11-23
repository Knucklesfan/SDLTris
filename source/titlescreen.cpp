#include "titlescreen.h"
#include <iostream>
#include <filesystem>
#include <SDL2/SDL_ttf.h>
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
    SDL_Rect bx = { 160, 250, 320, 195 };
    SDL_RenderFillRect(renderer, &bx);
    drawTexture(textures[5], 0, 0, 0.0, 1.0, false);

    background[bgnum].render(renderer, true);

    for (int i = 0; i < selections; i++) {
        buttonfont->render(renderer, options[i], 320, 300 + (i * 16),  true, 255, ((i == currentselection && currentscreen == 0)?0:255), 255);
    }
    versfont->render(490, 165, "VERSION 0.2 DEMO", true, renderer);
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
            headerfont->render(320, 140, messagetitle, true, renderer);
            bodyfont->render(320, 180, messagebody, true, renderer);
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
            headerfont->render(320, 100, creditstitle, true, renderer);
            for (int i = 0; i < creditssize; i++) {
                versfont->render(renderer, credits[i], 320, 130 + (i * 32),  true, 255, ((i == currentselection)?0:255), 255);
            }
            buttonfont->render(320, 300, messagebutton, true, renderer);
        }

    }
    SDL_RenderPresent(renderer);

}

void titlescreen::logic(double deltatime)
{
    background[bgnum].logic(deltatime);
}

int titlescreen::endlogic()
{
    if (loadgame) {
        return 1;
    }
	return 0;
}

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
void titlescreen::renderfont(int x, int y, std::string strg, bool selected, TTF_Font* size) {
    SDL_Surface* text;
    SDL_Color color = { 255, 255, 0 };
    if (!selected) {
        color = { 255, 255, 255 };
    }
    text = TTF_RenderText_Solid(size, strg.c_str(), color);
    SDL_Texture* words = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);
    drawTexture(words, x, y, 0, 1.0, true);
    SDL_DestroyTexture(words);

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