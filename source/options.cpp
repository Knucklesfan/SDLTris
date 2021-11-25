#include "options.h"

options::options(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures, Mix_Music* musicVec, Mix_Chunk** soundVec, std::vector<font*> fonts) {
	renderer = render;
	background = backg;
	music = musicVec;
	sound = soundVec;
    texture = textures;
    cub = new cube(render,320/2,240/2,320,240);
    bigcub = new cube(render, 320, 240, 640, 480);
    tickertext = fonts.at(0);
    newhead = fonts.at(1);
    newft = fonts.at(2);
}

void options::keyPressed(SDL_Keycode key) {
        switch (key) {
            case SDLK_RIGHT: {
                if (currenttitle < 3) {
                    mvright = true;
                }
                break;
            }
            case SDLK_LEFT: {
                if (currenttitle > 0) {
                    mvleft = true;
                }
                break;

            }
            case SDLK_UP: {
                if (currentselection > 0) {
                    Mix_PlayChannel(-1, sound[1], 0);
                    currentselection--;
                }
                break;
            }
            case SDLK_DOWN: {
                if (currentselection < 6) {
                    Mix_PlayChannel(-1, sound[1], 0);
                    currentselection++;
                }
                break;

            }

        }
}

void options::render() {
	SDL_RenderClear(renderer);
	background.render(renderer, false);
    drawTexture(texture.at(8), 0, 0, rot/25, 1.0, true);
    drawTexture(texture.at(8), 640, 480, rot / 25, 1.0, true);
    cub->render(renderer, 255, 0, 0);
    drawTexture(cub->texture, -32, 0, 0, 1.0, false);
    drawTexture(cub->texture, 362, 0, 0, 1.0, false);
    drawTexture(cub->texture, 362, 272, 0, 1.0, false);
    drawTexture(cub->texture, -32, 272, 0, 1.0, false);
    bigcub->render(renderer, 0, 255, 0);
    drawTexture(bigcub->texture, 0, 0, 0, 1.0, false);


    std::string toprint = tcktxt.substr(txtpos, txtpos + 41);
    if (txtpos >= tcktxt.length()) {
        txtpos = 0;
    }

    if (txtpos + 41 > tcktxt.length()) {
        toprint = tcktxt.substr(txtpos, tcktxt.length()) + " " + tcktxt.substr(0, txtpos + 41 - tcktxt.length());
    }
    tickertext->render(fmod(-txtpos*16,16), 0, toprint, false, renderer);

    toprint = bottomtck.substr(bottompos, bottompos + 41);
    if (bottompos >= bottomtck.length()) {
        bottompos = 0;
    }

    if (bottompos + 41 > bottomtck.length()) {
        toprint = bottomtck.substr(bottompos, bottomtck.length()) + " " + bottomtck.substr(0, bottompos + 41 - bottomtck.length());
    }
    tickertext->render(fmod(-bottompos * 16, 16), 464, toprint, false, renderer);

    newhead->render(renderer, titles[currenttitle], 320, 80, true, 0, 0, 0, 0, true, rot / 100, 1, 5);

    for (int i = 0; i < 6; i++) {
        int selection = (currentselection == i) ? 0 : 255;
        tickertext->render(renderer, opts[currenttitle][i], settingsx, 128 + i * 24, true, 255, 255, selection);

    }
    if ((currenttitle - 1) >= 0) {
        for (int i = 0; i < 6; i++) {
            tickertext->render(renderer, opts[currenttitle - 1][i], settingsx - 640, 128 + i * 24, true, 255, 255, 255);

        }
    }
    if ((currenttitle + 1) < 4) {
        for (int i = 0; i < 6; i++) {
            tickertext->render(settingsx + 640, 128 + i * 24, opts[currenttitle + 1][i], true, renderer);

        }
    }
    if (currenttitle != 3 && currentselection != 5) {
        tickertext->render(renderer, "Enabled: " activations[currenttitle][currentselection]?"No":"Yes", 320, 360, true, 255, 0, 0);
    }
    tickertext->render(320, 400, details[currenttitle][currentselection], true, renderer,500);
	SDL_RenderPresent(renderer);

}
void options::logic(double deltatime) {
    //background.logic(deltatime);
    cub->logic(deltatime);
    bigcub->logic(-deltatime);
    rot += deltatime;
    txtpos += deltatime/100;
    bottompos += deltatime / 100;

    if (mvright && settingsx > -320) {
        settingsx -= deltatime;
    }
    else if (mvright) {
        mvright = false;
        currenttitle++;
        settingsx = 320;
    }

    if (mvleft && settingsx < 960) {
        settingsx += deltatime;
    }
    else if (mvleft) {
        mvleft = false;
        currenttitle--;
        settingsx = 320;
    }


}
int options::endlogic() {
	return 0;
}

void options::reset() {
    Mix_HaltMusic();
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(music, -1);
    }
    //If music is being played
    else
    {
        //If the music is paused
        if (Mix_PausedMusic() == 1)
        {
            //Resume the music
            Mix_ResumeMusic();
        }
        //If the music is playing
        else
        {
            //Pause the music
            Mix_PauseMusic();
        }
    }

}
void options::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
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


void options::moveright() {
    mvright = true;
}