#include "results.h"


results::results(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures,  Mix_Music* musicVec, Mix_Chunk** soundVec) {
    renderer = render;
    window = windows;
    background = backg;
    texture = textures;
    music = musicVec;
    sound = soundVec;

}

void results::reset()
{
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
void results::render(game* game) {
    SDL_RenderClear(renderer);
    background.render(renderer, false);
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
void results::renderfont(int x, int y, std::string strg, bool selected, TTF_Font* size) {
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
