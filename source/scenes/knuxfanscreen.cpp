#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include "../background.h"
#include "knuxfanscreen.h"
#include "../pixfont.h"
#include "../utils/defs.h"
/*
okay, i know this is a lot to ask of you, not midnight version of me, but listen and hear me out

this entire sequence? lets not hardcode it. Nah, instead, make a cutscene engine that lets you handle stuff like this.
Obviously, to do this we'd need an object system, but that shouldn't be too hard to recreate
besides, having a cutscene system to let you handle things like this should make development easier, especially for mod devs.
i know that this is difficult, but you have plenty of time so why not work on something like this
yknow what they say, your best stuff is always written in college

also secondary note: find out who that girl who knew your name was

*/
knuxfanscreen::knuxfanscreen() {
    active = true;
    godown = true;
    backnum = std::rand() % graphics::backgrounds->size();
    splash = rand() % 35;
    // front = new pixfont("small8x8font");

}
void knuxfanscreen::render() {
    SDL_RenderClear(graphics::render);
    graphics::backgrounds->at(backnum).render(graphics::render, false);
    drawTexture(graphics::sprites["knuxfan"], knuxfanx, knuxfany, rotate, 1.0, false);
    drawTexture(graphics::sprites["presents"], presentsx, presentsy, -rotate, 1.0, false);

    graphics::fonts->at(2)->render(320, 450, splashes[splash], true);
    SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255 * alpha);
    SDL_Rect splashbox = { 0, 0, 640, 480 };
    SDL_RenderFillRect(graphics::render, &splashbox);
    //SDL_RenderPresent(renderer);

}
void knuxfanscreen::keyPressed(SDL_Keycode key) {
    active = false;
}
void knuxfanscreen::logic(double deltatime) {
    graphics::backgrounds->at(backnum).logic(deltatime);
    if (active) {
        lifetime += deltatime / 5;
        if (alpha > 0.0 && godown) {
            alpha -= deltatime / 1500;
        }
        else if (alpha <= 0.0 && godown) {
            godown = false;
            alpha = 0;
        }
        // if (alpha < 1.0 && goup) {
        //     alpha += deltatime / 1500;
        // }
        // else if (alpha >= 1.0 && goup) {
        //     godown = false;
        //     active = false;
        // }

        if ((!godown && !goup) && lifetime > 1000) {
            active = false;
        }
        if(lifetime > 650) {
            explode = true;
        }
        //std::cout << lifetime << "\n";
        if (alpha < 0.0) {
            alpha = 0.0;
        }
        if (alpha > 1.0) {
            alpha = 1.0;
        }
        if(!explode) {
            knuxfany = utils::lerp(knuxfany,210,0.05);
            presentsy = utils::lerp(presentsy,210,0.05);
        }
        else {
            rotate += deltatime;
            knuxfanx -= deltatime*0.15;
            presentsx += deltatime*0.15;
            
            knuxfany += yvelocity*deltatime*0.5;
            presentsy += yvelocity*deltatime*0.5;

            yvelocity += deltatime*0.0025;
        }

    }
}
void knuxfanscreen::reset() {
    Mix_HaltMusic();
    lifetime = 0;
    active = true;
     knuxfanx = 76;
	 knuxfany = 0.0;

	 presentsx = 306;
	 presentsy = 480.0;

	 yvelocity = -1.0;

	 explode = false;
	 rotate = 0;


}
Transition knuxfanscreen::endLogic() {
    Transition t = Transition();
    if (!active) {
        t.gamemode = 1;
        t.transition = true;
    }
    
    return t;
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
    SDL_RenderCopyEx(graphics::render, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}
