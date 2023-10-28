#pragma once
#include "../gamemode.h"
#include "../utils/defs.h"
class white : public Gamemode { //literally just renders a white screen to test transitions
    int r = 128 + rand() % (256 - 128);
    int g = 128 + rand() % (256 - 128);
    int b = 128 + rand() % (256 - 128);
    bool advance = false;
    int transition = 0;
    int background = 0;
    std::string transitions[4] = {
        "FADE",
        "BARS",
        "BLOCKS",
        "CIRCLE"
    };
    void render() {
        #ifdef __LEGACY_RENDER
        SDL_SetRenderDrawColor(graphics::render,r,g,b,255);
        SDL_RenderClear(graphics::render);
        SDL_SetRenderDrawColor(graphics::render,0,0,0,255);
        graphics::fonts->at(2)->render(16,16,"Currently selected transition: "+transitions[transition],false);
        graphics::fonts->at(2)->render(16,32,"Left decreases Right increases",false);
        #else
        if(background > 0) {
            graphics::backgrounds->at(background-1).render();
        }
        else {
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        }
        graphics::fonts->at(2)->render(16,16,"Currently selected transition: "+transitions[transition],false);
        graphics::fonts->at(2)->render(16,32,"Currently selected bg: "+std::to_string(background),false);
        graphics::fonts->at(2)->render(16,48,"Left -trans Right +trans Down -bg Up +bg",false);


        #endif

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
             case SDLK_UP: {
                if(background < graphics::backgrounds->size()) {
                    background++;
                }
                break;
            }
            case SDLK_DOWN: {
                if(background > 0) {
                    background--;
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
    void logic(double deltatime) {
        if(background > 0) {
            graphics::backgrounds->at(background-1).logic(deltatime);
        }
    }
    void reset() {
        r = 128 + rand() % (256 - 128);
        g = 128 + rand() % (256 - 128);
        b = 128 + rand() % (256 - 128);
    }
};