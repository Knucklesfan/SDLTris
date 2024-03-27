#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include "opengl/buffermanager.h"

class wireframecube
{
public:
    double width = 320;
    double height = 240;
    void logic(double deltatime);
    void render(int r, int g, int b);
    wireframecube(int x, int y, int width, int height);
    buffermanager* buff;
    int x = width/2;
    int y = height/2;
    double rotation = 0.0;

    int edges[12][2] = {
    {0, 1}, {1, 3}, {3, 2}, {2, 0},
    {4, 5}, {5, 7}, {7, 6}, {6, 4},
    {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };
    double nodes[8][3] = {
    {-100, -100, -100},
    {-100, -100, +100},
    {-100, +100, -100},
    {-100, +100, +100},
    {+100, -100, -100},
    {+100, -100, +100},
    {+100, +100, -100},
    {+100, +100, +100}
    };
private:
    double rad(double deg);
    void rotateZ3D(double t);
    void rotateX3D(double t);
    void rotateY3D(double t);
};
