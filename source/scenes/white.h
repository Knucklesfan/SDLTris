#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
class white : public Gamemode { //literally just renders a white screen to test transitions
    int r = 128 + rand() % (256 - 128);
    int g = 128 + rand() % (256 - 128);
    int b = 128 + rand() % (256 - 128);
    bool advance = false;
    int transition = 0;
    std::string transitions[4] = {
        "FADE",
        "BARS",
        "BLOCKS",
        "CIRCLE"
    };
    void render() {
        SDL_SetRenderDrawColor(graphics::render,r,g,b,255);
        SDL_RenderClear(graphics::render);
        SDL_SetRenderDrawColor(graphics::render,0,0,0,255);
        graphics::fonts->at(2)->render(16,16,"Currently selected transition: "+transitions[transition],false);
        graphics::fonts->at(2)->render(16,32,"Left decreases Right increases",false);
    }
    void input(SDL_Keycode keysym) {
        switch(keysym) {
            case SDLK_RIGHT: {
                if(transition < 3) {
                    transition++;
                }
                break;
            }
            case SDLK_LEFT: {
                if(transition > 0) {
                    transition--;
                }
                break;
            }
            case SDLK_z: {
                if(!advance) {
                    Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
                    advance = true;
                }
            }
        }
    }
    Transition endLogic() {
        if(advance) {
            advance = false;
            return {
                0.001,
                0,
                320,240,
                (FADETYPE)transition,
                true
            };
        }
        else {
            return {
                0.001,
                0,
                320,240,
                (FADETYPE)transition,
                false
            };

        }
    };
    void reset() {
        r = 128 + rand() % (256 - 128);
        g = 128 + rand() % (256 - 128);
        b = 128 + rand() % (256 - 128);
    }
};