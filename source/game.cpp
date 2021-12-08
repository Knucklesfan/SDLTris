#include "game.h"
#include "tetriminos.h"
#include "ghostblock.h"
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include <SDL2/SDL.h>
#include <cmath>
#include "background.h"
#include "ingamemessagebox.h"
#include "server.h"
#include <random>
#include <ctime>

#include "font.h"

#define LINES 0
#define LEVEL 1
#define BLOCKY 0
#define BLOCKX 2
#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif

//TODO: fix the stupid bug here lol
//TODO: also, fix ingamemessagebox.h
//TODO: take a shower
game::game(SDL_Renderer* renderman, SDL_Window* window, std::vector<SDL_Texture*> textureb, std::vector<bg>  backg, Mix_Music* musicVec[], Mix_Chunk* soundVec[], std::vector<font*> fonts, int(active)[4][6]) {
    //srand((unsigned)time(0)); 
    texture = SDL_CreateTexture(renderman,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,640,480);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    memcpy(activations, active, sizeof activations);
    //volume = Mix_VolumeMusic(-1);
    time = std::time(nullptr);
    std::fill_n(testblocks, 240, 0);
    std::fill_n(ghostblocks, 240, 0);
    std::fill_n(previousblocks, 240, 0);
    renderer = renderman;
    textures = textureb;
    t = tetrimino(BLOCKX, BLOCKY, testblocks, 10, 24, 0);
    g = ghostblock(BLOCKX, BLOCKY, previousblocks, 10, 24, 0, ghostblocks);
    g.changePos(0, 0, 0);
    double ticks = 0;
    int realtick = 0;
    //int nextblocks[16];
    nextblocks = std::rand() % 7;
    int holdblock = -1;
    music = musicVec;
    sound = soundVec;
    gameactive = false;
    backgrounds = backg;
    lines = LINES;
    level = LEVEL;
    paused = false;
    bodyfont = fonts.at(2);
    header = fonts.at(1);
    msg = new ingamemessagebox("null","null",renderer, textures, bodyfont, 0);

}
void game::logic(double deltatime) {
    if (gameactive && !paused) {
        if (fmod(realtick, getspeed()) == 0) {
            score++;
            t.movedown();
        }
        ticks += deltatime;
        if (ticks >= 1) {
            ticks = 0;
            realtick++;
        }
        msg->logic(deltatime);
        visiblelifetime += deltatime;

        if(activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::ROTATEBOARD]) {
            rotval = visiblelifetime/25;
        }
    }
    if (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::MOVINGBG] == 1) {
        backgrounds[(bglevel) % (backgrounds.size())].logic(deltatime);
    }
    
    //std::cout << bglevel << "\n";
    if (warningflag) {
        //Mix_VolumeMusic(volume/5);
        alphalifetime += deltatime / 5;
        if (warningalpha < 1.0 && godown) {
            warningalpha += deltatime / 750;
        }
        else if (warningalpha >= 1.0 && godown) {
            godown = false;
            warningalpha = 1.0;
            alphalifetime = 0;
            Mix_PlayChannel(-1, sound[9], 0);

        }
        if (warningalpha > 0 && goup) {
            warningalpha -= deltatime / 750;
        }
        else if (warningalpha <= 0 && goup) {
            warningalpha = 0;
            alphalifetime = 0;
            godown = true;
            goup = false;

        }

        if ((!godown && !goup) && alphalifetime > 250) {
            goup = true;
        }

    }
    else {
        warningalpha = 0;

    }
    if (activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::BLINDMODE]) {
        visibility = (sin(visiblelifetime/5000)+1)/2;
    }
    else {
        visibility = 1.0;

    }

    for (int i = 0; i < 24; i++) {
        if (lineclears[i] > 0.0) {
            lineclears[i] -= 0.05;

        }
    }


}
void game::render() {
    //if (gameactive) {
        SDL_RenderClear(renderer);
        backgrounds[(bglevel)%(backgrounds.size())].render(renderer,false);
        if (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::NEARTOPFLASH] == 1) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128 * warningalpha);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);
        }
        
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderClear(renderer);
        
        SDL_RenderCopy(renderer, textures.at(9), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??

        g.changePos(t.x, t.y, t.rot);
        t.draw();
        g.draw();
        if (activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::GHOSTPIECE] == 1) {
            drawCubes(ghostblocks, testangles, ghostscale, 240, 16, 240, 10, textures, renderer);
        }
        drawCubes(testblocks, testangles, testscale, 240, 16, 240, 10, textures, renderer);
        //std::cout << "LINECLEARS\n";
        if (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::LINECLEAR] == 1) {
            for (int i = 0; i < 24; i++) {
                if (lineclears[i] > 0) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255 * lineclears[i]);
                    SDL_Rect splashbox = { 240, 16 + i * 16, 160, 16 };
                    SDL_RenderFillRect(renderer, &splashbox);
                }
                //std::cout << lineclears[i] << "\n";

            }
        }
        SDL_RenderCopy(renderer, textures.at(0), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_SetRenderTarget(renderer, NULL);
        drawTexture(renderer, texture, 0, 0, rotval, visibility, false);

        
        
        SDL_RenderCopy(renderer, textures.at(10), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        if(nextblocks > -1 && nextblocks < 7) {
            drawCubes(t.Pieces[nextblocks][0], testangles, testscale, 512, 48, 16, 4, textures, renderer);
        }
        if (holdblock > -1) {
            drawCubes(t.Pieces[holdblock][0], testangles, testscale, 64, 48, 16, 4, textures, renderer);
        }

        backgrounds[(bglevel) % (backgrounds.size())].render(renderer, true);
        bodyfont->render(320, 32, "LN: " + std::to_string(lines) + " LV: " + std::to_string(level), true, renderer);
        bodyfont->render(320, 48, "SCORE: " + std::to_string(score),true, renderer);
        if(paused) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_Rect splashbox = { 0, 0, 640, 480 };
                SDL_RenderFillRect(renderer, &splashbox);
                for (int i = 0; i < optionsize; i++) {
                    bodyfont->render(renderer, choices[i], 320, 300 + (i * 12), true, 255, (i == pauseselection?0:255), 255);
                }

            header->render(320, 240, "GAME PAUSED", true, renderer);
        }
        msg->render(renderer);
        SDL_RenderPresent(renderer);
    //}
}
int game::endlogic() {
    
    if (!t.alive && gameactive && !paused) {
        std::cout << "block not alive!!!";
        Mix_PlayChannel( -1, sound[5], 0 );
        ticks = 0;
        realtick = 0;
        //Mix_VolumeMusic(volume);
        warningflag = false;
        checkLines(testblocks);
        memcpy(previousblocks, testblocks, sizeof previousblocks);
        
        if (!t.rebirth(BLOCKX, BLOCKY, nextblocks)) {
            gameactive = false;
            return 1;
        }
        g.rebirth(BLOCKX, BLOCKY, t.piece, previousblocks);
        std::fill_n(ghostblocks, 240, 0);
        nextblocks = rand() % 7;

        return 2;
    }
    return 0;
}
void game::keyPressed(SDL_Keycode key)
{
    if (gameactive && !paused) {
        t.draw();
        switch (key) {
        case SDLK_UP: {
            Mix_PlayChannel( -1, sound[3], 0 );
            if (activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::FASTDROP] == 1) {
                score += t.forcedrop();
            }
            break;
        }
        case SDLK_DOWN: {
            Mix_PlayChannel( -1, sound[2], 0 );
            t.movedown();
            score++;
            break;
        }
        case SDLK_LEFT: {
            Mix_PlayChannel( -1, sound[2], 0 );
            t.moveleft();
            break;
        }
        case SDLK_RIGHT: {
            Mix_PlayChannel( -1, sound[2], 0 );
            t.moveright();
            break;
        }
        case SDLK_z: {
            Mix_PlayChannel( -1, sound[1], 0 );
            t.rotate();
            break;
        }
        case SDLK_x: {
            if (activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::HOLDPIECE] == 1) {
                Mix_PlayChannel(-1, sound[4], 0);
                //checkLines(testblocks);
                t.draw();
                t.removeolddraw();

                memcpy(previousblocks, testblocks, sizeof previousblocks);
                int temp = holdblock;
                holdblock = t.piece;
                if (temp > -1) {
                    t.rebirth(BLOCKX, BLOCKY, temp);
                }
                else {
                    t.rebirth(BLOCKX, BLOCKY, nextblocks);
                    nextblocks = rand() % 7;
                }
                g.rebirth(BLOCKX, BLOCKY, t.piece, previousblocks);
                std::fill_n(ghostblocks, 240, 0);
                ticks = 0;
                realtick = 0;
            }
            break;
        }
        }
    }
    else {
        switch (key) {
            case(SDLK_UP): {
                if (pauseselection > 0) {
                    pauseselection = (pauseselection - 1);
                }
                Mix_PlayChannel( -1, sound[1], 0 );
                break;
            }
            case(SDLK_DOWN): {
                if (pauseselection < optionsize - 1) {
                    pauseselection = (pauseselection + 1);
                }
                Mix_PlayChannel( -1, sound[1], 0 );
                break;
            }
            case(SDLK_z): {
                switch(pauseselection) {
                    case 0: {
                        if (Mix_PausedMusic() == 1)
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
                        paused = false;
                        break;
                    }
                    case 1: {
                        gameactive = false;
                    }
                }
            }
        }

    }
    if(key == SDLK_ESCAPE) {
        if (Mix_PausedMusic() == 1)
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
        paused = !paused;
    }
}
void game::shiftarray(int(array)[], int size, int shift) {
    for (int i = 0; i < abs(shift); i++) {
        std::vector<int> newarray(size);
        if (shift < 0) { //negative
            for (int i = 0; i < size - 1; i++) {
                newarray[i + 1] = array[i];
            }
            newarray[0] = array[size - 1];
        }
        else { //positive
            for (int i = 1; i < size; i++) {
                newarray[i - 1] = array[i];
            }
            newarray[size - 1] = array[0];
        }

        std::memcpy(array, newarray.data(), size);
    }
}

void game::checkLines(int(blocks)[240]) {
    int times = 0;
    for (int i = 0; i < 24; i++) {
        int temp[10];
        for (int j = 0; j < 10; j++) {
            temp[j] = blocks[i * 10 + j];
            if (i < 8 && temp[j] > 0) {
                if (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::NEARTOPFLASH] == 1) {
                    warningflag = true;
                }
            }
        }
        if (checkRow(temp)) {
            clearRow(blocks, i);
            times++;
        }
    }
    if(times == 1) {
        Mix_PlayChannel( -1, sound[6], 0 );
        score += 100 * level;
    } else if(times == 2) {
        Mix_PlayChannel( -1, sound[7], 0 );
        score += 300 * level;

    }
    else if (times == 3) {
        Mix_PlayChannel(-1, sound[7], 0);
        score += 500 * level;
    }
    else if(times >= 4) {
        Mix_PlayChannel( -1, sound[8], 0 );
        score += 800 * level;
    }
    lines += times;
    if (times > 0) {
        level = (lines / 10) + 1;
    }
    if (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::BGMODE] == 1) {
        bglevel = activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + level-1;
    }
    changemusic();
}
bool game::checkRow(int(blocks)[10]) {
    for (int i = 0; i < 10; i++) {
        if (blocks[i] == 0) {
            return false;
        }
    }
    return true;
}

void game::clearRow(int(blocks)[240], int y) {
    int newarray[240];
    std::fill_n(newarray, 240, 0);
    for (int j = 0; j < 10; j++) {
        blocks[(y * 10) + j] = 0;
    }
    for (int j = 0; j < (y * 10); j++) {
        newarray[j + 10] = blocks[j];
    }
    for (int j = (y * 10) + 10; j < 240; j++) {
        newarray[j] = blocks[j];
    }
    memcpy(blocks, newarray, sizeof newarray);
    lineclears[y] = 1.0;
    std::cout << "CLEARED LINE" << "\n";
    //shiftarray(blocks, 240, -10);

}
void game::changemusic() {
    if((bglevel)%(backgrounds.size()) != currentsong) {
        Mix_HaltMusic();
        if( Mix_PlayingMusic() == 0 )
        {
            //Play the music
            Mix_PlayMusic(backgrounds[(bglevel)%(backgrounds.size())].music, -1 );
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
        currentsong = (bglevel)%(backgrounds.size());
        msg->activate("YOU ARE CURRENTLY LISTENING TO:", backgrounds[(bglevel)%(backgrounds.size())].songname + " by: " + backgrounds[(bglevel)%(backgrounds.size())].artist);

    }

}
void game::reset() {
    time = std::time(nullptr);
    currentsong = -1;
    bglevel = activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG];
    changemusic();
    std::fill_n(testblocks, 240, 0);
    std::fill_n(testangles, 240, 0);
    std::fill_n(testscale, 240, 1);
    std::fill_n(ghostblocks, 240, 0);
    std::fill_n(ghostscale, 240, 0.5);
    std::fill_n(previousblocks, 240, 0);
    t = tetrimino(BLOCKX, BLOCKY, testblocks, 10, 24, 0);
    g = ghostblock(BLOCKX, BLOCKY, previousblocks, 10, 24, 0, ghostblocks);
    g.changePos(0, 0, 0);
    double ticks = 0;
    int realtick = 0;
    int holdblock = -1;
    lines = LINES;
    level = LEVEL;
    paused = false;
    nextblocks = std::rand() % 7;
    //srand((unsigned)time(0)); 
    gameactive = true;


}

void game::drawCubes(int position[], double angles[], double scale[], int x, int y, int size, int width, std::vector<SDL_Texture*> textures, SDL_Renderer* renderer) {
    
    for (int i = 0; i < size; i++) {
        if (position[i] > 0) {
            drawTexture(renderer, textures.at(position[i]), x + (i % width) * 16, y + (i / width) * 16, angles[i], scale[i]);
        }
    }
}

void game::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale) {
    SDL_Rect sprite;
    SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h);
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;

    sprite.x = x + oldwidth / 2 - sprite.w / 2;
    sprite.y = y + oldheight / 2 - sprite.h / 2;
    SDL_RenderCopyEx(renderer, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}
void game::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
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
//DISGUSTING CODE AHEAD
//most of these were infered based on the table stored in the NES version of tetris. I decided just to wing it and use those numbers instead of just making an array.
//...i'm stupid
double game::getspeed() {
    double returndb;
    if(activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::BLOCKSPEED] == 1) {
        switch (level) {
            case 1: {
                returndb = 8;
                break;
            }
            case 2: {
                returndb = 7.5;
                break;
            }
            case 3: {
                returndb = 7;
                break;
            }
            case 4: {
                returndb = 6.5;
                break;
            }
            case 5: {
                returndb = 6.25;
                break;
            }
            case 6: {
                returndb = 6;
                break;
            }
            case 7: {
                returndb = 5.75;
                break;
            }
            case 8: {
                returndb = 5.5;
                break;
            }
            case 9: {
                returndb = 5;
                break;
            }
            case 10:
            case 11:
            case 12: {
                returndb = 4.5;
                break;
            }
            case 13:
            case 14:
            case 15: {
                returndb = 4;
                break;
            }
            case 16:
            case 17:
            case 18: {
                returndb = 3;
                break;
            }
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28: {
                returndb = 2;
                break;
            }
            default: {
                returndb = 1;
                break;
            }
            }





    }
    else {
        returndb = 8;
    }
    return returndb * 12.5;
}