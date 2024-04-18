#include "classicmenu.h"

classicmenu::classicmenu() {
    cd = new plane({0.75,-0.5,-1.5},{1,1,1},{0,0,0});
    background = new bg("classicmenu",false);
    startTime = SDL_GetTicks();
}
void classicmenu::input(SDL_Keycode keysym) {
    switch(currentscreen) {
        case 0: {
            switch(keysym) {
                case SDLK_x: {
                    Mix_CrossFadeMusicStream(audio::music->at(3),audio::music->at(1),-1,1000,0);
                    t.gamemode = gameplay::gamemode-1;
                    t.transition = 1;
                    t.fade = BARS;
                    Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                }break;
            }
        }break;
    }
}
Transition classicmenu::endLogic() {
    return t;
}
void classicmenu::logic(double deltatime) {
    background->logic(deltatime);
}
void classicmenu::render() {
    background->render();
};
void classicmenu::reset() {
    startTime = SDL_GetTicks();
    t = Transition();
}
