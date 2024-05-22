#include "results.h"
#include "../utils/defs.h"
#include "../opengl/cube.h"
#include <ctime>

#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif

results::results() {
    newft = graphics::fonts->at(0);
    newhead = graphics::fonts->at(1);
    cubeRenderer = new cube(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f),glm::vec3((-0.0f), 0.0f, 0.0f));
    blockBuff = new buffermanager(200,200,true);
    background = bg("./sprites/resultsbg", true);
}

void results::input(SDL_Keycode key) {
    switch(key) {
        case SDLK_ESCAPE:
        case SDLK_z: {
            Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
            loadgame = true;
            break;
        }
    }
}
void results::reset()
{
    loadgame = false;
    time = std::time(nullptr);
    Mix_HaltMusic();
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( background.music, -1 );
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
    networking::globalRPC->update("Viewing the results.", "Last game's score: " + std::to_string(settings::previousscore), "icon4", std::time(nullptr));

}
void results::render() {
    // SDL_RenderClear(renderer);
    background.render();
    // SDL_Texture* temp = SDL_GetRenderTarget(renderer);
    // SDL_SetRenderTarget(renderer, rendertext);
    // SDL_RenderClear(renderer);
    for(int i = 0; i < 14; i++) {
        drawCubes(gameplay::Pieces[i%7][0], 0.5+abs(sin(leftangle / 25))/2, -20.0f+fmod(SDL_GetTicks()/1000.0f,28.0f)+(-20+i*4), 0, 16, 4, true,{0,(SDL_GetTicks()/-25.0f),0});
    }
    // SDL_SetRenderTarget(renderer, temp);
    // drawTexture(rendertext, 50, 200, leftangle / 2.5, 1.0, false);
    // SDL_SetRenderTarget(renderer, rendertext);
    // SDL_RenderClear(renderer);
    // drawCubes(gameplay::Pieces[2][0], 0.5+abs(cos(leftangle / 25))/2, 4.45, 0, 16, 4, true,{0,(SDL_GetTicks()/25.0f),0});
    // SDL_SetRenderTarget(renderer, temp);
    // drawTexture(rendertext, 462, 200, leftangle / -2.5, 1.0, false);

    newhead->render(320,160, "LAST GAME SCORE",  true);
    newft->render(320, 196, std::to_string(settings::previousscore), true);
    // renderfont(320,188,std::to_string(gamer->score), false, ttf);
    newft->render(320, 224, "Level: " + std::to_string(settings::lastlevel), true, 255,0,0,-1,false,0,0,0);
    newft->render(320, 252, "Lines: " + std::to_string(settings::lastlines), true);
    newhead->render(320,284, "RUN LENGTH", true);
    int real = time - settings::lasttime;
    int seconds = real%60;
    int minutes = (real/60)%60;
    int hours = minutes/60;
    char buff[12];
    snprintf(buff, sizeof(buff), "%02d:%02d:%02d", hours, minutes, seconds);
    std::string time = buff;
    newft->render(320, 320, time, true);
    
    // renderfont(320,244,"POINTS BY BLOCKS:",true, header);




    // background.render(renderer, true);


    //SDL_RenderPresent(renderer);
}

void results::logic(double deltatime)
{
    background.logic(deltatime);
    leftangle += deltatime/25;
}

Transition results::endLogic()
{
    if (loadgame) {
        return {
            0.001,
            2,
            320,
            240,
            FADETYPE::BLOCKS,
            true
        };
    }
    return {
            0.001,
            2,
            320,
            240,
            FADETYPE::BLOCKS,
            false
        };

}


void results::drawCubes(const int position[], float scale, float x, float y, int size, int width, bool threed, glm::vec3 rotation) {

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    view = glm::translate(view, glm::vec3(0.0, 0, -6.0)); 
                cubeRenderer->position = {x,y,-10};

    for (int i = 0; i < size; i++) {
        if (position[i] > 0) {
            if(!threed) {
                graphics::sprite->render(graphics::shaders[4],
                graphics::blocks->at(position[i]-1),
                {
                (x + (i % width) * 16)+(16-(16*scale))/2,
                (y + (i / width) * 16)+(16-(16*scale))/2
                },
                {16*scale,16*scale}, 0,{0,0},{16,16});
            }
            else { //handle 3d rendering of blocks
                glEnable(GL_DEPTH_TEST);  

                cubeRenderer->postposition = {
                    -width/2.0f+(i % width),
                    -(size/width)/2.0f+(i/width),
                    0
                    };
                cubeRenderer->scale = {scale,scale,scale};
                cubeRenderer->rotation = rotation;
                cubeRenderer->render(
                    graphics::shaders.at(0),
                    graphics::blocks->at(position[i]-1),
                    projection,view);
                
                glDisable(GL_DEPTH_TEST);  

            }
        }
    }
}

