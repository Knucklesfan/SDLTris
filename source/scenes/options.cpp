#include "options.h"
#include "../utils/defs.h"

options::options()
{
    cub = new cube(graphics::render, 320 / 2, 240 / 2, 320, 240);
    bigcub = new cube(graphics::render, 320, 240, 640, 480);
    tickertext = graphics::fonts->at(0);
    newhead = graphics::fonts->at(1);
    newft = graphics::fonts->at(2);
    background = bg("./sprites/optionsbg", true, graphics::render);
}

void options::input(SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_RIGHT:
    {
        switch (currentscreen)
        {
        case 0:
        {
            if (currenttitle < 4 && !mvleft)
            {
                mvright = true;
            }
            break;
        }
        case 1:
        {
            int currentvol = Mix_VolumeMusic(-1);
            if (currentvol < MIX_MAX_VOLUME)
            {
                Mix_VolumeMusic(currentvol + 1);
            }
            break;
        }
        case 2:
        {
            int currentvol = Mix_Volume(-1, -1);
            if (currentvol < MIX_MAX_VOLUME)
            {
                Mix_Volume(-1, currentvol + 1);
            }
            break;
        }
        case 3:
        {
            int currentvol = settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG];
            if (currentvol < graphics::backgrounds->size() - 1)
            {
                settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]++;
            }
            break;
        }
        case 4:
        {
            if (currentmenuselect < 1)
            {
                currentmenuselect++;
                Mix_PlayChannel(-1, audio::sfx->at(1), 0);
            }

            break;
        }
        case 5:
        { // Level selector screen
            if (settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::LEVELLENGTH] < 255)
            {
                settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::LEVELLENGTH]++;
                Mix_PlayChannel(-1, audio::sfx->at(1), 0);
            }

            break;
        }
        }
        break;
    }
    case SDLK_LEFT:
    {
        switch (currentscreen)
        {
        case 0:
        {
            if (currenttitle > 0 && !mvright)
            {
                mvleft = true;
            }
            break;
        }
        case 1:
        {
            int currentvol = Mix_VolumeMusic(-1);
            if (currentvol > 0)
            {
                Mix_VolumeMusic(currentvol - 1);
            }
            break;
        }
        case 2:
        {
            int currentvol = Mix_Volume(-1, -1);
            if (currentvol > 0)
            {
                Mix_Volume(-1, currentvol - 1);
            }
            break;
        }
        case 3:
        {
            int currentvol = settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG];
            if (currentvol > 0)
            {
                settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]--;
            }
            break;
        }
        case 4:
        {
            if (currentmenuselect > 0)
            {
                currentmenuselect--;
                Mix_PlayChannel(-1, audio::sfx->at(1), 0);
            }

            break;
        }
        case 5:
        { // Level selector screen
            if (settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::LEVELLENGTH] > 1)
            {
                settings::activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::LEVELLENGTH]--;
                Mix_PlayChannel(-1, audio::sfx->at(1), 0);
            }

            break;
        }
        }
        break;
    }
    case SDLK_UP:
    {
        if (currentselection > 0 && currentscreen == 0)
        {
            Mix_PlayChannel(-1, audio::sfx->at(1), 0);
            currentselection--;
        }
        break;
    }
    case SDLK_DOWN:
    {
        if (currentselection < maxes[currenttitle] && currentscreen == 0)
        {
            Mix_PlayChannel(-1, audio::sfx->at(1), 0);
            currentselection++;
        }
        break;
    }
    case SDLK_z:
    {
        switch (currentscreen)
        {
        case 0:
        {
            switch (currenttitle)
            {
            case 0:
            {
                switch (currentselection)
                {
                case 8:
                {
                    loadgame = true;
                    break;
                }
                case GAMEPLAYOPTIONS::LEVELLENGTH:
                {
                    currentscreen = 5;
                    break;
                }

                default:
                {
                    settings::activations[currenttitle][currentselection] = !settings::activations[currenttitle][currentselection];
                    break;
                }
                }
                break;
            }
            case 1:
            {
                switch (currentselection)
                {
                case 6:
                {
                    loadgame = true;
                    break;
                }
                case DISPLAYOPTIONS::FIRSTBG:
                {
                    currentscreen = 3;
                    break;
                }
                default:
                {
                    settings::activations[currenttitle][currentselection] = !settings::activations[currenttitle][currentselection];
                    break;
                }
                }
                break;
            }
            case 2:
            {
                switch (currentselection)
                {
                case 5:
                {
                    loadgame = true;
                    break;
                }

                case 0:
                {
                    settings::activations[currenttitle][currentselection] = !settings::activations[currenttitle][currentselection];
                    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
                    bool IsFullscreen = SDL_GetWindowFlags(graphics::window) & FullscreenFlag;
                    SDL_SetWindowFullscreen(graphics::window, IsFullscreen ? 0 : FullscreenFlag);
                    SDL_ShowCursor(IsFullscreen);
                    break;
                }
                case 1:
                {
                    currentscreen = 1;
                    break;
                }
                case 2:
                {
                    currentscreen = 2;
                    break;
                }
                case 3:
                {
                    currentscreen = 4;
                    break;
                }
                case 4:
                {
                    currentscreen = 4;
                    break;
                }
                }
                break;
            }
            case 3:
            {
                switch (currentselection)
                {
                default:
                {
                    settings::activations[currenttitle][currentselection] = !settings::activations[currenttitle][currentselection];
                    break;
                }
                }
                break;
            }
            }
            break;
        }
        case 4:
        {
            if (currentmenuselect == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 6; j++)
                    {
                        settings::activations[i][j] = settings::defaults[i][j];
                    }
                }
            }
            currentscreen = 0;
            break;
        }
        default:
        {
            currentscreen = 0;
            break;
        }
        }
        Mix_PlayChannel(-1, audio::sfx->at(0), 0);
        break;
    }
    case SDLK_F1:
    {
        settings::activations[OPTIONTYPE::DEBUG][DEBUGOPTIONS::DEBUGENABLED] = !settings::activations[OPTIONTYPE::DEBUG][DEBUGOPTIONS::DEBUGENABLED];
        break;
    }
    }
}

void options::render()
{
    SDL_RenderClear(graphics::render);
    background.render(graphics::render, false);
    drawTexture(graphics::sprites["ring"], 0, 0, rot / 25, 1.0, true);
    drawTexture(graphics::sprites["ring"], 640, 480, rot / 25, 1.0, true);
    cub->render(graphics::render, 255, 0, 0);
    drawTexture(cub->texture, -32, 0, 0, 1.0, false);
    drawTexture(cub->texture, 362, 0, 0, 1.0, false);
    drawTexture(cub->texture, 362, 272, 0, 1.0, false);
    drawTexture(cub->texture, -32, 272, 0, 1.0, false);
    bigcub->render(graphics::render, 0, 255, 0);

    drawTexture(bigcub->texture, 0, 0, 0, 1.0, false);

    std::string toprint = tcktxt.substr(txtpos, txtpos + 41);
    if (txtpos >= tcktxt.length())
    {
        txtpos = 0;
    }

    if (txtpos + 41 > tcktxt.length())
    {
        toprint = tcktxt.substr(txtpos, tcktxt.length()) + " " + tcktxt.substr(0, txtpos + 41 - tcktxt.length());
    }
    tickertext->render(fmod(-txtpos * 16, 16), 0, toprint, false);

    toprint = bottomtck.substr(bottompos, bottompos + 41);
    if (bottompos >= bottomtck.length())
    {
        bottompos = 0;
    }

    if (bottompos + 41 > bottomtck.length())
    {
        toprint = bottomtck.substr(bottompos, bottomtck.length()) + " " + bottomtck.substr(0, bottompos + 41 - bottomtck.length());
    }
    tickertext->render(fmod(-bottompos * 16, 16), 464, toprint, false);

    newhead->render(320, 80, titles[currenttitle], true, 255, 255, 255, 0, true, rot / 100, 1, 5);

    for (int i = 0; i < maxes[currenttitle] + 1; i++)
    {
        int selection = (currentselection == i) ? 0 : 255;
        tickertext->render(settingsx, 128 + i * 24, opts[currenttitle][i], true, 255, 255, selection, 0, false, 0, 0, 0);
    }
    if ((currenttitle - 1) >= 0)
    {
        for (int i = 0; i < maxes[currenttitle - 1] + 1; i++)
        {
            tickertext->render(settingsx - 640, 128 + i * 24, opts[currenttitle - 1][i], true);
        }
    }
    if ((currenttitle + 1) < 5)
    {
        for (int i = 0; i < maxes[currenttitle + 1] + 1; i++)
        {
            tickertext->render(settingsx + 640, 128 + i * 24, opts[currenttitle + 1][i], true);
        }
    }
    if ((!(currenttitle == 3 && currentselection == 3)) && currentselection != 5)
    {
        std::string txt = "Value: ";
        if (currenttitle == OPTIONTYPE::DISPLAY && currentselection == DISPLAYOPTIONS::FIRSTBG)
        {
            txt += graphics::backgrounds->at(settings::activations[currenttitle][currentselection]).name;
        }
        else if (currenttitle == OPTIONTYPE::SYSTEM && currentselection == SYSTEMOPTIONS::MUSIC)
        {
            txt += std::to_string(Mix_VolumeMusic(-1));
        }
        else if (currenttitle == OPTIONTYPE::SYSTEM && currentselection == SYSTEMOPTIONS::SOUNDS)
        {
            txt += std::to_string(Mix_Volume(-1, -1));
        }
        else
        {
            txt += settings::activations[currenttitle][currentselection] ? "Enabled" : "Disabled";
        }
        tickertext->render(320, 360, txt, true, 255, 0, 0, 0, false, 0, 0, 0);
    }
    tickertext->render(320, 400, details[currenttitle][currentselection], true, 255, 255, 255, 500, false, 0, 0, 0);

    switch (currentscreen)
    {
    case 0:
        break;
    case 1:
    {
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
        SDL_Rect splashbox = {0, 0, 640, 480};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
        splashbox = {320 - 240, 240 - 120, 480, 240};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
        SDL_RenderDrawRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
        double var = ((double)Mix_VolumeMusic(-1) / 128) * 58.0 + sin(time / 50) * 10;
        // drawTexture(graphics::render, texture.at(7), 80, 360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
        double coz = ((double)Mix_VolumeMusic(-1) / 128) * 58.0 + cos(time / 50) * 10;
        // drawTexture(graphics::render, texture.at(7), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);
        newhead->render(320, 140, "MUSIC VOLUME", true);
        tickertext->render(320, 180, std::to_string(Mix_VolumeMusic(-1)), true, 255, 0, 255, 0, false, 0, 0, 0);
        tickertext->render(320, 300, "EXIT", true);
        break;
    }
    case 2:
    {
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
        SDL_Rect splashbox = {0, 0, 640, 480};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
        splashbox = {320 - 240, 240 - 120, 480, 240};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
        SDL_RenderDrawRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
        double var = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + sin(time / 50) * 10;
        // drawTexture(graphics::render, texture.at(7), 80, 360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
        double coz = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + cos(time / 50) * 10;
        drawTexture(graphics::render, texture.at(7), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);

        newhead->render(320, 140, "SOUND VOLUME", true);
        tickertext->render(320, 180, std::to_string(Mix_Volume(-1, -1)), true);
        tickertext->render(320, 300, "EXIT", true);

        break;
    }
    case 3:
    {
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
        SDL_Rect splashbox = {0, 0, 640, 480};
        SDL_RenderFillRect(graphics::render, &splashbox);

        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
        splashbox = {320 - 240, 80, 480, 360};
        SDL_RenderFillRect(graphics::render, &splashbox);

        SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
        SDL_RenderDrawRect(graphics::render, &splashbox);

        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
        newhead->render(320, 88, "SELECT A BACKGROUND", true);

        tickertext->render(320, 120, "Current Selection: " + graphics::backgrounds->at(settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]).name, true, 255, 0, 255, 400, false, 0, 0, 0);
        int before = (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] - 1) >= 0 ? (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] - 1) : graphics::backgrounds->size() - 1;
        int after = (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + 1) > graphics::backgrounds->size() - 1 ? 0 : (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] + 1);

        drawTexture(graphics::backgrounds->at(settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]).thumbnail, 320 - 64, 156, 0, 1.0, false);
        drawTexture(graphics::backgrounds->at(before).thumbnail, 320 - 168, 185, 0, 0.5, false);
        drawTexture(graphics::backgrounds->at(after).thumbnail, 360, 185, 0, 0.5, false);

        tickertext->render(320, 284, "Name: " + graphics::backgrounds->at(settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]).name, true, 255, 0, 255, 0, false, 0, 0, 0);
        tickertext->render(320, 308, "Creator: " + graphics::backgrounds->at(settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]).creator, true, 255, 0, 255, 0, false, 0, 0, 0);
        tickertext->render(320, 332, "Song Name: " + graphics::backgrounds->at(settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]).songname, true, 255, 0, 255, 0, false, 0, 0, 0);
        tickertext->render(320, 356, "Song Creator: " + graphics::backgrounds->at(settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG]).artist, true, 255, 0, 255, 0, false, 0, 0, 0);

        tickertext->render(320, 408, "EXIT", true);

        break;
    }
    case 4:
    {
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
        SDL_Rect splashbox = {0, 0, 640, 480};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
        splashbox = {320 - 240, 240 - 120, 480, 240};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
        SDL_RenderDrawRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

        newhead->render(320, 140, "ARE YOU SURE?", true);
        tickertext->render(320, 180, "This will reset all of your settings!", true);
        tickertext->render(256, 300, "YES", true, (currentmenuselect == 0 ? 255 : 0), 0, 0, 0, false, 0, 0, 0);
        tickertext->render(384, 300, "NO", true, (currentmenuselect == 1 ? 255 : 0), 0, 0, 0, false, 0, 0, 0);
        break;
    }
    case 9:
    {
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
        SDL_Rect splashbox = {0, 0, 640, 480};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
        splashbox = {320 - 240, 240 - 120, 480, 240};
        SDL_RenderFillRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
        SDL_RenderDrawRect(graphics::render, &splashbox);
        SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

        newhead->render(320, 140, "UNFINISHED", true);
        tickertext->render(320, 180, "I swear it'll be here when I'm done...", true);
        tickertext->render(320, 300, "EXIT", true);
        break;
    }
    }

    // SDL_RenderPresent(renderer);
}
void options::logic(double deltatime)
{
    if (settings::activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::MOVINGBG])
    {
        background.logic(deltatime);
        cub->logic(deltatime);
        bigcub->logic(-deltatime);
        rot += deltatime;
        txtpos += deltatime / 100;
        bottompos += deltatime / 100;
    }
    if (mvright && settingsx > -320)
    {
        settingsx -= deltatime;
    }
    else if (mvright)
    {
        mvright = false;
        currenttitle++;
        settingsx = 320;
        currentselection = 0;
    }

    if (mvleft && settingsx < 960)
    {
        settingsx += deltatime;
    }
    else if (mvleft)
    {
        mvleft = false;
        currenttitle--;
        settingsx = 320;
        currentselection = 0;
    }
}
Transition options::endLogic()
{
    if (loadgame)
    {
        return {
            0.1,
            1,
            320,
            240,
            FADETYPE::FADE,
            true};
    }
    return Transition();
}

void options::reset()
{
    Mix_HaltMusic();
    if (Mix_PlayingMusic() == 0)
    {
        // Play the music
        Mix_PlayMusic(background.music, -1);
    }
    // If music is being played
    else
    {
        // If the music is paused
        if (Mix_PausedMusic() == 1)
        {
            // Resume the music
            Mix_ResumeMusic();
        }
        // If the music is playing
        else
        {
            // Pause the music
            Mix_PauseMusic();
        }
    }
}
void options::drawTexture(SDL_Texture *texture, int x, int y, double angle, double scale, bool center)
{
    SDL_Rect sprite;
    SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h);
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;
    if (center)
    {
        sprite.x = x - oldwidth / 2;
        sprite.y = y - oldheight / 2;
    }
    else
    {
        sprite.x = x + oldwidth / 2 - sprite.w / 2;
        sprite.y = y + oldheight / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(graphics::render, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}

void options::moveright()
{
    mvright = true;
}

// this is the new drawtexture, for drawing sprite sheets and whatnot
void options::drawTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch)
{
    SDL_Rect sprite;
    SDL_Rect srcrect = {srcx, srcy, srcw, srch};
    if (SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0)
    {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    sprite.w = srcw * scale;
    sprite.h = srch * scale;
    if (center)
    {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else
    {
        sprite.x = x + srcw / 2 - sprite.w / 2;
        sprite.y = y + srch / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(renderer, texture, &srcrect, &sprite, 0, NULL, SDL_FLIP_NONE);
}
