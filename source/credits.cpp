#include "credits.h"

credits::credits(SDL_Renderer* renderer,std::vector<font*> fonts, bg* background, std::vector<SDL_Texture*>* texts) {
    headerfont = fonts[0];
    textfont = fonts[1];
    textures = texts;
    cub = new cube(renderer, 320, 240, 640, 480);
    sineWave = new sine(renderer, 0, 120, 320, 240);
    backg = background;
    loadgame = false;
    rendertext = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 128, 128);
    SDL_SetTextureBlendMode(rendertext, SDL_BLENDMODE_ADD);

}
void credits::keyPressed(SDL_Keycode key) {
    if(key == SDLK_z) {
        loadgame = true;
    }
    speed = 25;
    if (key == SDLK_x) {
        speed = 5;
    }
}
void credits::logic(double deltaTime) {
    if(wordsy < -1900 && !goup) {
        alpha = 0.0;
        goup = true;
    }
    if(godown && alpha > 0.0) {
        alpha -= deltaTime/1000;
    }
    else if(godown) {
        godown = false;
    }
    if(goup && alpha < 1.0) {
        alpha += deltaTime/1000;
    }
    else if(goup) {
        goup = false;
        gamemode = 1;
        godown = true;
    }
    std::cout << wordsy << "\n";
    if(gamemode == 0) {
        wordsy -= deltaTime/speed;
    }
    else {
        wordsy = 0;
    }
    sineWave->logic(deltaTime);
    backg->logic(deltaTime);
    if(alpha < 0.0) {
        alpha = 0.0;
    }
    if(alpha > 1.0) {
        alpha = 1.0;
    }

    time += deltaTime;
    cub->logic(deltaTime);

}
int credits::endlogic() {
    if(loadgame) {
        return 1;
    }
    return 0;
}

void credits::reset(Mix_Music* music) {
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
    loadgame = false;
    wordsy = 512;
    alpha = 0.0;
    goup = false;
    godown = false;
    gamemode = 0;


}
void credits::render(SDL_Renderer* render) {
    switch(gamemode) {
        case 0: {
            backg->render(render, false);
            sineWave->render(render,255,255,255,255,0);
            sineWave->render(render,255,0,0,128,2);
            sineWave->render(render,0,0,255,128,-2);
            cub->render(render, 255, 0, 0);
            drawTexture(render, cub->texture, 0, 0, 0, 1.0, false);
            drawRotatedBlock(render, 16,16,lpiece,time / 15, 1);
            drawRotatedBlock(render, 496,16,tpiece,time / -15, 2);
            drawRotatedBlock(render, 496,336,lpiece,time / 15, 1);
            drawRotatedBlock(render, 16,336,tpiece,time / -15, 2);


            int layers = 0;
            for(int i = 0; i < 26; i++) {
                int y = wordsy + layers;
                if (i >= 24) {
                    if (wordsy + layers < 224 + (i%24)*32) {
                        y = 224 + (i % 24) * 32;
                    }
                }

                switch(wordprops[i]) {
                    case 1: {
                        textfont->render(320, y, words[i], true, render,580);
                        layers+=textfont->height;
                        break;
                    };
                    case 0: {
                        headerfont->render(320, y, words[i], true, render,580);
                        layers+=headerfont->height;
                        break;
                    };
                    case 2: {
                        textfont->render(render, words[i],320, y, true,255,0,255,580,true,time/500, 5,20);
                        layers+=headerfont->height;
                        break;
                    }

                }
                layers+=wordspacing[i];
            }
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255*alpha);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_RenderFillRect(render, &splashbox);
            break;
        }
        case 1: {
            SDL_RenderCopy(render, textures->at(13), NULL, NULL);
            SDL_Rect splashbox = { 0, 0, 640, 480 };
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255*alpha);
            SDL_RenderFillRect(render, &splashbox);
            break;
        }
    }

}

void credits::drawTexture(SDL_Renderer* render, SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
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
    SDL_RenderCopyEx(render, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}

void credits::drawRotatedBlock(SDL_Renderer* renderer,int x, int y, const int position[], double angle, int texture) {
    SDL_Texture* temp = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, rendertext);
    SDL_RenderClear(renderer);
    drawCubes(renderer, position, 16, 32, 6, 3, textures, 0, abs(sin(time / 500)) + 0.85, texture);
    SDL_SetRenderTarget(renderer, temp);
    drawTexture(renderer, rendertext, x, y, angle, 1.0, false);

}

void credits::drawCubes(SDL_Renderer* renderer, const int position[], int x, int y, int size, int width, std::vector<SDL_Texture*>* textures, double angle, double scale, int texture) {

    for (int i = 0; i < size; i++) {
        if (position[i] > 0) {
            drawTexture(renderer,textures->at(texture), x + (i % width) * 32, y + (i / width) * 32, angle, scale, false);
        }
    }
}
