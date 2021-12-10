#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <math.h>
class sine
{
public:
    double width = 320;
    double height = 240;
    void logic(double deltatime);
    void render(SDL_Renderer* renderer, int r, int g, int b);
    sine(SDL_Renderer* render, int x, int y, int width, int height);
    int x = width/2;
    int y = height/2;
    double rotation = 0.0;
    SDL_Texture* texture;
private:
    double rad(double deg);
    void rotateZ3D(double t);
    void rotateX3D(double t);
    void rotateY3D(double t);
};
