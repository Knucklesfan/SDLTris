#include "ingamemessagebox.h"

void ingamemessagebox::logic(double deltatime)
{
	if (active) {
		uptime += deltatime/5;
		if (y > 400 && goup) {
			y-=deltatime/15;
		}
		else if (y < 400 && goup) {
			goup = false;
		}
		if (y <= 480 && godown) {
			y+=deltatime / 15;
		}
		else if (y > 480 && godown) {
			godown = false;
			active = false;
		}

		if ((!godown && !goup) && uptime > 500) {
			godown = true;
		}
	}

}

void ingamemessagebox::render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
	SDL_Rect splashbox = { x-2, y, 252, 100 };
	SDL_RenderFillRect(renderer, &splashbox);
	int tmpx = (int) (uptime / 2.5) % 378;
	drawTexture(renderer, textures.at(7), x - 2, y+44, 0, 1.0, false, tmpx, 0, 252, 38);
	letterfont->render(renderer, name, x, y + 4, false, 255, 0, 255, 250, true, uptime / 20, 1, 5);
	//letterfont->render(renderer, name, x, y + 4, false, 255, 0, 255,250);
	letterfont->render(renderer, content, x, y + 20, false, 255, 255, 255,250);
}	


ingamemessagebox::ingamemessagebox(std::string title, std::string desc, SDL_Renderer* render, std::vector<SDL_Texture*> texture, font* letters, int loc)
{
	name = title;
	content = desc;
	renderer = render;
	letterfont = letters;
	x = loc;
	y = 480;
	active = false;
	textures = texture;
}
void ingamemessagebox::activate(std::string title, std::string desc) {
	name = title;
	content = desc;
	y = 480;
	uptime = 0;
	godown = false;
	goup = true;
	active = true;

}
ingamemessagebox::ingamemessagebox()
{
	name = "null";
	content = "its empty lol";
	x = 16;
	y = 480;
	active = false;
}
//this is from font.h's more superior drawTexture.
//Whenever I get around to making utils.h (which will contain generic functions like these),
//i should remove this. For now, this is the latest version of the drawTexture function.

void ingamemessagebox::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
	SDL_Rect sprite;
	SDL_Rect srcrect = { srcx, srcy, srcw, srch };
	if (SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
		printf("TEXTURE ISSUES!!! \n");
		std::cout << SDL_GetError() << "\n";
	};
	sprite.w = srcw * scale;
	sprite.h = srch * scale;
	if (center) {
		sprite.x = x - srcw / 2;
		sprite.y = y - srch / 2;
	}
	else {
		sprite.x = x + srcw / 2 - sprite.w / 2;
		sprite.y = y + srch / 2 - sprite.h / 2;
	}
	SDL_RenderCopyEx(renderer, texture, &srcrect, &sprite, 0, NULL, SDL_FLIP_NONE);
}
