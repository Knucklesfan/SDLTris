#include "debugscene.h"
void debugscene::render()
{
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
        }
        graphics::fonts->at(1)->render(320,100,"WELCOME TO KNUXFANS TETRIMINOS",true,((int)(SDL_GetTicks()/100.0)%3==0)*255,((int)(SDL_GetTicks()/100.0)%3==1)*255,((int)(SDL_GetTicks()/100.0)%3==2)*255,-1,true,SDL_GetTicks()/1000.0,10,10);
        for(int i = 0; i<MENUSIZE; i++) {
            std::string text = menu[i];
            if(endings[i] != nullptr) {
                text.append(std::to_string(*(endings[i])));
            }
            graphics::fonts->at(1)->render(320,0+(i*24),text,true);
        }

        #endif

    }

void debugscene::input(SDL_Keycode keysym)
{
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
Transition debugscene::endLogic()
{
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
void debugscene::logic(double deltatime) {
    if(background > 0) {
        graphics::backgrounds->at(background-1).logic(deltatime);
    }
}
void debugscene::reset() {
    r = 128 + rand() % (256 - 128);
    g = 128 + rand() % (256 - 128);
    b = 128 + rand() % (256 - 128);
}