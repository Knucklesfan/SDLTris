#include "nullscene.h"
#include "SDL2/SDL_keycode.h"

nullscene::nullscene() {
    backg = bg("null",false);
}
void nullscene::render() {
    backg.render();
}
void nullscene::input(SDL_Keycode key) {
    //empty
}

void nullscene::logic(double deltatime) {
    backg.logic(deltatime);
}

Transition nullscene::endLogic() {
    return Transition();
}

void nullscene::reset() {
    Mix_PlayMusic( backg.music, -1 );

}