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

};
void highscore::input(SDL_Keycode key) {

};
void highscore::render() {
    graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("segacd_bg"),
    {0,0},{640,480},0,{0,0},{640,480});
    spaceBuff->enable();
    if(refreshBackdrop) {
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("segacd_planet"),
        {0,0},{640,480},0,{0,0},{640,480});
    }
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)614 / (float)406, 0.001f, 10000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    view = glm::translate(view, glm::vec3(0.0, 0, 0.0)); 
    glEnable(GL_DEPTH_TEST);
    if(logo->position.z < pres->position.z) {
        logo->render(graphics::shaders.at(0),graphics::sprites.at("knfnlogo"),projection,view);
        pres->render(graphics::shaders.at(0),graphics::sprites.at("knuxfan"),projection,view);
    }
    else {
        pres->render(graphics::shaders.at(0),graphics::sprites.at("knuxfan"),projection,view);
        logo->render(graphics::shaders.at(0),graphics::sprites.at("knfnlogo"),projection,view);
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
                animationtime = 1000;
                animationstart = SDL_GetTicks64();
            }

        }break;
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
    animationindex = 0;
    animationtime = 2000;
    animationstart = SDL_GetTicks64();
}