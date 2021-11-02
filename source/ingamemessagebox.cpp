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
	SDL_Rect splashbox = { x-2, y, 252, 80 };
	SDL_RenderFillRect(renderer, &splashbox);
	renderfont(x, y + 4, name, true, font, renderer);
	renderfont(x, y + 20, content, false, font, renderer);
}	


ingamemessagebox::ingamemessagebox(std::string title, std::string desc, SDL_Renderer* render, TTF_Font* letters, int loc)
{
	name = title;
	content = desc;
	renderer = render;
	font = letters;
	x = loc;
	y = 480;
	active = false;
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

void ingamemessagebox::renderfont(int x, int y, std::string strg, bool selected, TTF_Font* size, SDL_Renderer* renderer) {
	SDL_Surface* text;
	SDL_Color color = { 255, 255, 0 };
	if (!selected) {
		color = { 255, 255, 255 };
	}
	text = TTF_RenderText_Blended_Wrapped(size, strg.c_str(), color,250);
	SDL_Texture* words = SDL_CreateTextureFromSurface(renderer, text);
	SDL_FreeSurface(text);
	drawTexture(renderer, words, x, y, 0, 1.0, false);
    SDL_DestroyTexture(words);

}

void ingamemessagebox::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
	SDL_Rect sprite;
	SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h);
	int oldwidth = sprite.w;
	int oldheight = sprite.h;
	sprite.w = sprite.w * scale;
	sprite.h = sprite.h * scale;
	if (center) {
		sprite.x = x - oldwidth / 2;
		sprite.y = y - oldheight / 2;
	}
	else {
		sprite.x = x + oldwidth / 2 - sprite.w / 2;
		sprite.y = y + oldheight / 2 - sprite.h / 2;
	}
	SDL_RenderCopyEx(renderer, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}
