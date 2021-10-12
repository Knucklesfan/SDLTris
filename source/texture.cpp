#include "texture.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

SDL_Texture** texture::loadTextures(std::string path, SDL_Renderer* render) {
	SDL_Texture* textures[64];
	for (const auto& entry : fs::directory_iterator(path)) {
		std::cout << entry.path() << std::endl;
	}
	return textures;
}
