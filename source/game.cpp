#include "game.h"
#include "tetriminos.h"
#include "ghostblock.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include <SDL2/SDL.h>
#include <cmath>
#include "background.h"
#include <SDL2/SDL_ttf.h>
#include "ingamemessagebox.h"
#include "server.h"
#include <random>
#include "font.h"

#define LINES 0
#define LEVEL 1

#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif

//TODO: fix the stupid bug here lol
//TODO: also, fix ingamemessagebox.h
//TODO: take a shower
game::game(SDL_Renderer* renderman, SDL_Window* window, std::vector<SDL_Texture*> texture, std::vector<bg>  backg, Mix_Music* musicVec[], Mix_Chunk* soundVec[], std::vector<font*> fonts) {
    //srand((unsigned)time(0)); 
    time = std::time(nullptr);
    std::fill_n(testblocks, 200, 0);
    std::fill_n(ghostblocks, 200, 0);
    std::fill_n(previousblocks, 200, 0);
    renderer = renderman;
    textures = texture;
    t = tetrimino(2, 0, testblocks, 10, 20, 0);
    g = ghostblock(2, 0, previousblocks, 10, 20, 0, ghostblocks);
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
    std::string path = filepath "sprites/00.ttf";
    bodyfont = fonts.at(2);
    header = fonts.at(1);
    msg.letterfont = bodyfont;

}
void game::logic(double deltatime) {
    if (gameactive && !paused) {
        if (realtick % 100 == 0) {
            score++;
            t.movedown();
        }
        ticks += deltatime;
        if (ticks >= 1) {
            ticks = 0;
            realtick++;
        }
        msg.logic(deltatime);
    }
    backgrounds[(level)%(backgrounds.size())].logic(deltatime);

}
void game::render() {
    //if (gameactive) {
        SDL_RenderClear(renderer);
        backgrounds[(level)%(backgrounds.size())].render(renderer,false);
        SDL_RenderCopy(renderer, textures.at(0), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        g.changePos(t.x, t.y, t.rot);
        t.draw();
        g.draw();
        drawCubes(ghostblocks, testangles, ghostscale, 240, 80, 200, 10, textures, renderer);
        drawCubes(testblocks, testangles, testscale, 240, 80, 200, 10, textures, renderer);
        if(nextblocks > -1 && nextblocks < 7) {
            drawCubes(t.Pieces[nextblocks][0], testangles, testscale, 512, 48, 16, 4, textures, renderer);
        }
        if (holdblock > -1) {
            drawCubes(t.Pieces[holdblock][0], testangles, testscale, 64, 48, 16, 4, textures, renderer);
        }
        backgrounds[(level) % (backgrounds.size())].render(renderer, true);

        bodyfont->render(320, 32, "LN: " + std::to_string(lines) + " LV: " + std::to_string(level), true, renderer);
        bodyfont->render(320, 48, "SCORE: " + std::to_string(score),true, renderer);
        if(paused) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_Rect splashbox = { 0, 0, 640, 480 };
                SDL_RenderFillRect(renderer, &splashbox);
                for (int i = 0; i < optionsize; i++) {
                    bodyfont->render(renderer, options[i], 320, 300 + (i * 12), true, 255, (i == pauseselection?0:255), 255);
                }

            header->render(320, 240, "GAME PAUSED", true, renderer);
        }
        msg.render(renderer);
        SDL_RenderPresent(renderer);
    //}
}
int game::endlogic() {
    
    if (!t.alive && gameactive && !paused) {
        Mix_PlayChannel( -1, sound[5], 0 );
        ticks = 0;
        realtick = 0;
        checkLines(testblocks);
        memcpy(previousblocks, testblocks, sizeof previousblocks);
    
        if (!t.rebirth(2, 0, nextblocks)) {
            gameactive = false;
            return 1;
        }
        g.rebirth(2, 0, t.piece, previousblocks);
        std::fill_n(ghostblocks, 200, 0);
        nextblocks = rand() % 7;
        return 2;
    }
    return 0;
}
void game::keyPressed(SDL_Keycode key)
{
    if (gameactive && !paused) {
        switch (key) {
        case SDLK_UP: {
            Mix_PlayChannel( -1, sound[3], 0 );
            score+= t.forcedrop();
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
            Mix_PlayChannel( -1, sound[4], 0 );
            checkLines(testblocks);
            t.removeolddraw();
            memcpy(previousblocks, testblocks, sizeof previousblocks);
            int temp = holdblock;
            ticks = 0;
            realtick = 0;
            holdblock = t.piece;
            if (temp > -1) {
                t.rebirth(2, 0, temp);
            }
            else {
                t.rebirth(2, 0, nextblocks);
                nextblocks = rand() % 7;
            }
            g.rebirth(2, 0, t.piece, previousblocks);
            std::fill_n(ghostblocks, 200, 0);
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

void game::checkLines(int(blocks)[200]) {
    int times = 0;
    for (int i = 0; i < 20; i++) {
        int temp[10];
        for (int j = 0; j < 10; j++) {
            temp[j] = blocks[i * 10 + j];
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

void game::clearRow(int(blocks)[200], int y) {
    int newarray[200];
    std::fill_n(newarray, 200, 0);
    for (int j = 0; j < 10; j++) {
        blocks[(y * 10) + j] = 0;
    }
    for (int j = 0; j < (y * 10); j++) {
        newarray[j + 10] = blocks[j];
    }
    for (int j = (y * 10) + 10; j < 200; j++) {
        newarray[j] = blocks[j];
    }
    memcpy(blocks, newarray, sizeof newarray);
    //shiftarray(blocks, 200, -10);

}
void game::changemusic() {
    if((level)%(backgrounds.size()) != currentsong) {
        Mix_HaltMusic();
        if( Mix_PlayingMusic() == 0 )
        {
            //Play the music
            Mix_PlayMusic(backgrounds[(level)%(backgrounds.size())].music, -1 );
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
        currentsong = (level)%(backgrounds.size());
        msg.activate("YOU ARE CURRENTLY LISTENING TO:", backgrounds[(level)%(backgrounds.size())].songname + " by: " + backgrounds[(level)%(backgrounds.size())].artist);

    }

}
void game::reset() {
    time = std::time(nullptr);
    currentsong = -1;
    changemusic();
    std::fill_n(testblocks, 200, 0);
    std::fill_n(testangles, 200, 0);
    std::fill_n(testscale, 200, 1);
    std::fill_n(ghostblocks, 200, 0);
    std::fill_n(ghostscale, 200, 0.5);
    std::fill_n(previousblocks, 200, 0);
    t = tetrimino(2, 0, testblocks, 10, 20, 0);
    g = ghostblock(2, 0, previousblocks, 10, 20, 0, ghostblocks);
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

void game::renderfont(int x, int y, std::string strg, bool selected, TTF_Font* size) {
    SDL_Surface* text;
    SDL_Color color = { 255, 255, 0 };
    if (!selected) {
        color = { 255, 255, 255 };
    }
    text = TTF_RenderText_Solid(size, strg.c_str(), color);
    SDL_Texture* words = SDL_CreateTextureFromSurface(renderer, text);
    SDL_FreeSurface(text);
    drawTexture(renderer, words, x, y, 0, 1.0,true);
    SDL_DestroyTexture(words);

}