#include "texture.h"
#include <SDL.h>
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

SDL_Texture **loadTextures(std::string path) {
	SDL_Texture* textures[64];
	for (const auto& entry : fs::directory_iterator(path)) {

	}
}