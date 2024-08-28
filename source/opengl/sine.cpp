#include "sine.h"
#include "../utils/defs.h"

sine::sine(int vx, int vy, int wid, int high) {
    buff = new buffermanager(wid,high,true);
    width = wid;
    height = high;
    x = vx;
    y = vy;
}
void sine::logic(double deltaTime) {
    rotation += deltaTime/100;
    
}
void sine::render(int r, int g, int b, int a, int offset) {
    // SDL_Texture* temp = SDL_GetRenderTarget(renderer);
    // SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    // SDL_SetRenderTarget(renderer, texture);
    // SDL_RenderClear(renderer);
    buff->enable();
    double last = sin((rotation))*(50+offset)+y;
    for(int i = 0; i < width; i++) {
        // SDL_SetRenderDrawColor(renderer, r,g,b,a);
        double drawpoint = sin((rotation+i/20))*(50+offset)+y;
        graphics::line->render(graphics::shaders.at(1),{i-1,last},{i,drawpoint},1,{r,g,b,a},{width,height});
        last = drawpoint;
    }
    buff->disable(640,480,true);
    graphics::sprite->render(graphics::shaders.at(4),
    &buff->renderTexture, {0,0},{640,480},{0,0,0},{0,0},{320,240});
    // SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    // SDL_SetRenderTarget(renderer,temp);
    // SDL_RenderCopy(renderer, texture, NULL, NULL);

}