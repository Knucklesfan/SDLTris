#include "results.h"

#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif

results::results(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures,  Mix_Music* musicVec, Mix_Chunk** soundVec, std::vector<font*> fonts) {
    renderer = render;
    window = windows;
    background = backg;
    texture = textures;
    music = musicVec;
    sound = soundVec;
    newft = fonts.at(0);
    newhead = fonts.at(1);
}

void results::keyPressed(SDL_Keycode key) {
    switch(key) {
        case SDLK_ESCAPE:
        case SDLK_z: {
            Mix_PlayChannel( -1, sound[1], 0 );
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
        Mix_PlayMusic( music, -1 );
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
void results::render(game* gamer) {
    SDL_RenderClear(renderer);
    background.render(renderer, false);

    newhead->render(renderer, "LAST GAME SCORE", 320,160, true);
    newft->render(renderer,std::to_string(gamer->score), 320, 196,true);
    //renderfont(320,188,std::to_string(gamer->score), false, ttf);
    newft->render(renderer, "Level: " + std::to_string(gamer->level), 320, 224,true, 255,0,0);
    newft->render(renderer, "Lines: " + std::to_string(gamer->lines), 320, 252,true);
    newhead->render(renderer, "RUN LENGTH", 320,284,true);
    int real = time - gamer->time;
    int seconds = real%60;
    int minutes = real/60;
    int hours = minutes/60;
    std::string time = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    newft->render(renderer, time, 320, 320,true);
    
    //renderfont(320,244,"POINTS BY BLOCKS:",true, header);




    background.render(renderer, true);


    SDL_RenderPresent(renderer);
}

void results::logic(double deltatime)
{
    background.logic(deltatime);
}

int results::endlogic()
{
    if (loadgame) {
        return 1;
    }
	return 0;
}

void results::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
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
