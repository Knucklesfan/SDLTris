#include "classicmenu.h"
#include <iostream>
#include <fstream>

classicmenu::classicmenu() {
    cd = new plane({0.75,-0.5,-1.5},{1,1,1},{0,0,0});
    background = new bg("classicmenu",false);
    startTime = SDL_GetTicks();
    cube = new wireframecube(320,240,640,480);
    std::fill_n(savedatatest, 480, 0);


    { //this is temp code that will eventually be placed in utils to load save files and unload them.
        std::streampos size;
        char * memblock;
        std::ifstream file ("example.bin", std::ios::in|std::ios::binary|std::ios::ate);
        if (file.is_open())
        {
            size = file.tellg();
            memblock = new char [size];
            file.seekg (0, std::ios::beg);
            file.read (memblock, size);
            file.close();

            std::cout << "the entire file content is in memory";
            size_t offset = 0;
            offset += sizeof(uint);
            offset += sizeof(uint);
            offset += sizeof(int);
            offset += sizeof(int);
            offset += sizeof(int);
            offset += sizeof(int);

            offset += sizeof(int);
            offset += sizeof(int);
            offset += sizeof(Uint32);

            memcpy(&level, memblock+offset, sizeof(int)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(int);
            memcpy(&lines, memblock+offset, sizeof(int)); //very memory unsafe, please do not supply bad savestates...
            offset += sizeof(int);

            memcpy(&savedatatest, memblock+offset, sizeof(int[480])); //very memory unsafe, please do not supply bad savestates...

            offset += sizeof(int[480]);
            delete memblock;
        }
        for(int i = 0; i < 240; i++) {
            std::cout << savedatatest[i];
            if(i%10 == 0) {
                std::cout << "\n";
            }
        }
    }
}
void classicmenu::input(SDL_Keycode keysym) {
    switch(currentscreen) {
        case 0: {
            switch(keysym) {
                case SDLK_x: {
                    Mix_FadeOutMusic(500);
                    Mix_HookMusicFinished([](){ //very cool lambda function to replace crossfademusicstream
                        Mix_FadeInMusic(audio::music->at(1),-1,500); //dear C++, please explain: how does this work?
                        Mix_HookMusicFinished(NULL); //THIS IS INSANITY!
                    });
                    t.gamemode = gameplay::gamemode-1;
                    t.transition = 1;
                    t.fade = BARS;
                    Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                }break;
                case SDLK_z: {
                    t.gamemode = 4;
                    t.transition = 1;
                    t.fade = BARS;
                    Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                }break;
                case SDLK_DOWN: {
                    if(selection < savenum) {
                        transition += 1.0f;
                        selection++;
                    }
                }break;
                case SDLK_UP: {
                    if(selection > 0) {
                        transition -= 1.0f;
                        selection--;
                    }
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
    cube->logic(deltatime);
    if(transition > 0) {
        if(transition-deltatime*0.01 < 0) {
            transition = 0;
        }
        else {
            transition-=deltatime*0.01;
        }
    }
    else {
        if (transition < 0) {
            if(transition+(deltatime*0.01) > 0) {
                transition = 0;
            }
            else {
                transition +=deltatime*0.01;
            }
        }
    }
}
void classicmenu::render() {
    background->render();
    cube->render(0,0,255);
    graphics::sprite->render(graphics::shaders.at(4),&cube->buff->renderTexture,{0,0},{640,480},{0,0,0},{0,0},{640,480});
    for(int i = 0; i < savenum; i++) {
        graphics::shaders.at(5)->activate();
        glm::vec4 othershade = {1,1,0,1};
        glm::vec4 baseshade = {1,1,1,1};

        if(i == selection) {
            graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(othershade));
        }
        else {
            graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(baseshade));

        }
        graphics::sprite->render(graphics::shaders.at(5),graphics::sprites.at("savefile"),
        {34,
        200+i*116-(selection*116)+(116*transition)
        },
        {250,100},0,{0,0},{250,100});
        for(int j = 0; j < 240; j++) {
            if(savedatatest[j] > 0 && savedatatest[j]-1 < graphics::blocks->size()) {
                graphics::shaders.at(4)->activate();
                graphics::sprite->render(
                    graphics::shaders.at(4),
                    graphics::blocks->at(savedatatest[j]-1),
                    {
                        34+198+(j%10)*4,
                        200+(i*116)+(j/10)*4-(selection*116)+(116*transition)
                    },
                    {
                        4,
                        4
                    },
                    0,
                    {0,0},
                    {4,4}
                );
            }
        }
        if((SDL_GetTicks()/500)%2 ==0)
        graphics::fonts->at(4)->render(320,64,"LOAD SAVE",true);

    }

};
void classicmenu::reset() {
    networking::globalRPC->update("Getting ready to start a classic game...", "Top high score: " + std::to_string(settings::maxscore), "icon1", std::time(nullptr));
    startTime = SDL_GetTicks();
    t = Transition();
}
