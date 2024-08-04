#include "classicmenu.h"
#include "SDL2/SDL_mixer_ext.h"
#include "SDL2/SDL_timer.h"
#include "../utils/defs.h"
classicmenu::classicmenu() {
    cd = new plane({0.75,-0.5,-1.5},{1,1,1},{0,0,0});
    redbackground = new bg("classicmenu",false);
    bluebackground = new bg("newgamemenu",false);
    buff = new buffermanager(640,480,false);

    startTime = SDL_GetTicks();
    cube = new wireframecube(320,240,640,480);

}
void classicmenu::input(SDL_Keycode keysym) {
    switch(screenmode) {
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
                    Mix_PlayChannel( -1, audio::sfx->at(0), 0 );

                }break;
                case SDLK_z: {
                    if(selection > 0) { //if we loadin, then we loading
                        settings::saveload = settings::saveDir + "/" + settings::saveCache.at(selection-1).name;
                        t.gamemode = 4;
                        t.transition = 1;
                        t.fade = BARS;
                        std::cout << "Loading Save: "<<settings::saveload << "\n";
                        Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
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
                        Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                    }
                }break;
                case SDLK_UP: {
                    if(selection > 0) {
                        transition -= 1.0f;
                        selection--;
                        Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                    }
                }break;

            }
        }break;
        case 1: { //handle the complex cursor movement here
            switch(subscreen) {
                case 0: {
                    switch(keysym) {
                        case SDLK_x: {
                            //return to the last screen
                        }break;
                        case SDLK_z: {
                            if(rightSide) {
                                switch(selection) {
                                    case 0: {
                                        modifierTab = true;
                                    }break;
                                    // case 1: {

                                    // }break;
                                }
                            }
                            else {
                                if(selection < NUMGAMEMODES) {
                                    gamemodeSelection = selection;
                                }
                                else {

                                    std::cout << "scanning\n";
                                    int currentOption = 0;
                                    int index = 0;
                                    for(index = 0; index < NUMSETTINGS; index++) {
                                        std::cout << selection-4 << " "<< currentOption << " " << gamemodesVisibility[gamemodeSelection] << " " << defaultsettingVisiblity[index] << " " << index << "\n";
                                        if(gamemodesVisibility[gamemodeSelection]&defaultsettingVisiblity[index]) {
                                            if(currentOption == selection-4) {
                                                std::cout << "option found!\n" << currentOption << "\n";
                                                break;
                                            }
                                            currentOption++;
                                        }
                                    }
                                    std::cout << index << "\n";
                                    switch(index) {
                                        case 0: { //open difficulty screen
                                            subscreen = 1;
                                        }break;
                                        default: {
                                            activeToggles=activeToggles^(1<<index);
                                        }break;
                                    }
                                    subscreenAge = SDL_GetTicks64();
                                    Mix_PlayChannel( -1, audio::sfx->at(0), 0 );

                                }
                            }
                            //THIS is gonna be fun
                        }break;
                        case SDLK_DOWN: {
                            if(
                                (selection < NUMGAMEMODES+visiblesettings-1 && !rightSide) ||
                                (selection < RIGHTBUTTONS-1 && rightSide)
                            ) {
                                transition -= 1.0f;
                                selection++;
                                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                            }
                        }break;
                        case SDLK_UP: {
                            if(selection > 0) {
                                transition += 1.0f;
                                selection--;
                                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                            }
                        }break;
                        case SDLK_RIGHT: {
                            if(!rightSide) {
                                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                                rightSide = true;
                                selection = 0;
                            }
                        }break;
                        case SDLK_LEFT: {
                            if(rightSide) {
                                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                                rightSide = false;
                                selection = 0;
                            }
                        }break;

                    }
                }break;
                case 1: { //difficulty picker
                    switch(keysym) {
                        case SDLK_z:
                        case SDLK_x: {
                            subscreen = 0;
                            Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
                            //return to the last screen
                        }break;
                        case SDLK_UP: {
                            if(difficultySelection > 0) {
                                subTransition += 1.0f;
                                difficultySelection--;
                                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                            }
                        }break;
                        case SDLK_DOWN: {
                            if(
                                (difficultySelection < NUMDIFFICULTIES-1)
                            ) {
                                subTransition -= 1.0f;
                                difficultySelection++;
                                Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                            }
                        }break;
                    }
                }break;

            }
        }
    }
}
Transition classicmenu::endLogic() {
    return t;
}
void classicmenu::logic(double deltatime) {
    switch(screenmode) {
        case 0: {
            redbackground->logic(deltatime);
            cube->logic(deltatime);
            if (transition != 0) {
                double lerpval = utils::lerp(transition,0,deltatime*0.005);
                if(std::abs(lerpval) < 0.01) {
                    transition = 0;
                }
                else {
                    transition = lerpval;
                }
            }
            if(SDL_GetTicks64()-transitionTime > TRANSITION_LENGTH-350 && transitionTime > 0 && explosionTime <= 0) {
                explosionTime = SDL_GetTicks64();
                audio::playSound(11);
            }
            if(transitionTime > 0 && SDL_GetTicks64()-transitionTime > TRANSITION_LENGTH) {
                screenmode++;
                currentscreenAge = SDL_GetTicks64();
                transitionTime = 0;
                Mix_FadeOutMusic(500);
                Mix_HookMusicFinished([](){ //very cool lambda function to replace crossfademusicstream
                    Mix_PlayMusic(audio::music->at(6), -1);
                });

            }
        }break;
        case 1: {
            if (transition != 0) {
                double lerpval = utils::lerp(transition,0,deltatime*0.05);
                if(std::abs(lerpval) < 0.01) {
                    transition = 0;
                }
                else {
                    transition = lerpval;
                }
            }
            if (subTransition != 0) {
                double lerpval = utils::lerp(subTransition,0,deltatime*0.05);
                if(std::abs(lerpval) < 0.01) {
                    subTransition = 0;
                }
                else {
                    subTransition = lerpval;
                }
            }

            bluebackground->logic(deltatime);
        }break;

    }

}

void classicmenu::render() {
    buff->enable();

    switch(screenmode) {
        case 0: {
            redbackground->render();
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
            //time to start adapating concept to creation!
            bluebackground->render();
            
            graphics::rect->render(graphics::shaders.at(1),{30,0},{605,480},0,{0,0.1,0.50,0.70},true,4,{0,0,0,1});
            graphics::line->render(graphics::shaders.at(1), {192,0}, {192,480-(8*8)-16-32}, 4, {0,0,0,1});
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("newgamebanner"),
            {0,0},{640,480},0,{(SDL_GetTicks64()/10.0),0},{640,480});
            graphics::fonts->at(1)->render(30+8+8,32+4,"NEW GAME",false);
            graphics::line->render(graphics::shaders.at(1), {30,36+48}, {192,36+48}, 4, {0,0,0,1});

            graphics::fonts->at(0)->render(38,32+4+70,"GAMEMODE",false,255,255,255,-1,true,SDL_GetTicks64()/500.0,4,4);
            int offset = 0;
            for(int i = 0; i < 4; i++) {
                graphics::fonts->at(2)->render(30+8,32+4+70+24+(offset*12),gamemodes[i],false,255,255,gamemodeSelection == i?0:255,-1,false,0,0,0);
                offset++;
            }
            graphics::line->render(graphics::shaders.at(1), {30,32+4+70+20+(offset*12)+20}, {605,32+4+70+20+(offset*12)+20}, 4, {0,0,0,1});
            offset+=3;

            graphics::fonts->at(0)->render(38,32+4+70+24+(offset*12),"SETTINGS",false,255,255,255,-1,true,SDL_GetTicks64()/500.0,4,4);
            offset++; //just doing it like this cuz its easier to read honestly
            offset++;

            visiblesettings = 0;
            for(int i = 0; i < NUMSETTINGS; i++) {
                if(gamemodesVisibility[gamemodeSelection]&defaultsettingVisiblity[i]) {
                    int sideIcon = 0;
                    switch(i) {
                        case 0: { //difficulty
                            sideIcon=2+difficultySelection+1;
                        }break;
                        default: {
                            if(activeToggles>>i&1) {
                                sideIcon = 1;
                            }
                            else {
                                sideIcon = 2;
                            }
                        }break;
                    }
                    if(sideIcon > 0) {
                        graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("optionsicons"),{30+8+(defaultsettings[i].length()*8)+4,32+4+70+24+(offset*12)},{8,8},0,{(sideIcon-1)*8,0},{8,8});
                    }
                    graphics::fonts->at(2)->render(30+8,32+4+70+24+(offset*12),defaultsettings[i],false);
                    offset++;
                    visiblesettings++;
                }
            }
            //OTHER SIDE START!

            graphics::fonts->at(0)->render(196,32+8,"MODIFIERS",false,255,255,255,-1,true,SDL_GetTicks64()/500.0,4,4);
            
            graphics::rect->render(graphics::shaders.at(1),{192+8-3,32+8+32},{600-3,32+8+32+96},0,{0,0.1,0.50,0.70},true,2,{
                (selection==0&&rightSide)?255:0,
                (selection==0&&rightSide)?255:0,
                0,
                1
                
                });
            for(int i = 0; i < 16; i++) {
                graphics::sprite->render(graphics::shaders.at(4),gameplay::modifiers.at(0).metadata.tex,{192+8-3+4+(i%8)*48,64+8+(i/8)*48},{48,48},0,
                {48*(activeMods>>i&1),0},
                {48,48});
                //THE EQUATION TO SOLVE THE NEW LOCATION:
                //n = some binary number, i = slot to check
                //n>>i&1
                if(newModifiers>>i&1) {
                    graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("new"),{192+8-3+4+(i%8)*48,64+8+(i/8)*48},{16,16},0,{0,0},{16,16});
                }


            }

            graphics::fonts->at(2)->render(192+12-3, 32+8+32+96-8,std::to_string(math::numActive(activeMods))+"/6",false);
            graphics::line->render(graphics::shaders.at(1), {30,480-(8*8)-16-32}, {605,480-(8*8)-16-32}, 4, {0,0,0,1});
            graphics::fonts->at(2)->render(38,480-(8*8)-16-24,gameplay::modifiers[0].metadata.desc,false,255,255,255,559,false,0,0,0);
            //38 to 597 = 559/8 = 69
            graphics::line->render(graphics::shaders.at(1), {192+206,32+4+70+20+(4*12)+20}, {192+206,480-(8*8)-16-32}, 4, {0,0,0,1});
            graphics::fonts->at(0)->render(196,32+4+70+20+(4*12)+28,"SCORE MULT",false);
            graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+16,"Overall Score Mult: 1x",false);
            graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+24,"Line Score Mult: 1x",false);
            graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+32,"Combo Score Mult: 1x",false);
            graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+40,"Gravity Score Mult: 1x",false);
            graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+48,"Drop Score Mult: 1x",false);
            graphics::fonts->at(0)->render(196,32+4+70+20+(4*12)+28+56,"MODIFIERS",false);

            graphics::fonts->at(0)->render(192+206+101,32+4+70+20+(4*12)+28,"TEXTURE PACKS",true);

            offset = 0; //we on the left side, so we dont need offset's value anymore
            for(int i = 0; i < gameplay::modifiers.size();i++) {
                if(activeMods>>i&1) {
                    for(modifierTag meta : gameplay::modifiers.at(i).metadata.tags) {
                        switch(meta.quality) {
                            case GOOD: { //its green. amazing.
                                graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+72+(offset*8),meta.tag,false,0,0,255,-1,false,0,0,0);
                            }break;
                            case BAD: { //get this: this one is... RED?! how unpredictable.
                                graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+72+(offset*8),meta.tag,false,255,0,0,-1,false,0,0,0);
                            }break;
                            case UGLY: { //this one is white because hotel sheets legally cannot be any color other than white to detect shit stains.
                                graphics::fonts->at(2)->render(196,32+4+70+20+(4*12)+28+72+(offset*8),meta.tag,false,255,255,255,-1,false,0,0,0);
                            }break;
                        }
                        offset++;
                    }
                }
            }
            if(!rightSide) {
                graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("cursor"),
                {38,32+4+70+24+selection*12+12*(transition)+(selection>=NUMGAMEMODES?60:0)},
                {8,8},0,{((SDL_GetTicks64()/100)%4)*8,0},{8,8});
            }
            graphics::rect->render(
                graphics::shaders.at(1),
                 {0,0}, {640,480},
                 0, 
                 {0,0,0,
                 SDL_GetTicks64()-currentscreenAge<250?1-(SDL_GetTicks64()-currentscreenAge)/250.0:0},
                  false, 0, {0,0,0,1});
                

        }break;
    }

    switch(subscreen) {
        case 1: {
            double transitionIn = (SDL_GetTicks64()-subscreenAge)/250.0;
            if(transitionIn < 1) {
                graphics::rect->render(graphics::shaders.at(1), {0,0}, {640,480}, 0, {0,0,0,transitionIn*0.8}, false, 0, {0,0,0,1});
            }
            else {
                transitionIn = 1;
                graphics::rect->render(graphics::shaders.at(1), {0,0}, {640,480}, 0, {0,0,0,0.8}, false, 0, {0,0,0,1});
            }
            int offset = 640*(1-transitionIn);

            graphics::rect->render(graphics::shaders.at(1),{64+offset,48},{576+offset,256},0,{0,0,0,0.8},true,2,{1,1,1,1});
            graphics::fonts->at(0)->render(64+256+offset,64,"CHOOSE A DIFFICULTY",true);
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("difficultytextures"),{248+offset,80},{224,105},0,{0+difficultySelection*224,0},{224,105});
            for(int i = 0; i < 4; i++) {
                graphics::fonts->at(0)->render(64+32+offset,64+32*(i+1),difficulties[i],false,255,difficultySelection==i?0:255,255,-1,false,0,0,0);
            }
            graphics::fonts->at(2)->render(64+offset+192,192,difficultyDesc[difficultySelection],false,255,255,255,256,false,0,0,0);
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("cursor"),
            {64+16+offset,64+32*(difficultySelection+1)+(subTransition)},
            {16,16},0,{((SDL_GetTicks64()/100)%4)*8,0},{8,8});


        }
    }

    buff->disable(640,480,true);

    if(SDL_GetTicks64()-startTime < 1000 && screenmode == 0) {
        float percent = (SDL_GetTicks64()-startTime)/1000.0;
        graphics::sprite->render(graphics::shaders.at(4),
    &buff->renderTexture, {320-320*percent,240-240*percent},
    {640*percent,480*percent},
    {0,0,1440*percent},{0,0},{640,480});
    }
    else {
        graphics::sprite->render(graphics::shaders.at(4),
    &buff->renderTexture, {0,0},{640,480},{0,0,0},{0,0},{640,480});
    }
    if(explosionTime > 0) {
        int frame = (((SDL_GetTicks64()-explosionTime)/100));
        if(frame >= 15) { //15, instead of 16 because first frame is 0 duh
            explosionTime = 0;
        }
        else {
            std::cout << frame << "\n";
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("stupidexplosion"),
            {320-256,240-256},{512,512},0,{
                (frame%4)*64,
                (frame/4)*64
                },{64,64});
        }
    }

};
void classicmenu::reset() {
    Mix_PlayMusic(audio::music->at(3),-1);

    networking::globalRPC->update("Getting ready to start a classic game...", "Top high score: " + std::to_string(settings::maxscore), "icon1", std::time(nullptr));
    startTime = SDL_GetTicks64();
    screenmode = 0;
    t = Transition();
    settings::clearSaveData();
    settings::loadSaveData();
}
