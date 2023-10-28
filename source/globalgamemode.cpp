#include "globalgamemode.h"
#include <SDL2/SDL.h>
#include "utils/defs.h"
#include <cmath>


int GlobalGamemode::logic(double deltatime) {
    if(!fade && alpha > 0) {
        alpha -= deltatime*0.001;
    }
    if(alpha < 0) {
        alpha = 0;
    }
    if(fade && alpha <= 1) {
        alpha += deltatime*currentTransition.speed; //why is dividing deltatime a bad practice??
    }
    if(fade && alpha >= 1) {
        fade = false;
        alpha = 1;
        return 1;
    }
    return 0;
}
void GlobalGamemode::setFade(Transition resp) {
    if(!fade) {
        alpha = 0;
        fade = true;
        currentTransition = resp;

    }
    
}
void GlobalGamemode::render() {

    switch(currentTransition.fade) {
        case FADE: {
            #ifdef __LEGACY_RENDER
            SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255 * alpha);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(graphics::render, &splashbox);
            #else
            graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{0,0,0,alpha},false,-1);
            #endif
            break;
        }
        case BLOCKS: {
            #ifdef __LEGACY_RENDER
            SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255);
            #endif
            if(fade) {
                #ifdef __LEGACY_RENDER
                SDL_Rect splashbox = { 0, -480+math::easeOutBounce((alpha*0.75))*480, 640, 480};
                SDL_RenderFillRect(graphics::render, &splashbox);
                #else
                graphics::rect->render(graphics::shaders.at(1),{
                    0,
                    0
                },{
                    640,
                    math::easeOutBounce((alpha*0.75))*480
                    },0,{0,0,0,1},false,-1);
                #endif
            }
            else {
                #ifdef __LEGACY_RENDER
                SDL_Rect splashbox = { 
                    0,
                     -480+(1-math::easeOutBounce(1-alpha))*480,
                      640,
                       480};
                SDL_RenderFillRect(graphics::render, &splashbox);
                #else
                graphics::rect->render(graphics::shaders.at(1),{0,-480+(1-math::easeOutBounce(1-alpha))*480},{640,480},0,{0,0,0,1},false,-1);
                #endif

            }
            break;
        }
    }

}
double math::easeOutBounce(double x) {
double n1 = 7.5625;
double d1 = 2.75;

if (x < 1 / d1) {
    return n1 * x * x;
} else if (x < 2 / d1) {
    return n1 * (x -= 1.5 / d1) * x + 0.75;
} else if (x < 2.5 / d1) {
    return n1 * (x -= 2.25 / d1) * x + 0.9375;
} else {
    return n1 * (x -= 2.625 / d1) * x + 0.984375;
}

}