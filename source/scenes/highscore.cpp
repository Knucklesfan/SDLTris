#include "highscore.h"
#include "../utils/defs.h"


highscore::highscore() {
    spaceBuff = new buffermanager(614,406,true);
    refreshBackdrop = true;
    logo = new plane({0,0,-2},{1,71.0/235.0,1},{0,0,0});
    pres = new plane({0,0,-2},{1,67.0/219.0,1},{0,0,0});
    animationindex = 0;
    animationtime = 1000;
    animationstart = SDL_GetTicks64();

    memset(starX, 0, sizeof(starX));
    memset(starY, 0, sizeof(starY)); //clear out the coordinates for the stars
    memset(starZ, 0, sizeof(starZ));

    for(int i = 0; i < 255; i++) {
        starX[i] = (rand() % 640) - 320;
        starY[i] = (rand() % 480) - 240;
        starZ[i] = (rand() % 1700) - 100;

    }

};
void highscore::input(SDL_Keycode key) {

};
void highscore::render() {
    graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("segacd_bg"),
    {0,0},{640,480},0,{0,0},{640,480});
    spaceBuff->enable();
    glEnable(GL_DEPTH_TEST);
        for(int i = 0; i < 255; i++) {
            float perspective = 300.0 / (300.0 - starZ[i]);
            int x = 320 + starX[i] * perspective;
            int y = 240 + starY[i] * perspective;

            starZ[i] += 2;
            if (starZ[i] > 300) {
                starX[i] = (rand() % 640) - 320;
                starY[i] = (rand() % 480) - 240;

                starZ[i] -= 600;
            }
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("star"),{x,y},{8*(perspective / 2),8*(perspective / 2)},{0,0,starZ[i]*4},{0,0},{8,8},{614,406},(perspective / 2));            
        }
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    if(refreshBackdrop) {
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("segacd_planet"),
        {0,0},{614,406},{0,0,0},{0,0},{614,406},{614,406},0.0f);
    }
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)614 / (float)406, 0.001f, 10000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    view = glm::translate(view, glm::vec3(0.0, 0, 0.0)); 

    graphics::shaders.at(13)->activate();
    graphics::shaders.at(13)->setVec4("palette",palette,8);
    graphics::sprites.at("knfnlogo_cd")->activate(0);
    graphics::sprites.at("knfnrainbow")->activate(1);
	graphics::shaders.at(13)->setInt("texture1",0);
	graphics::shaders.at(13)->setInt("texture2",1);

    glEnable(GL_DEPTH_TEST);
    if(logo->position.z < pres->position.z) {
        logo->render(graphics::shaders.at(13),graphics::sprites.at("knfnlogo_cd"),projection,view);
        pres->render(graphics::shaders.at(0),graphics::sprites.at("knuxfan"),projection,view);
    }
    else {
        pres->render(graphics::shaders.at(0),graphics::sprites.at("knuxfan"),projection,view);
        logo->render(graphics::shaders.at(13),graphics::sprites.at("knfnlogo_cd"),projection,view);
    }
    glDisable(GL_DEPTH_TEST);

    spaceBuff->disable(640,480,true);
    graphics::sprite->render(graphics::shaders.at(4),&spaceBuff->renderTexture,{13,37},{614,406},0,{0,0},{614,406});
};

void highscore::logic(double deltatime) {
    std::cout << SDL_GetTicks64()-animationstart << "\n";
    float animationframe = (SDL_GetTicks64()-animationstart)/animationtime;
    switch(animationindex) {
        case 0: {
            if(SDL_GetTicks64()-animationstart >= animationtime) {
                animationindex++;
                animationtime = 2000;
                animationstart = SDL_GetTicks64();
            }
        }break;
        case 1: {
            float sinVal = 1-(SDL_GetTicks64()-animationstart)/animationtime;
            float cosVal = (SDL_GetTicks64()-animationstart)/animationtime;

            logo->position.z = -2-16*(sinVal);
            logo->position.y = 2*math::easeInOutCubic(abs(sinVal));
            logo->position.x = 2*math::easeInOutCubic(abs(sinVal));
            // logo->position.x = -2*std::pow((2*(1+sin(SDL_GetTicks64()/1000.0)/2))-1, 2)+1;
            pres->position.z = -2-16*(cosVal);
            pres->position.y = -2*math::easeInOutCubic(abs(cosVal));
            pres->position.x = -2*math::easeInOutCubic(abs(cosVal));
            if(SDL_GetTicks64()-animationstart >= animationtime) {
                animationindex++;
                animationtime = 4000;
                animationstart = SDL_GetTicks64();
            }

        }break;
        case 2: {
            if(SDL_GetTicks64()-animationstart >= animationtime) {
                animationindex++;
                animationtime = 2000;
                animationstart = SDL_GetTicks64();
            }
        }break;
        case 3: {
            float sinVal = 1-(SDL_GetTicks64()-animationstart)/animationtime;
            float cosVal = (SDL_GetTicks64()-animationstart)/animationtime;

            pres->position.z = -2-16*(sinVal);
            pres->position.y = 2*math::easeInOutCubic(abs(sinVal));
            pres->position.x = 2*math::easeInOutCubic(abs(sinVal));
            // logo->position.x = -2*std::pow((2*(1+sin(SDL_GetTicks64()/1000.0)/2))-1, 2)+1;
            logo->position.z = -2-16*(cosVal);
            logo->position.y = -2*math::easeInOutCubic(abs(cosVal));
            logo->position.x = -2*math::easeInOutCubic(abs(cosVal));
            if(SDL_GetTicks64()-animationstart >= animationtime) {
                animationindex=0;
                animationtime = 4000;
                animationstart = SDL_GetTicks64();
            }

        }break;

    }
    if(SDL_GetTicks64()-fakeDelta > 132) {
        glm::vec4 temp = {palette[0],palette[1],palette[2],palette[3]}; //store the first 4 colors
        for(int i = 0; i < 28; i+=4) {
            palette[i] = palette[i+4];
            palette[i+1] = palette[i+5];
            palette[i+2] = palette[i+6];
            palette[i+3] = palette[i+7];
        }
        palette[28] = temp.x;
        palette[29] = temp.y;
        palette[30] = temp.z;
        palette[31] = temp.w;
        fakeDelta = SDL_GetTicks64();
    }

};
Transition highscore::endLogic() {
    return Transition();
};

void highscore::handleAnimation() {
    switch(animationindex) {
        case 0: {

        }break;
    }
}

void highscore::reset() {
    Mix_PlayMusic(audio::music->at(7),-1);
    animationindex = 0;
    animationtime = 4000;
    animationstart = SDL_GetTicks64();
}