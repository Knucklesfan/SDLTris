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

game::game(SDL_Renderer* renderman, SDL_Window* window, std::vector<SDL_Texture*> texture, Mix_Music* musicVec[], Mix_Chunk* soundVec[]) {
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
    int nextblocks[16];
    int holdblock = -1;
    for (int i = 0; i < 16; i++) {
        nextblocks[i] = rand() % 7;
    }
    music = musicVec;
    sound = soundVec;
    gameactive = false;


}
void game::logic(double deltatime) {
    if (gameactive) {
        if (realtick % 100 == 0) {
            t.movedown();
        }
        ticks += deltatime;
        if (ticks >= 1) {
            ticks = 0;
            realtick++;
        }
        layerpos[0] -= (deltatime)/2.5;
        layerpos[1] -= (deltatime) / 5;
        layerpos[2] -= (deltatime) / 10;
        layerpos[3] -= (deltatime) / 15;


    }

}
void game::render() {
    if (gameactive) {
        SDL_RenderClear(renderer);
        drawTexture(renderer,textures[10], fmod(layerpos[4], 640) + 0, 0, 0.0, 1.0);
        drawTexture(renderer,textures[10], fmod(layerpos[4], 640) + 640, 0, 0.0, 1.0);
        drawTexture(renderer,textures[9], fmod(layerpos[3], 640) + 0, 0, 0.0, 1.0);
        drawTexture(renderer,textures[9], fmod(layerpos[3], 640) + 640, 0, 0.0, 1.0);
        drawTexture(renderer,textures[8], fmod(layerpos[2], 640) + 0, 0, 0.0, 1.0);
        drawTexture(renderer,textures[8], fmod(layerpos[2], 640) + 640, 0, 0.0, 1.0);
        drawTexture(renderer,textures[7], fmod(layerpos[1], 640) + 0, 0, 0.0, 1.0);
        drawTexture(renderer,textures[7], fmod(layerpos[1], 640) + 640, 0, 0.0, 1.0);
        drawTexture(renderer,textures[6], fmod(layerpos[0],640)+0, 0, 0.0, 1.0);
        drawTexture(renderer,textures[6], fmod(layerpos[0], 640)+640, 0, 0.0, 1.0);

        SDL_RenderCopy(renderer, textures.at(0), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        g.changePos(t.x, t.y, t.rot);
        t.draw();
        g.draw();
        drawCubes(ghostblocks, testangles, ghostscale, 240, 80, 200, 10, textures, renderer);
        drawCubes(testblocks, testangles, testscale, 240, 80, 200, 10, textures, renderer);
        drawCubes(t.Pieces[nextblocks[0]][0], testangles, testscale, 512, 48, 16, 4, textures, renderer);
        if (holdblock > -1) {
            drawCubes(t.Pieces[holdblock][0], testangles, testscale, 64, 48, 16, 4, textures, renderer);
        }
        SDL_RenderPresent(renderer);
    }
}
int game::endlogic() {
    if (!t.alive && gameactive) {
        Mix_PlayChannel( -1, sound[5], 0 );
        ticks = 0;
        realtick = 0;
        checkLines(testblocks);
        memcpy(previousblocks, testblocks, sizeof previousblocks);
        if (!t.rebirth(2, 0, nextblocks[0])) {
            gameactive = false;
            return 1;
        }
        g.rebirth(2, 0, t.piece, previousblocks);
        std::fill_n(ghostblocks, 200, 0);
        shiftarray(nextblocks, 16, -1);
        nextblocks[15] = rand() % 7;
    }
    return 0;
}
void game::keyPressed(SDL_Keycode key)
{
    if (gameactive) {
        switch (key) {
        case SDLK_UP: {
            Mix_PlayChannel( -1, sound[3], 0 );
            t.forcedrop();
            break;
        }
        case SDLK_DOWN: {
            Mix_PlayChannel( -1, sound[2], 0 );
            t.movedown();
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
                t.rebirth(0, 0, temp);
            }
            else {
                t.rebirth(5, 0, nextblocks[0]);
                shiftarray(nextblocks, 16, -1);
                nextblocks[15] = rand() % 7;
            }
            g.rebirth(5, 0, t.piece, previousblocks);
            std::fill_n(ghostblocks, 200, 0);
            break;
        }

        }
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
            printf("FULL ROW DETECTED. %i", i);
            clearRow(blocks, i);
            times++;
        }
    }
    if(times == 1) {
        Mix_PlayChannel( -1, sound[6], 0 );
    } else if(times > 1 && times < 4) {
        Mix_PlayChannel( -1, sound[7], 0 );
    } else if(times >= 4) {
        Mix_PlayChannel( -1, sound[8], 0 );
    }
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
void game::reset() {
    Mix_HaltMusic();
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
    int nextblocks[16];
    int holdblock = -1;
    for (int i = 0; i < 16; i++) {
        nextblocks[i] = rand() % 7;
    }
    gameactive = true;
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( music[1], -1 );
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

