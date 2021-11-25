#include "options.h"

options::options(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures, Mix_Music* musicVec, Mix_Chunk** soundVec, std::vector<font*> fonts) {
	renderer = render;
	background = backg;
	music = musicVec;
	sound = soundVec;
    texture = textures;
    cub = new cube(render,320/2,240/2,320,240);
    bigcub = new cube(render, 320, 240, 640, 480);

}

void options::keyPressed(SDL_Keycode key) {

}
void options::render() {

	SDL_RenderClear(renderer);
	background.render(renderer, false);
    drawTexture(texture.at(8), 0, 0, rot/25, 1.0, true);
    drawTexture(texture.at(8), 640, 480, rot / 25, 1.0, true);
    cub->render(renderer, 255, 0, 0);
    drawTexture(cub->texture, -32, 0, 0, 1.0, false);
    drawTexture(cub->texture, 362, 0, 0, 1.0, false);
    drawTexture(cub->texture, 362, 272, 0, 1.0, false);
    drawTexture(cub->texture, -32, 272, 0, 1.0, false);
    bigcub->render(renderer, 0, 255, 0);
    drawTexture(bigcub->texture, 0, 0, 0, 1.0, false);

	SDL_RenderPresent(renderer);

}
void options::logic(double deltatime) {
    background.logic(deltatime);
    cub->logic(deltatime);
    bigcub->logic(-deltatime);
    rot += deltatime;

}
int options::endlogic() {
	return 0;
}

void options::reset() {
    Mix_HaltMusic();
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(music, -1);
    }
    //If music is being played
    else
    {
        //If the music is paused
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
    }

}
void options::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
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
