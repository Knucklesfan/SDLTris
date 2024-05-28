#include "credits.h"
#include "../utils/defs.h"
#include "../pixfont.h"
credits::credits() {
    // headerfont = fonts[0];
    // textfont = fonts[1];
    // textures = texts;
    cub = new wireframecube(320, 240, 640, 480);
    sineWave = new sine(0, 120, 320, 240);
    backg = new bg("./sprites/resultsbg",true);
    loadgame = false;
    sky = new skybox();
    
    
    p = new plane({0,-2,-50},{100,100,1},{-70,0,0});
    // rendertext = SDL_CreateTexture(graphics::render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 128, 128);
    // SDL_SetTextureBlendMode(rendertext, SDL_BLENDMODE_ADD);
    buff= new buffermanager(128,128,true);
}
void credits::input(SDL_Keycode key) {
    if(key == SDLK_z) {
        loadgame = true;
    }
    speed = 25;
    if (key == SDLK_x) {
        speed = 5;
    }
    if(key == SDLK_UP) {
        posx+=0.05;
    }
    if(key == SDLK_DOWN) {
        posx-=0.05;
    }
    if(key == SDLK_LEFT) {
        posy+=0.05;
    }
    if(key == SDLK_RIGHT) {
        posy-=0.05;
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
    // std::cout << wordsy << "\n";
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
    // cub->logic(deltaTime);

}
Transition credits::endLogic() {
    Transition t = Transition();
    if (loadgame) {
        t.gamemode = 1;
        t.transition = true;
    }
    return t;
}

void credits::reset() {
    Mix_HaltMusic();
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(audio::music->at(4), -1);
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
    networking::globalRPC->update("Watching the credits.", "Top high score: " + std::to_string(settings::maxscore), "icon2", std::time(nullptr));


}
void credits::render() {
    switch(gamemode) {
        case 0: {
            sky->render(0);
            glm::mat4 projection;
            projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
            glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
            graphics::shaders.at(9)->activate();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, graphics::cubemaps->at(0));
            glm::vec3 objColor = glm::vec3(1.0f, 0.5f, 0.31f);
            glm::vec3 lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);
            glm::vec3 lightpos = glm::vec3(posx, 0.5, posy);
            graphics::sprites.at("normalbrick")->activate(1);
            graphics::shaders.at(9)->setInt("normalMap",1);
            graphics::shaders.at(9)->setFloat("offset",SDL_GetTicks64()/10000.0f);

            graphics::shaders.at(9)->setVec3("objectColor", glm::value_ptr(objColor));
            graphics::shaders.at(9)->setVec3("lightColor", glm::value_ptr(lightcolor));
            graphics::shaders.at(9)->setVec3("lightPos", glm::value_ptr(lightpos));
            p->render(graphics::shaders.at(9),graphics::sprites.at("homophobicdog"),projection,view);
            graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["beachgridskyboxoverlay"],
    {0,0},{640,480},{0,0,0},{0,0},{640,480});   

            sineWave->render(255,255,255,255,0);
            sineWave->render(255,0,0,128,2);
            sineWave->render(0,0,255,128,-2);
            // cub->render(255, 0, 0);
            std::cout << posx << " " << posy << "\n";
            // graphics::sprite->render(graphics::shaders.at(4),&cub->buff->renderTexture,{0,0},{640,480},{0,0,0},{0,0},{640,480});
            drawRotatedBlock(16,16,lpiece,time / 15, 1);
            drawRotatedBlock(496,16,tpiece,time / -15, 2);
            drawRotatedBlock(496,336,lpiece,time / 15, 1);
            drawRotatedBlock(16,336,tpiece,time / -15, 2);


            int layers = 0;
            for(int i = 0; i < 26; i++) {
                int y = wordsy + layers;
                if (i >= 24) {
                    if (wordsy + layers < 224 + (i%24)*32) {
                        y = 224 + (i % 24) * 32;
                    }
                }
                if(y < 512) {
                    switch(wordprops[i]) {
                        case 1: {
                            graphics::fonts->at(1)->render(320, y, words[i], true,255,255,255, 580,false,0,0,0);

                            layers+=((pixfont*) graphics::fonts->at(0))->height;
                            break;
                        };
                        case 0: {
                            graphics::fonts->at(0)->render(320, y, words[i], true,255,255,255, 580,false,0,0,0);
                            layers+=((pixfont*) graphics::fonts->at(1))->height;
                            break;
                        };
                        case 2: {
                            graphics::fonts->at(1)->render(320, y, words[i], true,255,0,255, 580,true,time/500, 5,20);
                            // textfont->render(render, words[i],320, y, true,255,0,255,580,true,time/500, 5,20);
                            layers+=((pixfont*) graphics::fonts->at(0))->height;
                            break;
                        }

                    }
                    layers+=wordspacing[i];
                }
            }
            graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{0,0,0,alpha},false,1,glm::vec4(1,1,1,1));
            break;
        }
        case 1: {
            graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["thxforplaying"],
    {0,0},{640,480},{0,0,0},{0,0},{640,480});   
            graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{0,0,0,alpha},false,1,glm::vec4(1,1,1,1));
            // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255*alpha);
            // SDL_RenderFillRect(graphics::render, &splashbox);
            break;
        }
    }

}


void credits::drawRotatedBlock(int x, int y, const int position[], double angle, int texture) {
    buff->enable();
    drawCubes(position, 16, 32, 6, 3, 0, abs(sin(time / 500)) + 0.85, texture);
    buff->disable(640,480,true);
    graphics::sprite->render(graphics::shaders.at(4),&buff->renderTexture,{x,y},{128,128},angle,{0,0},{128,128});

    // drawTexture(graphics::render, rendertext, x, y, angle, 1.0, false);

}

void credits::drawCubes(const int position[], int x, int y, int size, int width, double angle, double scale, int texture) {

    for (int i = 0; i < size; i++) {
        if (position[i] > 0) {
            graphics::sprite->render(graphics::shaders.at(4),graphics::blocks->at(texture),{x + (i % width) * 32, y + (i / width) * 32},{16*scale,16*scale},{angle,0,0},{0,0},{16,16},{128,128});
            // graphics::drawTexture(graphics::blocks->at(texture), x + (i % width) * 32, y + (i / width) * 32, angle, scale, false);
        }
    }
}
