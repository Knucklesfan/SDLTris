
#pragma once

#include <SDL.h>
#include <string>
#include <iostream>
#include <filesystem>

class texture
{
public:
	static SDL_Texture** loadTextures(std::string path, SDL_Renderer* render);
};

