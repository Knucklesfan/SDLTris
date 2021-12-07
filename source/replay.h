#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

class replay
{
    
	public:
        std::string path;
        std::ofstream outfile;

        bool record;
        bool playback;
        replay();
        int loadreplay(std::string path);
        int recordreplay(std::string path);
        SDL_Keycode logic(long long tick);
        SDL_Keycode logic(SDL_Keycode key, long long tick);
        void endlogic(bool gameactive);

};
