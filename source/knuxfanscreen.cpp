#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include <array>
#include <cstring>
#include "background.h"
#include "knuxfanscreen.h"

knuxfanscreen::knuxfanscreen(SDL_Renderer* render, std::vector<SDL_Texture*> texture, Mix_Chunk* soundVec[]) {
    renderer = render; 
    textures = texture;
    sound = soundVec;
    active = true;
    godown = true;
}
void knuxfanscreen::render() {
    SDL_RenderClear(renderer);
    drawTexture(textures[8],0,0,0,1.0,false);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255*alpha);
    SDL_Rect splashbox = { 0, 0, 640, 480 };
    SDL_RenderFillRect(renderer, &splashbox);
    SDL_RenderPresent(renderer);

}

void knuxfanscreen::logic(double deltatime) {
	if (active) {
		lifetime += deltatime/5;
		if (alpha > 0.0 && godown) {
			alpha-=deltatime/1500;
		}
		else if (alpha <= 0.0 && godown) {
			godown = false;
		}
		if (alpha < 1.0 && goup) {
			alpha+=deltatime / 1500;
		}
		else if (alpha >= 1.0 && goup) {
			godown = false;
			active = false;
		}

		if ((!godown && !goup) && lifetime > 1000) {
			goup = true;
		}
        std::cout << lifetime << "\n";
	}
}

int knuxfanscreen::endlogic() {
    if(!active) {
        return 1;
    }
}
void knuxfanscreen::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
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
