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
}
void classicmenu::render() {
    background->render();
    cube->render(0,0,255);
    graphics::sprite->render(graphics::shaders.at(4),&cube->buff->renderTexture,{0,0},{640,480},{0,0,0},{0,0},{640,480});
    for(int i = 0; i < 6; i++) {
        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("savefile"),{34,84+i*116},{250,100},0,{0,0},{250,100});
    }

};
void classicmenu::reset() {
    startTime = SDL_GetTicks();
    t = Transition();
}
