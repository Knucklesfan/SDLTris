#include "sine.h"


sine::sine(SDL_Renderer* render, int vx, int vy, int wid, int high) {
    texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, wid,high);
    width = wid;
    height = high;
    x = vx;
    y = vy;
}
void sine::logic(double deltaTime) {
    rotation += deltaTime/100;
}
void sine::render(SDL_Renderer* renderer, int r, int g, int b) {
    SDL_Texture* temp = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderClear(renderer);
    double last = sin(rotation*100)*5+y;
    for(int i = 0; i < width; i++) {
        SDL_SetRenderDrawColor(renderer, rand()%255,rand()%255,rand()%255,255);
        double drawpoint = sin(rotation+i/20)*50+y;
        SDL_RenderDrawLine(renderer, i-1, last, i, drawpoint);
        last = drawpoint;
    }
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_SetRenderTarget(renderer,temp);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

}