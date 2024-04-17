#include "classicmenu.h"

classicmenu::classicmenu() {
    cd = new plane({0.75,-0.5,-1.5},{1,1,1},{0,0,0});
    background = new bg("classicmenu",false);
    startTime = SDL_GetTicks();
}
void classicmenu::input(SDL_Keycode keysym) {
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
    Mix_PlayMusic( audio::music->at(3), -1 );
    startTime = SDL_GetTicks();
    t = Transition();
}
