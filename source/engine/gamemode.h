#include <SDL2/SDL.h>
#include <string>
#pragma once

enum FADETYPE {
    FADE,
    BARS, //unimplemented
    BLOCKS,
    CIRCLE //unimplemented
};
struct Transition {
    double speed = 0.001;
    int gamemode = 0;
    int originX = 320;
    int originY = 240;
    FADETYPE fade = FADETYPE::FADE;
    bool transition = false;
};

class Gamemode {
    public:
    virtual void logic(double deltaTime) {};
    virtual Transition endLogic() {return Transition();};
    virtual void input(SDL_Keycode keysym) {};
    virtual void render() {};
    virtual void reset() {};
    std::string name;

};

