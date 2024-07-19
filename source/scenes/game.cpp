#include "game.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <cstring>
#include <SDL2/SDL.h>
#include <cmath>
#include <ctime>
#include "../utils/defs.h"
#include "SDL2/SDL_stdinc.h"

#define LINES 0
#define LEVEL 1
#define BLOCKY 2
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
game::game() {

    #ifdef __LEGACY_RENDER
    texture = SDL_CreateTexture(graphics::render,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,640,480);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    #else
        playfield = new buffermanager(640,480,true);
        cubeRenderer = new cube(glm::vec3(0.0f,0.0f,-24.0f),glm::vec3(0.0f,0.0f,0.0f),
	glm::vec3(1.0f,1.0f,1.0f),glm::vec3((-0.0f), 0.0f, 0.0f));

    #endif
    // memcpy(activations, active, sizeof activations);
    //volume = Mix_VolumeMusic(-1);
    time = std::time(nullptr);
    srand((unsigned)time); 

    std::fill_n(testblocks, 480, 0);
    std::fill_n(ghostblocks, 480, 0);
    std::fill_n(previousblocks, 480, 0);
    std::fill_n(keyboardname, 8, 0);
    currentChar = 0;
    selectedkey = 0;

    // renderer = renderman;
    // textures = textureb;
    if(settings::activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::BIGGERBOARD]) {
        boardwidth = 20;
    }
    t = tetrimino(BLOCKX, BLOCKY, testblocks, boardwidth, boardheight, 0);
    g = ghostblock(BLOCKX, BLOCKY, previousblocks, boardwidth, boardheight, 0, ghostblocks);
    g.changePos(0, 0, 0);
    ticks = 0;
    realtick = 0;
    //int nextblocks[16];
    toad = new model("models/toad.kmf",{0,-6,-6},{1,1,1},{0,0,0});
    std::srand(time+randomIters);
    nextblocks = std::rand() % 7;

    randomIters++;
    holdblock = -1;
    // music = musicVec;
    // sound = soundVec;
    // backgrounds = backg;
    lines = LINES;
    level = LEVEL;
    paused = false;
    bodyfont = graphics::fonts->at(2);
    header = graphics::fonts->at(1);
    msg = new ingamemessagebox("null","null", 0);
    gameactive = true;
    invisScore = 0;

}
void game::logic(double deltatime) {

    if(demoPlayback) {
        if(demotick != 0 && demotick != UINT32_MAX && demokey != SDLK_ESCAPE) {
            if(SDL_GetTicks()-gameStart >= demotick) {
                if(demokey == SDLK_DELETE) { //if it set to delete, then we can start fading
                    demoReturn = true;
                }
                inputKey(demokey);
                memcpy(&demotick, demo+demoOffset, sizeof(Uint32)); 
                demoOffset += sizeof(Uint32);
                memcpy(&demokey, demo+demoOffset, sizeof(SDL_Keycode)); 
                demoOffset += sizeof(SDL_Keycode);
            }
        }
        else {
            if(demotick == UINT32_MAX || demokey == SDLK_ESCAPE) {
                demoPlayback = false;
            }
            else {
                memcpy(&demotick, demo+demoOffset, sizeof(Uint32)); 
                demoOffset += sizeof(Uint32);
                memcpy(&demokey, demo+demoOffset, sizeof(SDL_Keycode)); 
                demoOffset += sizeof(SDL_Keycode);
            }
        }

    }
    // graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).renderLyrics();
    if (gameactive && !paused) {
        if (fmod(realtick, getspeed()) == 0) {
            invisScore++; //increment the drop score, but dont add it yet

            t.movedown();
            
        }
        ticks += deltatime;
        if (ticks >= 1) {
            ticks = 0;
            realtick++;
        }
        msg->logic(deltatime);
        visiblelifetime += deltatime;

        if(settings::activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::ROTATEBOARD]) {
            rotval = visiblelifetime/25;
        }
        if(!demoPlayback) {
            networking::globalRPC->update("Playing a game", "Current score: " + std::to_string(score), "icon2", startTime);
        }
        g.changePos(t.x, t.y, t.rot);
        g.draw();
        t.draw();
    }
    if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::MOVINGBG] == 1) {
        graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).logic(deltatime);
    }
    
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
            
            Mix_PlayChannel(-1, audio::sfx->at(9), 0);

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
    if(warningalpha < 0.0) {
        warningalpha = 0.0;
    }
    if(warningalpha > 1.0) {
        warningalpha = 1.0;
    }

    if (settings::activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::BLINDMODE]) {
        visibility = (sin(visiblelifetime/5000)+1)/2;
    }
    else {
        visibility = 1.0;

    }

    for (int i = 0; i < boardheight; i++) {
        if (lineclears[i] > 0.0) {
            lineclears[i] -= 0.05;

        }
    }


}
void game::render() {

    //if (gameactive) {
        #ifdef __LEGACY_RENDER
        SDL_RenderClear(graphics::render);
        #endif
        // graphics::backgrounds[(bglevel)%(backgrounds.size())].render(renderer,false);
        #ifdef __LEGACY_RENDER
        if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::NEARTOPFLASH] == 1) {
            SDL_SetRenderDrawColor(graphics::render, 255, 0, 0, 128 * warningalpha);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(graphics::render, &splashbox);
            
        }
        SDL_Texture* temp = SDL_GetRenderTarget(graphics::render);
        SDL_SetRenderTarget(graphics::render, texture);
        SDL_RenderClear(graphics::render);
        if(boardwidth > 10) {
            SDL_RenderCopy(graphics::render, graphics::sprites.at("bbackdrop"), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        }
        else {
            SDL_RenderCopy(graphics::render, graphics::sprites.at("sbackdrop"), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        }
        g.changePos(t.x, t.y, t.rot);
        t.draw();
        g.draw();
        if (settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::GHOSTPIECE] == 1) {
            drawCubes(ghostblocks, 0.5, 320-boardwidth*8, 16, boardheight*boardwidth, boardwidth);
        }
        drawCubes(testblocks, 1.0, 320-boardwidth*8, 16, boardheight*boardwidth, boardwidth);
        //std::cout << "LINECLEARS\n";
        if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::LINECLEAR] == 1) {
            for (int i = 0; i < boardheight; i++) {
                if (lineclears[i] > 0) {
                    SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255 * lineclears[i]);
                    SDL_Rect splashbox = { 240, 16 + i * 16, 160, 16 };
                    SDL_RenderFillRect(graphics::render, &splashbox);
                }
                //std::cout << lineclears[i] << "\n";

            }
        }
        if(boardwidth > 10) {
            SDL_RenderCopy(graphics::render, graphics::sprites.at("bigstage"), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        }
        else {
            SDL_RenderCopy(graphics::render, graphics::sprites.at("stage"), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        }
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255);
        SDL_SetRenderTarget(graphics::render, temp);
        graphics::drawTexture(texture, 0, 0, rotval, visibility, false);

        
        
        SDL_RenderCopy(graphics::render, graphics::sprites.at("hold"), NULL, NULL); //its offically too late to be coding and yet... my code's working i think??
        if(nextblocks > -1 && nextblocks < 7) {
            drawCubes(t.Pieces[nextblocks][0], 1.0, 512, 48, 16, 4);
        }
        if (holdblock > -1) {
            drawCubes(t.Pieces[holdblock][0], 1.0, 64, 48, 16, 4);
        }

        graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).render(graphics::render,true);
        bodyfont->render(320, 32, "LN: " + std::to_string(lines) + " LV: " + std::to_string(level), true);
        bodyfont->render(320, 48, "SCORE: " + std::to_string(score),true);
        
        msg->render();

        if(paused) {
                SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
                SDL_Rect splashbox = { 0, 0, 640, 480 };
                SDL_RenderFillRect(graphics::render, &splashbox);
                for (int i = 0; i < optionsize; i++) {
                    bodyfont->render(320, 300 + (i * 12),choices[i],
                    true, 255, (i == pauseselection?0:255), 255,0,false,0,0,0);
                }

            header->render(320, 240, "GAME PAUSED", true);
        }
        #else
            playfield->enable();
            if(boardwidth > 10) {
                graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("bbackdrop"), {0,0}, {640,480},0,{0,0},{640,480}); //its offically too late to be coding and yet... my code's working i think??
            }
            else {
                graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("sbackdrop"), {0,0}, {640,480},0,{0,0},{640,480}); //its offically too late to be coding and yet... my code's working i think??
            }

            if (settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::GHOSTPIECE] == 1) {
                drawCubes(ghostblocks, 0.5, 320-boardwidth*8, 16, boardheight*boardwidth, boardwidth);
            }
            drawCubes(testblocks, 1.0, 320-boardwidth*8, 16, boardheight*boardwidth, boardwidth);
        if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::LINECLEAR] == 1) {
            for (int i = 0; i < boardheight; i++) {
                if (lineclears[i] > 0) {
                    graphics::rect->render(graphics::shaders.at(1),{240,16+i*16},{240+160,(16+i*16)+16},0,{1,1,1,lineclears[i]},false,-1,{0,0,0,0});
                }
                //std::cout << lineclears[i] << "\n";
            }
        }
        if(boardwidth > 10) {
            graphics::sprite->render(graphics::shaders.at(4), graphics::sprites.at("bigstage"), {0,0}, {640,480},0,{0,0},{640,480}); //its offically too late to be coding and yet... my code's working i think??
        }
        else {
            graphics::sprite->render(graphics::shaders.at(4), graphics::sprites.at("stage"), {0,0}, {640,480},0,{0,0},{640,480}); //its offically too late to be coding and yet... my code's working i think??
        }
            playfield->disable(640,480,true);
            graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).render();
            if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::NEARTOPFLASH] == 1) {
                graphics::rect->render(
                    graphics::shaders.at(1),
                    {0,0},
                    {640,480},0,{255,0,0,0.5*warningalpha},false,-1,glm::vec4(1,1,1,1));                
            }

            playfield->render(graphics::shaders.at(3),0,0,false);
            graphics::sprite->render(graphics::shaders.at(4), graphics::sprites.at("hold"), {0,0}, {640,480},0,{0,0},{640,480}); //its offically too late to be coding and yet... my code's working i think??
            
            if(nextblocks > -1 && nextblocks < 7) {
                drawCubes(gameplay::Pieces[nextblocks][0], 1.0, 10.45, 8.25, 16, 4,true,{0,sin(SDL_GetTicks()/1000.0f)*10,0});
            }
            if (holdblock > -1) {
                drawCubes(gameplay::Pieces[holdblock][0], 1.0, -10.45, 8.25, 16, 4,true,{0,sin(SDL_GetTicks()/1000.0f)*10,0});
            }

            bodyfont->render(320, 32, "LN: " + std::to_string(lines) + " LV: " + std::to_string(level), true);
            bodyfont->render(320, 48, "SCORE: " + std::to_string(score),true);
            
            msg->render();
            if(paused) {
                    graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{0,0,0,0.5},false,-1,{0,0,0,0});
                    for (int i = 0; i < optionsize; i++) {
                        bodyfont->render(320, 300 + (i * 12),choices[i],
                        true, 255, (i == pauseselection?0:255), 255,0,false,0,0,0);
                    }

                header->render(320, 240, "GAME PAUSED", true);
                if(keyboard) {
                    graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{0,0,0,0.5},false,-1,{0,0,0,0});
                    header->render(320,48, "Please name your save.",true);
                    for(int i = 0; i < FILENAME_LENGTH; i++) {
                        std::string str(1,keyboardname[i]);

                        // header->render(320-(16*4)+i*16,80, str,true);
                        header->render(320-(16*(FILENAME_LENGTH/2))+i*16,80,str,true,255,currentChar==i?0:255,255,-1,false,0,0,0);

                    }
                    for(int i = 0; i < 40; i++) {
                        std::string str(1,displayKeys[i]);
                        graphics::fonts->at(0)->render(200+(i%10)*24,160+(i/10)*32,str,true,255,selectedkey==i?96-64*sin(SDL_GetTicks()/500.0f):255,255,-1,false,0,0,0);
                    }
                }

            }
            if(demoRecord) {
                bodyfont->render(32, 32, "RECORDING DEMO", false,255,0,0,-1,false,0,0,0);
                if(demoReturn) {
                    bodyfont->render(32, 64, "DEMO IS FADING", false,255,255,0,-1,false,0,0,0);
                }
            }
        #endif
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
        glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
        view = glm::translate(view, glm::vec3(0.0, 0, -14.0)); 
        //SDL_RenderPresent(renderer);
    //}
}
Transition game::endLogic() {

    if (!t.alive && gameactive && !paused) {
        //std::cout << "block not alive!!!";
        Mix_PlayChannel(-1, audio::sfx->at(5), 0);
        ticks = 0;
        realtick = 0;
        //Mix_VolumeMusic(volume);
        warningflag = false;
        checkLines(testblocks);
        memcpy(previousblocks, testblocks, sizeof previousblocks);
        
        if (!t.rebirth(BLOCKX, BLOCKY, nextblocks)) {
            settings::lastlevel = level;
            settings::lastlines = lines;
            settings::lasttime = time;
            settings::previousscore = score;

            gameactive = false;
            return {
                0.001,
                gameplay::gamemode+1,
                320,
                240,
                FADETYPE::BLOCKS,
                true
            };
        }
        g.rebirth(BLOCKX, BLOCKY, t.piece, previousblocks);
        std::fill_n(ghostblocks, 480, 0);
        std::srand(time+randomIters);
        nextblocks = rand() % 7;
        randomIters++;
    }
    if(!gameactive) {
        warningflag = false;
        settings::lastlevel = level;
        settings::lastlines = lines;
        settings::lasttime = time;
        settings::previousscore = score;
        return {
            0.001,
            gameplay::gamemode+1,
            320,
            240,
            FADETYPE::BLOCKS,
            true
        };

    }
    return {
                0.001,
                gameplay::gamemode+1,
                320,
                240,
                FADETYPE::BLOCKS,
                false
            };
;
}
void game::input(SDL_Keycode key)
{
    if(!demoPlayback) {
        inputKey(key);
        if(demoRecord) {
            Uint32 tick = SDL_GetTicks()-gameStart;
            demofile.write((char *) &tick, sizeof(Uint32));
            demofile.write((char *) &key, sizeof(SDL_Keycode));
            if(key == SDLK_DELETE) {
                demoReturn = true; //sets this just to display on screen that the demo is transitioning
            }
        }
    }
}
void game::inputKey(SDL_Keycode key) {
    if (gameactive && !paused) {
        t.draw();
        switch (key) {
        case SDLK_UP: {
            Mix_PlayChannel(-1, audio::sfx->at(3), 0);
            if (settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::FASTDROP] == 1) {
                invisScore += t.forcedrop();
            }
            break;
        }
        case SDLK_DOWN: {
            Mix_PlayChannel(-1, audio::sfx->at(2), 0);
            t.movedown();
            invisScore++;

            break;
        }
        case SDLK_LEFT: {
            Mix_PlayChannel(-1, audio::sfx->at(2), 0);
            t.moveleft();
            break;
        }
        case SDLK_RIGHT: {
            Mix_PlayChannel(-1, audio::sfx->at(2), 0);
            t.moveright();
            break;
        }
        case SDLK_z: {
            Mix_PlayChannel(-1, audio::sfx->at(1), 0);
            t.rotate();
            break;
        }
        case SDLK_x: {
            if (settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::HOLDPIECE] == 1) {
                Mix_PlayChannel(-1, audio::sfx->at(4), 0);
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
                    std::srand(time+randomIters);
                    nextblocks = rand() % 7;
                    randomIters++;

                }
                g.rebirth(BLOCKX, BLOCKY, t.piece, previousblocks);
                std::fill_n(ghostblocks, 480, 0);
                ticks = 0;
                realtick = 0;
                invisScore = 0;
            }
            break;
        }
        case SDLK_y: {
            if (settings::globalDebug) {
                level--;
            }
            break;
        }
        case SDLK_u: {
            if (settings::globalDebug) {
                level++;
            }
            break;
        }
        case SDLK_m: {
            if (settings::globalDebug) {
                loadState(settings::saveload);
            }
            break;
        }

        }
    }
    else {
        if(paused && !keyboard) {
            switch (key) {
                case(SDLK_UP): {
                    if (pauseselection > 0) {
                        pauseselection = (pauseselection - 1);
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_DOWN): {
                    if (pauseselection < optionsize - 1) {
                        pauseselection = (pauseselection + 1);
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
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
                            paused = false;
                            break;
                        }
                        case 1: {
                            gameactive = false;
                        }break;
                        case 2: {
                            keyboard = true;
                        }break;
                        case 3: {
                        }break;
                        case 4: {
                        }break;
                        case 5: {
                        }break;
                    }
                }
            }
        } else
        if(keyboard) {
            switch(key) {
                case(SDLK_UP): {
                    if (selectedkey >= 10) {
                        selectedkey-=10;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_DOWN): {
                    if (selectedkey <= 30) {
                        selectedkey+=10;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_LEFT): {
                    if (selectedkey > 0) {
                        selectedkey--;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_RIGHT): {
                    if (selectedkey < 40) {
                        selectedkey++;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_z): {
                    if(selectedkey < 37) { //if we arent the two move keys, then we're fine to keep going
                        if(currentChar < 8) {
                            keyboardname[currentChar] = keyboardKeys[selectedkey];
                            currentChar++;
                        }
                    }
                    else { //otherwise, we gotta actually handle those...
                        if(selectedkey == 37) { //go back one key
                            if(currentChar > 0) {
                                currentChar--;
                            }
                        }
                        else if(selectedkey == 38) { //go forward
                            if(currentChar < 8) {
                                currentChar++;
                            }
                        }
                        else if(selectedkey == 39) {
                            saveState();
                            std::fill_n(keyboardname, 8, 0);
                            keyboard = false;
                            paused = false;
                            gameactive = false;
                            //INSERT SUBMISSION CODE HERE
                        }
                    }
                }

            }
        }

    }
    if(key == SDLK_ESCAPE) {
        if(demoRecord) {
            stopRecord();
        }
        if(keyboard) {
            keyboard = !keyboard;
            return;
        }
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

void game::checkLines(int(blocks)[480]) {
    int times = 0;
    for (int i = 0; i < boardheight; i++) {
        int* temp = new int[boardwidth];
        for (int j = 0; j < boardwidth; j++) {
            temp[j] = blocks[i * boardwidth + j];
            if (i < 8 && temp[j] > 0) {
                if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::NEARTOPFLASH] == 1) {
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
        Mix_PlayChannel(-1, audio::sfx->at(6), 0);
        score += 100 * level;
    } else if(times == 2) {
        Mix_PlayChannel(-1, audio::sfx->at(7), 0);
        score += 300 * level;

    }
    else if (times == 3) {
        Mix_PlayChannel(-1, audio::sfx->at(7), 0);
        score += 500 * level;
    }
    else if(times >= 4) {
        Mix_PlayChannel(-1, audio::sfx->at(8), 0);
        score += 800 * level;
    }
    lines += times;
    if (times > 0) {
        level = (lines / settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::LEVELLENGTH]) + 1;
    }
    score+= invisScore;
    invisScore = 0;
    if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::BGMODE] == 1) {
        bglevel = settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + level-1;
    }
    changemusic();
    if(!demoPlayback) {
        networking::globalRPC->update("Playing a game", "Current score: " + std::to_string(score), "icon2", startTime);
    }

}
bool game::checkRow(int* (blocks)) {
    for (int i = 0; i < boardwidth; i++) {
        if (blocks[i] == 0) {
            return false;
        }
    }
    return true;
}

void game::clearRow(int(blocks)[480], int y) {
    int newarray[480];
    std::fill_n(newarray, 480, 0);
    for (int j = 0; j < boardwidth; j++) {
        blocks[(y * boardwidth) + j] = 0;
    }
    for (int j = 0; j < (y * boardwidth); j++) {
        newarray[j + boardwidth] = blocks[j];
    }
    for (int j = (y * boardwidth) + boardwidth; j < 480; j++) {
        newarray[j] = blocks[j];
    }
    memcpy(blocks, newarray, sizeof newarray);
    lineclears[y] = 1.0;
    //std::cout << "CLEARED LINE" << "\n";
    //shiftarray(blocks, 240, -10);

}
void game::changemusic() {
    if((bglevel)%(graphics::backgrounds->size()) != currentsong && !demoPlayback) {
        Mix_HaltMusic();
        if( Mix_PlayingMusic() == 0 )
        {
            //Play the music
            Mix_PlayMusic(graphics::backgrounds->at((bglevel)%(graphics::backgrounds->size())).music, -1 );
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
        currentsong = (bglevel)%(graphics::backgrounds->size());
        msg->activate("YOU ARE CURRENTLY LISTENING TO:", graphics::backgrounds->at((bglevel)%(graphics::backgrounds->size())).songname + " by: " + graphics::backgrounds->at((bglevel)%(graphics::backgrounds->size())).artist);
        graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).backgroundAge = SDL_GetTicks();

    }

}
void game::reset() {

    currentsong = -1;
    bglevel = settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG];
    if(!demoPlayback) {
        changemusic();
        startTime = std::time(nullptr);
        networking::globalRPC->update("Playing a game", "Current score: " + std::to_string(score), "icon2", startTime);
    }
    
    std::fill_n(testblocks, 480, 0);
    std::fill_n(ghostblocks, 480, 0);
    std::fill_n(previousblocks, 480, 0);
    std::fill_n(keyboardname, 8, ' ');
    currentChar = 0;
    selectedkey = 0;
    score = 0;
    t = tetrimino(BLOCKX, BLOCKY, testblocks, boardwidth, boardheight, 0);
    g = ghostblock(BLOCKX, BLOCKY, previousblocks, boardwidth, boardheight, 0, ghostblocks);
    g.changePos(0, 0, 0);
    ticks = 0;
    realtick = 0;
    holdblock = -1;
    demoOffset = 0;
    demotick = 0;
    demokey = 0;
    lines = LINES;
    level = LEVEL;
    paused = false;
    srand((unsigned)time); 
    nextblocks = std::rand() % 7;
    randomIters++;
    time = std::time(nullptr);
    startTime = time;
    gameactive = true;
    warningflag = false;
    gameStart = SDL_GetTicks();
    invisScore = 0;
    if(settings::saveload != "") { //if we got a save to load
        loadState(settings::saveload); //go ahead and load our save
        settings::saveload = ""; //clear out saveload so we dont accidentally load again
    }


}
int game::demoEndLogic() {

    if(demoReturn) {
        demoReturn = false;
        return 1;
    }
    return 0;
}
void game::setupDemo(std::string demofile) {
    demoPlayback = true;
    reset();
    loadDemo(demofile);
}
void game::drawCubes(int position[], float scale, float x, float y, int size, int width, bool threed, glm::vec3 rotation) {
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    view = glm::translate(view, glm::vec3(0.0, 0, -6.0)); 
                cubeRenderer->position = {x,y,-20};

    for (int i = 0; i < size; i++) {
        if (position[i] > 0) {
            if(!threed) {
                if(graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).blockpack[0] == NULL) {
                    graphics::sprite->render(graphics::shaders[4],
                    graphics::blocks->at(position[i]-1),
                    {
                    (x + (i % width) * 16)+(16-(16*scale))/2,
                    (y + (int)(i / width) * 16)+(16-(16*scale))/2
                    },
                    {16*scale,16*scale}, 0,{0,0},{16,16});
                }
                else {
                    graphics::sprite->render(graphics::shaders[4],
                    graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).blockpack[(position[i]-1)],
                    {
                    (x + (i % width) * 16)+(16-(16*scale))/2,
                    (y + (int)(i / width) * 16)+(16-(16*scale))/2
                    },
                    {16*scale,16*scale}, 0,{0,0},{16,16});

                }
            }
            else { //handle 3d rendering of blocks
                glEnable(GL_DEPTH_TEST);  

                cubeRenderer->postposition = {
                    -width/2.0f+(i % width),
                    -(int)(size/width)/2.0f+(int)(i/width),
                    0
                    };
                cubeRenderer->rotation = rotation;
                if(graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).blockpack[0] == NULL) {
                    cubeRenderer->render(
                        graphics::shaders.at(0),
                        graphics::blocks->at(position[i]-1),
                        projection,view);
                }
                else {
                    cubeRenderer->render(
                        graphics::shaders.at(0),
                        graphics::backgrounds->at((bglevel) % (graphics::backgrounds->size())).blockpack[(position[i]-1)],
                        projection,view);

                }
                glDisable(GL_DEPTH_TEST);  

            }
        }
    }
}

//DISGUSTING CODE AHEAD
//most of these were infered based on the table stored in the NES version of tetris. I decided just to wing it and use those numbers instead of just making an array.
//...i'm stupid
double game::getspeed() {
    double returndb;
    if(settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::BLOCKSPEED]) {
        //std::cout << "Speed!!!\n";
        switch (level) {
            case 0: {
                returndb = 8.5;
                break;
            }
            case 1: {
                returndb = 8;
                break;
            }
            case 2:
            case 3: {
                returndb = 7;
                break;
            }
            case 4:
            case 5: {
                returndb = 6.5;
                break;
            }
            case 6: {
                returndb = 6;
                break;
            }
            case 7:
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
        returndb = 12;
    }
    return returndb * 6;
}
void game::startRecord() {
    t.removeolddraw();
    g.removeolddraw();
    gameStart = SDL_GetTicks();
    demofile = std::ofstream("demo.bin", std::ios::out | std::ios::binary);
    demofile.write((char *) &time, sizeof(uint));
    demofile.write((char *) &randomIters, sizeof(uint));
    demofile.write((char *) &t.piece, sizeof(int));
    demofile.write((char *) &t.x, sizeof(int));
    demofile.write((char *) &t.y, sizeof(int));
    demofile.write((char *) &t.rot, sizeof(int));

    demofile.write((char *) &nextblocks, sizeof(int));
    demofile.write((char *) &holdblock, sizeof(int));
    demofile.write((char *) &level, sizeof(int));
    demofile.write((char *) &lines, sizeof(int));

    demofile.write((char *) &testblocks, sizeof(int[480]));
    demoRecord = true;
    
}
void game::stopRecord() {
    Uint32 max = UINT32_MAX;
    SDL_Keycode nullkey = SDLK_UNKNOWN;
    demofile.write((char *) &max, sizeof(Uint32));
    demofile.write((char *) &nullkey, sizeof(SDL_Keycode));

    demoRecord = false;
    demofile.close();
}

void game::loadDemo(std::string filename) {
    t.removeolddraw();
    g.removeolddraw();
    demoOffset = 0;
    demotick = 0;
    demokey = 0;
    std::streampos size;
    std::cout << "loading demo" << filename <<"\n";
    std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        demo = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (demo, size);
        file.close();
        std::cout << "file opened demo\n";

        memcpy(&time, demo+demoOffset, sizeof(uint)); 
        demoOffset += sizeof(uint);
        memcpy(&randomIters, demo+demoOffset, sizeof(uint)); 
        demoOffset += sizeof(uint);
        memcpy(&t.piece, demo+demoOffset, sizeof(int)); 
        memcpy(&g.piece, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);
        memcpy(&t.x, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);
        memcpy(&t.y, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);
        memcpy(&t.rot, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);

        memcpy(&nextblocks, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);
        memcpy(&holdblock, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);

        memcpy(&level, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);
        memcpy(&lines, demo+demoOffset, sizeof(int)); 
        demoOffset += sizeof(int);

        memcpy(&testblocks, demo+demoOffset, sizeof(int[480])); 
        memcpy(&previousblocks, demo+demoOffset, sizeof(int[480])); 

        demoOffset += sizeof(int[480]);
        if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::BGMODE] == 1) {
            bglevel = settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + level-1;
        }
        gameStart = SDL_GetTicks();
        // changemusic();
        demoPlayback = true;
    }
    else {
        demoPlayback = false;
    }
}
void game::saveState() { //saves the game's state. this is a debug function im coding quickly to test replays. Might stick around, though...
    t.removeolddraw();
    g.removeolddraw();
    std::string name(keyboardname);
    int last = 8 - 1;
    while (last >= 0 && name[last] == 32)
        --last;
    name =  name.substr(0, last + 1);
    for(int i = 0; i < name.length(); i++) {
        std::cout << (int)name[i] << " ";
    }

    std::cout << "END\n";
    std::string filename = settings::saveDir+"/"+name+".knfs";
    std::cout << "Saving to file " << filename <<"\n";
    std::ofstream fs(filename, std::ios::out | std::ios::binary);
    Uint32 version = SAVE_VERSION; //the version of the current save type, in case i update this
    fs.write((char *) &version, sizeof(Uint32));
    fs.write((char *) &time, sizeof(uint));
    fs.write((char *) &randomIters, sizeof(uint));
    fs.write((char *) &t.piece, sizeof(int));
    fs.write((char *) &t.x, sizeof(int));
    fs.write((char *) &t.y, sizeof(int));
    fs.write((char *) &t.rot, sizeof(int));

    fs.write((char *) &nextblocks, sizeof(int));
    fs.write((char *) &holdblock, sizeof(int));
    fs.write((char *) &level, sizeof(int));
    fs.write((char *) &lines, sizeof(int));
    fs.write((char *) &score, sizeof(Uint32)); //added in version 2

    fs.write((char *) &gameStart, sizeof(Uint32));

    fs.write((char *) &testblocks, sizeof(int[480]));
    

    fs.close();
    

}
void game::loadState(std::string path) {
    t.removeolddraw();
    g.removeolddraw();
    
    std::streampos size;
    char * memblock;
    std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();

        std::cout << "the entire file content is in memory";
        Uint32 version = 0; //the version of the current save type, in case i update this

        size_t offset = 0;
        memcpy(&version, memblock+offset, sizeof(uint)); //loads the version number first, so we know what we're working with here
        offset += sizeof(Uint32);
        std::cout << "VERSION " << version << "\n";
        switch(version) {
            case 1: { //early alpha save files, doesn't record the player's score
                memcpy(&time, memblock+offset, sizeof(uint)); 
                offset += sizeof(uint); //game's current time variable
                memcpy(&randomIters, memblock+offset, sizeof(uint)); 
                offset += sizeof(uint); //the number of times the random seed has been iterated
                memcpy(&t.piece, memblock+offset, sizeof(int)); 
                memcpy(&g.piece, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current piece, also stored to the ghost pieces as well
                memcpy(&t.x, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current x coordinate of a piece
                memcpy(&t.y, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current y coordinate of a piece
                memcpy(&t.rot, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current rotation of a piece

                memcpy(&nextblocks, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the next block up
                memcpy(&holdblock, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the currently held block

                memcpy(&level, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the level the player was at
                memcpy(&lines, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //lines of the player

                memcpy(&gameStart, memblock+offset, sizeof(Uint32)); 
                offset += sizeof(Uint32); 

                memcpy(&testblocks, memblock+offset, sizeof(int[480])); 
                memcpy(&previousblocks, memblock+offset, sizeof(int[480])); 


            }break;
            case 2: { //version 2 adds score to be saved
                
                memcpy(&time, memblock+offset, sizeof(uint)); 
                offset += sizeof(uint); //game's current time variable
                memcpy(&randomIters, memblock+offset, sizeof(uint)); 
                offset += sizeof(uint); //the number of times the random seed has been iterated
                memcpy(&t.piece, memblock+offset, sizeof(int)); 
                memcpy(&g.piece, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current piece, also stored to the ghost pieces as well
                memcpy(&t.x, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current x coordinate of a piece
                memcpy(&t.y, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current y coordinate of a piece
                memcpy(&t.rot, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the current rotation of a piece

                memcpy(&nextblocks, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the next block up
                memcpy(&holdblock, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the currently held block

                memcpy(&level, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //the level the player was at
                memcpy(&lines, memblock+offset, sizeof(int)); 
                offset += sizeof(int); //lines of the player
                memcpy(&score, memblock+offset, sizeof(Uint32)); 
                offset += sizeof(Uint32); //score of the player

                memcpy(&gameStart, memblock+offset, sizeof(Uint32)); 
                offset += sizeof(Uint32); 

                memcpy(&testblocks, memblock+offset, sizeof(int[480])); 
                memcpy(&previousblocks, memblock+offset, sizeof(int[480])); 

            }
        }
        offset += sizeof(int[480]);
        if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::BGMODE] == 1) {
            bglevel = settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + level-1;
        }
        changemusic();
        delete memblock;
    }

}