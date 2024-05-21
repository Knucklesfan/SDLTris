#include "debugscene.h"
debugscene::debugscene() {
    memset(starX, 0, sizeof(starX));
    memset(starY, 0, sizeof(starY)); //clear out the coordinates for the stars
    memset(starZ, 0, sizeof(starZ));
    memset(starBlock, 0, sizeof(starBlock));

    for(int i = 0; i < 255; i++) {
        starX[i] = (rand() % 640) - 320;
        starY[i] = (rand() % 480) - 240;
        starZ[i] = (rand() % 1700) - 100;
        starBlock[i] = (rand() % graphics::blocks->size()-1);

    }

}
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
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("cheatercroc"),{0,0},{640,480},0,{0,0},{640,480});
            glEnable(GL_DEPTH_TEST);
            for(int i = 0; i < 127; i++) {
                float perspective = 300.0 / (300.0 - starZ[i]);
                int x = 320 + starX[i] * perspective;
                int y = 240 + starY[i] * perspective;

                starZ[i] += 2;
                if (starZ[i] > 300) {
                    starZ[i] -= 600;
                }
                graphics::sprite->render(graphics::shaders.at(4),graphics::blocks->at(starBlock[i]),{x,y},{16*(perspective / 2),16*(perspective / 2)},{0,0,starZ[i]*4},{0,0},{16,16},{640,480},(perspective / 2));            
            }
            glDisable(GL_DEPTH_TEST);
        }

        graphics::fonts->at(1)->render(320,60,"WELCOME TO",true);
        graphics::fonts->at(1)->render(320,120,"KNUXFANS TETRIMINOS",true,((int)(SDL_GetTicks()/100.0)%3==0)*255,((int)(SDL_GetTicks()/100.0)%3==1)*255,((int)(SDL_GetTicks()/100.0)%3==2)*255,-1,true,SDL_GetTicks()/1000.0,10,10);
        for(int i = 0; i<MENUSIZE; i++) {
            std::string text = menu[i];
            if(endings[i] != nullptr) {
                text.append(std::to_string(*(endings[i])));
            }
            if(i == 2) {
                text.append((gameplay::gamemodes.at(gamemode)->name));
            }
            graphics::fonts->at(0)->render(320,200+(i*24),text,true,255,selection==i?0:255,255,-1,false,0,0,0);
        }

        #endif

    }

void debugscene::input(SDL_Keycode keysym)
{
    switch(keysym) {
        case SDLK_RIGHT: {
            switch(selection) {
                case 0: {
                    if(background < graphics::backgrounds->size()) {
                        background++;
                    }

                }break;
                case 1: {
                    if(transition < 3) {
                        transition++;
                    }
                }break;
                case 2: {
                    if(gamemode < gameplay::gamemodes.size()-1) {
                        gamemode++;
                    }
                }break;

            }
            break;
        }
        case SDLK_LEFT: {
            switch(selection) {
                case 0: {
                    if(background > 0) {
                        background--;
                    }

                }break;
                case 1: {
                    if(transition > 0) {
                        transition--;
                    }
                }break;
                case 2: {
                    if(gamemode > 0) {
                        gamemode--;
                    }
                }break;

            }
            break;
        }
            case SDLK_UP: {
            if(selection > 0) {
                selection--;
            }
            break;
        }
        case SDLK_DOWN: {
            if(selection < MENUSIZE-1) {
                selection++;
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
    return {
        0.001,
        gamemode,
        320,240,
        (FADETYPE)transition,
        advance
    };
};
void debugscene::logic(double deltatime) {
    if(background > 0) {
        graphics::backgrounds->at(background-1).logic(deltatime);
    }
}
void debugscene::reset() {
    advance = false;
    Mix_HaltMusic();

    r = 128 + rand() % (256 - 128);
    g = 128 + rand() % (256 - 128);
    b = 128 + rand() % (256 - 128);
}