#include "gameplaymenu.h"
float easeInOutCubic(float x) {
    return -std::pow((2*x)-1, 2)+1;
}
gameplaymenu::gameplaymenu() {
    cd = new plane({0.75,-0.5,-1.5},{1,1,1},{0,0,0});
    cdPos = {0.75,-0.5,-80.5};
    startTime = SDL_GetTicks();
}
void gameplaymenu::input(SDL_Keycode keysym) {
    switch(currentscreen) {
        case 0: {
            switch(keysym) {
                case SDLK_LEFT: {
                    if(selection > 0) {
                        selection--;
                    }
                }break;
                case SDLK_RIGHT: {
                    if(selection < NUMBUTTONS) {
                        selection++;
                    }
                }break;
                case SDLK_UP: {
                    if(selection >= ROWWIDTH) {
                        selection-=ROWWIDTH;
                    }
                }break;
                case SDLK_DOWN: {
                    if(selection < ROWWIDTH) {
                        selection+=ROWWIDTH;
                    }
                }
            }
        }break;
    }
}
Transition gameplaymenu::endLogic() {
    Transition t;
    return t;
}
void gameplaymenu::logic(double deltatime) {
    Uint32 time = SDL_GetTicks()-startTime;
    if(time < 5000) {
        cdPos = glm::vec3(0.75-11.5*easeInOutCubic(time/5000.0), -0.5, utils::lerp(-80.5,-1.5,time/5000.0));
    }
    else {
        if(buttonx < 312) {
            buttonx = utils::lerp(buttonx,340,deltatime*0.005);
        }
        else {
            buttonx = 312;
        }
    }
}
void gameplaymenu::render() {
    Uint32 time = SDL_GetTicks()-startTime;
    if(time > 5000) {
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("menubackground"),{0,0},{640,480},0,{SDL_GetTicks()/10.0,SDL_GetTicks()/10.0},{640,480});
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("ring"),{-164,-164},{328,328},-(SDL_GetTicks()/100.0),{0,0},{328,328});
    }
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    view = glm::translate(view, glm::vec3(0.0, 0, 0.0)); 
    cd->position = cdPos;
    cd->rotation = glm::vec3(-70,0,(SDL_GetTicks()/10)%360);
    graphics::sprites.at("beachgridwords")->activate(1);
    graphics::sprites.at("cdrom-reflectionmap")->activate(2);
    cd->render(graphics::shaders.at(7),graphics::sprites.at("cdrom"),projection,view);
    if(time > 5000) {
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("storymenubutton"),{-300+buttonx,116},{300,128},0,{selection==0?300:0,0},{300,128});
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("classicmenubutton"),{640-buttonx,116},{300,128},0,{selection==1?300:0,0},{300,128});
        
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("optionsmenubutton"),{52,260},{128,64},0,{selection==2?128:0,0},{128,64});
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("highscoremenubutton"),{52+128+8,260},{128,64},0,{selection==3?128:0,0},{128,64});
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("creditsmenubutton"),{52+128+8+128+8,260},{128,64},0,{selection==4?128:0,0},{128,64});
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("exitmenubutton"),{52+128+8+128+8+128+8,260},{128,64},0,{selection==5?128:0,0},{128,64});

    
    }
};
void gameplaymenu::reset() {
    Mix_PlayMusic( audio::music->at(1), -1 );
    startTime = SDL_GetTicks();
}
