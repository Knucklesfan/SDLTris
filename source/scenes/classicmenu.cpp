#include "classicmenu.h"
#include "SDL2/SDL_timer.h"

classicmenu::classicmenu() {
    cd = new plane({0.75,-0.5,-1.5},{1,1,1},{0,0,0});
    background = new bg("classicmenu",false);
    startTime = SDL_GetTicks();
    cube = new wireframecube(320,240,640,480);
    std::fill_n(savedatatest, 480, 0);


    // { //this is temp code that will eventually be placed in utils to load save files and unload them.
    //     std::streampos size;
    //     char * memblock;
    //     std::ifstream file ("example.bin", std::ios::in|std::ios::binary|std::ios::ate);
    //     if (file.is_open())
    //     {
    //         size = file.tellg();
    //         memblock = new char [size];
    //         file.seekg (0, std::ios::beg);
    //         file.read (memblock, size);
    //         file.close();

    //         std::cout << "the entire file content is in memory";
    //         size_t offset = 0;
    //         offset += sizeof(uint);
    //         offset += sizeof(uint);
    //         offset += sizeof(int);
    //         offset += sizeof(int);
    //         offset += sizeof(int);
    //         offset += sizeof(int);

    //         offset += sizeof(int);
    //         offset += sizeof(int);
    //         offset += sizeof(Uint32);

    //         memcpy(&level, memblock+offset, sizeof(int)); //very memory unsafe, please do not supply bad savestates...
    //         offset += sizeof(int);
    //         memcpy(&lines, memblock+offset, sizeof(int)); //very memory unsafe, please do not supply bad savestates...
    //         offset += sizeof(int);

    //         memcpy(&savedatatest, memblock+offset, sizeof(int[480])); //very memory unsafe, please do not supply bad savestates...

    //         offset += sizeof(int[480]);
    //         delete memblock;
    //     }
    //     for(int i = 0; i < 240; i++) {
    //         std::cout << savedatatest[i];
    //         if(i%10 == 0) {
    //             std::cout << "\n";
    //         }
    //     }
    // }
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
                    if(selection > 0) { //if we loadin, then we loading
                        settings::saveload = settings::saveDir + "/" + settings::saveCache.at(selection-1).name;
                        t.gamemode = 4;
                        t.transition = 1;
                        t.fade = BARS;
                        std::cout << "Loading Save: "<<settings::saveload << "\n";
                        Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                    }
                    else { //otherwise lets do some other wacky stuff
                        //trigger animation to bring up the actual gamemaking menu here
                        //queue all the extras!
                        transitionTime = SDL_GetTicks64();
                    }

                }break;
                case SDLK_DOWN: {
                    if(selection < settings::saveCache.size()) {
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
    switch(screenmode) {
        case 0: {
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
            if(transitionTime > 0 && SDL_GetTicks64()-transitionTime > TRANSITION_LENGTH) {
                screenmode++;
                transitionTime = 0;
            }
        }break;
        case 1: {

        }break;

    }

}

void classicmenu::render() {
    switch(screenmode) {
        case 0: {
            background->render();
            graphics::rect->render(graphics::shaders.at(1), {0,0}, {640,480}, 0, {0,0,0,transitionTime>0?(SDL_GetTicks64()-transitionTime)/TRANSITION_LENGTH:0}, false, 0, {0,0,0,1});
            cube->render(0,0,255);
            graphics::sprite->render(graphics::shaders.at(4),&cube->buff->renderTexture,{0,
                        SDL_GetTicks64()-transitionTime>(FALLTIME_OTHERS+8*FALLTIME_OTHERS_INTERVAL)&&transitionTime>0?
                                
                        math::easeOutBounce((SDL_GetTicks64()-transitionTime-(FALLTIME_OTHERS+8*FALLTIME_OTHERS_INTERVAL))/(FALLTIME_OTHERS))
                    :0,
            },{640,480},{0,0,0},{0,0},{640,480});
            if(selection != 0) {
                    //also render warning msg
                    graphics::fonts->at(0)->render(288,400-24,"WARNING!",false,255,0,0,-1,false,0,0,0);
                    graphics::fonts->at(2)->render(288,400,"Playing a saved game will delete the save\nupon game over! If you hate this horrible,\nterrible tragedy, then please purchase from\nthe upgrades shop the upgrade titled\n", false);
                    graphics::fonts->at(2)->render(288,400+32,"'please for the love of god stop deleting\nmy save files please help me im begging you'", false,64,255,64,-1,false,0,0,0);
                    graphics::fonts->at(2)->render(288,400+32+16,"to avoid any unforeseen consequences.\nWe appreciate your cooperation.\nThank you, management.", false);

            }
            for(int i = 0; i < settings::saveCache.size()+1; i++) {
                graphics::shaders.at(5)->activate();
                glm::vec4 othershade = {1,1,0,1};
                glm::vec4 baseshade = {0,0.5,1,1};

                if(i == selection) {
                    graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(othershade));
                }
                else {
                    graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(baseshade));

                }
                if(i == 0) { //use the cool newgame sprite
                    graphics::sprite->render(graphics::shaders.at(5),graphics::sprites.at("newgame"),
                    {transitionTime<=0?34:34+(640*(SDL_GetTicks64()-transitionTime)/500.0),
                    200+i*116-(selection*116)+(116*transition)
                    },
                    {250,100},0,{0,0},{250,100});
                }
                else { //use the normal, boring sprite
                    graphics::sprite->render(graphics::shaders.at(5),graphics::sprites.at("savefile"),
                    {34,

                    SDL_GetTicks64()-transitionTime>(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL)&&transitionTime>0?
                                (200+i*116-(selection*116)+(116*transition))
                        +math::easeOutBounce((SDL_GetTicks64()-transitionTime-(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL))/FALLTIME_OTHERS)
                    :(200+i*116-(selection*116)+(116*transition)),

                    },
                    {250,100},0,{0,0},{250,100});
                }
                if(i > 0) { //if we're rendering a normal, game save, then lets go ahead and do that
                    graphics::fonts->at(0)->render(34+4,

                    SDL_GetTicks64()-transitionTime>(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL)&&transitionTime>0?
                                (200+i*116-(selection*116)+(116*transition)+4)
                        +math::easeOutBounce((SDL_GetTicks64()-transitionTime-(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL))/FALLTIME_OTHERS)
                    :(200+i*116-(selection*116)+(116*transition)+4),

                    settings::saveCache.at(i-1).name, false);
                    graphics::fonts->at(2)->render(34+4,

                    SDL_GetTicks64()-transitionTime>(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL)&&transitionTime>0?
                                (200+i*116-(selection*116)+(116*transition)+4+16)
                        +math::easeOutBounce((SDL_GetTicks64()-transitionTime-(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL))/FALLTIME_OTHERS)
                    :(200+i*116-(selection*116)+(116*transition)+4+16),

                    "Lines: " + std::to_string(settings::saveCache.at(i-1).lines), false);
                    graphics::fonts->at(2)->render(34+4,

                    SDL_GetTicks64()-transitionTime>(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL)&&transitionTime>0?
                                (200+i*116-(selection*116)+(116*transition)+4+24)
                        +math::easeOutBounce((SDL_GetTicks64()-transitionTime-(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL))/FALLTIME_OTHERS)
                    :(200+i*116-(selection*116)+(116*transition)+4+24),

                    "Level: " + std::to_string(settings::saveCache.at(i-1).level), false);
                    graphics::fonts->at(2)->render(34+4,

                    SDL_GetTicks64()-transitionTime>(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL)&&transitionTime>0?
                                (200+i*116-(selection*116)+(116*transition)+4+32)
                        +math::easeOutBounce((SDL_GetTicks64()-transitionTime-(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL))/FALLTIME_OTHERS)
                    :(200+i*116-(selection*116)+(116*transition)+4+32),

                    "Score: " + std::to_string(settings::saveCache.at(i-1).score), false);

                    graphics::sprite->render(graphics::shaders.at(4),settings::saveCache.at(i-1).t,{230,
                    
                    SDL_GetTicks64()-transitionTime>(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL)&&transitionTime>0?
                                (200+i*116-(selection*116)+(116*transition)+4)
                        +math::easeOutBounce((SDL_GetTicks64()-transitionTime-(NEWGAMELENGTH+i*FALLTIME_OTHERS_INTERVAL))/FALLTIME_OTHERS)
                    :(200+i*116-(selection*116)+(116*transition)+4),
                    
                    },{40,96},0,{0,0},{40,96});        

                }
                else { //otherwise just show our newgame text
                    graphics::fonts->at(0)->render(transitionTime<=0?34+125:34+(640*(SDL_GetTicks64()-transitionTime)/500.0)+125,200+i*116-(selection*116)+(116*transition)+(50-8),"NEW GAME", true); //the (50-8) means 50 (half the height of the save file) minus 8 (half the height of a letter)
                }

                if((SDL_GetTicks()/500)%2 ==0 && transitionTime <= 0) graphics::fonts->at(4)->render(320,64,"LOAD SAVE",true);

            }
        }break;
        case 1: {

        }break;
    }


};
void classicmenu::reset() {
    networking::globalRPC->update("Getting ready to start a classic game...", "Top high score: " + std::to_string(settings::maxscore), "icon1", std::time(nullptr));
    startTime = SDL_GetTicks64();
    t = Transition();
    settings::clearSaveData();
    settings::loadSaveData();
}
