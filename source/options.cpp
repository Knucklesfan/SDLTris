#include "options.h"

options::options(SDL_Renderer* render, SDL_Window* windows, bg backg, std::vector<SDL_Texture*> textures, Mix_Music* musicVec, Mix_Chunk** soundVec, std::vector<font*> fonts, std::vector<bg> backgcollection) {
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
    window = windows;
    backgrounds = backgcollection;
}

void options::keyPressed(SDL_Keycode key) {
    switch (key) {
    case SDLK_RIGHT: {
        switch (currentscreen) {
        case 0: {
            if (currenttitle < 3) {
                mvright = true;
            }
            break;
        }
        case 1: {
            int currentvol = Mix_VolumeMusic(-1);
            if (currentvol < MIX_MAX_VOLUME) {
                Mix_VolumeMusic(currentvol + 1);
            }
            break;

        }
        case 2: {
            int currentvol = Mix_Volume(-1, -1);
            if (currentvol < MIX_MAX_VOLUME) {
                Mix_Volume(-1, currentvol + 1);
            }
            break;
        }
        case 3: {
            int currentvol = activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG];
            if (currentvol < backgrounds.size()-1) {
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]++;
            }
            break;
        }

        }
        break;
    }
    case SDLK_LEFT: {
        switch (currentscreen) {
        case 0: {
            if (currenttitle > 0) {
                mvleft = true;
            }
            break;
        }
        case 1: {
            int currentvol = Mix_VolumeMusic(-1);
            if (currentvol > 0) {
                Mix_VolumeMusic(currentvol - 1);
            }
            break;

        }
        case 2: {
            int currentvol = Mix_Volume(-1, -1);
            if (currentvol > 0) {
                Mix_Volume(-1, currentvol - 1);
            }
            break;
        }
        case 3: {
            int currentvol = activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG];
            if (currentvol > 0) {
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]--;
            }
            break;
        }

        }
        break;

    }
    case SDLK_UP: {
        if (currentselection > 0 && currentscreen == 0) {
            Mix_PlayChannel(-1, sound[1], 0);
            currentselection--;
            if (currentselection == 4 && currenttitle == 0) {
                currentselection--;
            }

        }
        break;
    }
    case SDLK_DOWN: {
        if (currentselection < 6 && currentscreen == 0) {
            Mix_PlayChannel(-1, sound[1], 0);
            currentselection++;
            if (currentselection == 4 && currenttitle == 0) {
                currentselection++;
            }
        }
        break;

    }
    case SDLK_z: {
        switch (currentscreen) {
            case 0: {
                switch (currenttitle) {
                case 0: {
                    switch (currentselection) {
                    case 5: {
                        break;
                    }

                    default: {
                        activations[currenttitle][currentselection] = !activations[currenttitle][currentselection];
                        break;
                    }

                    }
                    break;
                }
                case 1: {
                    switch (currentselection) {
                    case 5: {
                        break;
                    }
                    case DISPLAYOPTIONS::FIRSTBG: {
                        currentscreen = 3;
                        break;
                    }
                    default: {
                        activations[currenttitle][currentselection] = !activations[currenttitle][currentselection];
                        break;
                    }
                    }
                    break;
                }
                case 2: {
                    switch (currentselection) {
                    case 5: {
                        break;
                    }

                    case 0: {
                        Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
                        bool IsFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;
                        SDL_SetWindowFullscreen(window, IsFullscreen ? 0 : FullscreenFlag);
                        SDL_ShowCursor(IsFullscreen);
                        break;
                    }
                    case 1: {
                        currentscreen = 1;
                        break;
                    }
                    case 2: {
                        currentscreen = 2;
                        break;
                    }
                    case 3: {
                        currentscreen = 9;
                        break;
                    }
                    case 4: {
                        currentscreen = 9;
                        break;
                    }

                    }
                    break;
                }
                case 3: {
                    switch (currentselection) {
                        default: {
                            currentscreen = 9;
                            break;
                        }
                    }
                    break;
                }
                }
                if (currentselection == 5) {
                    loadgame = true;
                }
                break;
            }
            default: {
                currentscreen = 0;
                break;
            }
        }
        Mix_PlayChannel(-1, sound[0], 0);
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
    if (currenttitle != 2 && currentselection != 5) {
        std::string txt = "Value: ";
        if (currenttitle == OPTIONTYPE::DISPLAY && currentselection == DISPLAYOPTIONS::FIRSTBG) {
            txt += backgrounds[activations[currenttitle][currentselection]].name;
        }
        else {
            txt += activations[currenttitle][currentselection] ? "Enabled" : "Disabled";
        }
        tickertext->render(renderer, txt, 320, 360, true, 255, 0, 0);
        
    }
    tickertext->render(320, 400, details[currenttitle][currentselection], true, renderer,500);


    switch (currentscreen) {
        case 0:
            break;
        case 1: {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            splashbox = { 320 - 240, 240 - 120, 480, 240 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);
            double var = ((double)Mix_VolumeMusic(-1) / 128) * 58.0 + sin(time / 50) * 10;
            drawTexture(renderer, texture.at(7), 80, 360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
            double coz = ((double)Mix_VolumeMusic(-1) / 128) * 58.0 + cos(time / 50) * 10;
            drawTexture(renderer, texture.at(7), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);
            newhead->render(320, 140, "MUSIC VOLUME", true, renderer);
            tickertext->render(renderer, std::to_string(Mix_VolumeMusic(-1)), 320, 180, true, 255, 0, 255);
            tickertext->render(320, 300, "EXIT", true, renderer);
            break;
        }
        case 2: {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            splashbox = { 320 - 240, 240 - 120, 480, 240 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);
            double var = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + sin(time / 50) * 10;
            drawTexture(renderer, texture.at(7), 80, 360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
            double coz = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + cos(time / 50) * 10;
            drawTexture(renderer, texture.at(7), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);

            newhead->render(320, 140, "SOUND VOLUME", true, renderer);
            tickertext->render(renderer, std::to_string(Mix_Volume(-1, -1)), 320, 180, true, 255, 0, 255);
            tickertext->render(320, 300, "EXIT", true, renderer);


            break;
        }
        case 3: {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            splashbox = { 320 - 240, 80, 480, 360 };
            SDL_RenderFillRect(renderer, &splashbox);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &splashbox);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);
            newhead->render(320, 88, "SELECT A BACKGROUND", true, renderer);

            tickertext->render(renderer, "Current Selection: " + backgrounds[activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]].name, 320, 120, true, 255, 0, 255, 400);
            int before = (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] - 1) >= 0 ? (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] - 1) : backgrounds.size() - 1;
            int after = (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + 1) > backgrounds.size() - 1 ? 0 : (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + 1);

            drawTexture(backgrounds[activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]].thumbnail, 320-64, 156, 0, 1.0, false);
            drawTexture(backgrounds[before].thumbnail, 320 - 168, 185, 0, 0.5, false);
            drawTexture(backgrounds[after].thumbnail, 360, 185, 0, 0.5, false);

            tickertext->render(renderer, "Name: " + backgrounds[activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]].name, 320, 284, true, 255, 0, 255);
            tickertext->render(renderer, "Creator: " + backgrounds[activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]].creator, 320, 308, true, 255, 0, 255);
            tickertext->render(renderer, "Song Name: " + backgrounds[activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]].songname, 320, 332, true, 255, 0, 255);
            tickertext->render(renderer, "Song Creator: " + backgrounds[activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]].artist, 320, 356, true, 255, 0, 255);

            tickertext->render(320, 408, "EXIT", true, renderer);


            break;
        }

        case 9: {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            splashbox = { 320 - 240, 240 - 120, 480, 240 };
            SDL_RenderFillRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &splashbox);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 26);

            newhead->render(320, 140, "UNFINISHED", true, renderer);
            tickertext->render(320, 180, "I swear it'll be here when I'm done...", true, renderer, 448);
            tickertext->render(320, 300, "EXIT", true, renderer);

        }
    }





	SDL_RenderPresent(renderer);

}
void options::logic(double deltatime) {
    if (activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::MOVINGBG]) {
        background.logic(deltatime);
        cub->logic(deltatime);
        bigcub->logic(-deltatime);
        rot += deltatime;
        txtpos += deltatime / 100;
        bottompos += deltatime / 100;
    }
    if (mvright && settingsx > -320) {
        settingsx -= deltatime;
    }
    else if (mvright) {
        mvright = false;
        currenttitle++;
        settingsx = 320;
        currentselection = 0;
    }

    if (mvleft && settingsx < 960) {
        settingsx += deltatime;
    }
    else if (mvleft) {
        mvleft = false;
        currenttitle--;
        settingsx = 320;
        currentselection = 0;

    }


}
int options::endlogic() {
    if (loadgame) {
        return 1;
    }
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

//this is the new drawtexture, for drawing sprite sheets and whatnot
void options::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
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
