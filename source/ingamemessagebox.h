#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include "font.h"

class ingamemessagebox
{
    public:
        double x = 0;
        double y = 0;
        bool goup = false;
        bool godown = false;
        bool active = false;
        double uptime = 0;
        void logic(double deltatime);
        void activate(std::string title, std::string desc);
        std::string name;
        std::string content;
        void render();
        ingamemessagebox(std::string name, std::string desc, int loc);
        ingamemessagebox();

    private:

    };
