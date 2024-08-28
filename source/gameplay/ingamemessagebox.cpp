#include "ingamemessagebox.h"
#include "../utils/defs.h"
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

void ingamemessagebox::render()
{
	#ifdef __LEGACY_RENDER
	SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
	SDL_Rect splashbox = { (int)x-2, (int)y, 252, 100 };
	SDL_RenderFillRect(graphics::render, &splashbox);
	int tmpx = (int) (uptime / 2.5) % 378;
	graphics::drawTexture(graphics::render,graphics::sprites.at("visualizer"), x - 2, y+44, false, tmpx, 0, 252, 38,1,1);
	graphics::fonts->at(2)->render(x, y + 4,name, false, 255, 0, 255, 250, true, uptime / 20, 1, 5);
	//letterfont->render(renderer, name, x, y + 4, false, 255, 0, 255,250);
	graphics::fonts->at(2)->render(x, y + 20,content, false, 255, 255, 255,250,false,0,0,0);
	#else
	if(active) {
		graphics::rect->render(graphics::shaders.at(1),{ (int)x-2, (int)y},{(int)x-2+252, (int)y+100 },0,{0,0,0,0.5},true,1,glm::vec4(1,1,1,1));
		
		int tmpx = (int) (uptime / 2.5) % 378;
		graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("visualizer"), {x-2,y+44},{252,38},0,{tmpx,0},{252,38});
		
		graphics::fonts->at(2)->render(x, y + 4,name, false, 255, 0, 255, 250, true, uptime / 20, 1, 5);
		//letterfont->render(renderer, name, x, y + 4, false, 255, 0, 255,250);
		graphics::fonts->at(2)->render(x, y + 20,content, false, 255, 255, 255,250,false,0,0,0);
	}
	#endif
}	


ingamemessagebox::ingamemessagebox(std::string title, std::string desc, int loc)
{
	name = title;
	content = desc;
	// renderer = render;
	// letterfont = letters;
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
//this is from font.h's more superior drawTexture.
//Whenever I get around to making utils.h (which will contain generic functions like these),
//i should remove this. For now, this is the latest version of the drawTexture function.
