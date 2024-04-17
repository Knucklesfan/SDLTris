#include "titlescreen.h"
#include <iostream>
#include <SDL2/SDL_mixer_ext.h>
#include <vector>
#include <math.h>
#include "../utils/defs.h"
#ifdef __SWITCH__
#define prefix  "/"
#include <switch.h>

#else
#define prefix  "./"
#endif
double easeOutBounce(double x) {
double n1 = 7.5625;
double d1 = 2.75;

if (x < 1 / d1) {
    return n1 * x * x;
} else if (x < 2 / d1) {
    return n1 * (x -= 1.5 / d1) * x + 0.75;
} else if (x < 2.5 / d1) {
    return n1 * (x -= 2.25 / d1) * x + 0.9375;
} else {
    return n1 * (x -= 2.625 / d1) * x + 0.984375;
}

}
titlescreen::titlescreen()
{
    bgnum = std::rand() % graphics::backgrounds->size();
    buff = new buffermanager(640,480,false);
    p = new plane({0,0,-2.5},{4.0/3.0,1,1},{0,0,0});
    gg = new GlobalGamemode();
    frontGame=  new game();
    
    
}
void titlescreen::reset()
{
    Mix_PlayMusic( audio::music->at(0), -1 );
    time = 0;
    active = true;
    currentscreen = 0;
    bgnum = std::rand() % graphics::backgrounds->size();

    //audio::playMusic(0); //plays the first song in the list
}

void titlescreen::input(SDL_Keycode key)
{
    switch(key) {
        case(SDLK_z): {
            if(time < 20000) {
                time = 20000;
            }
            else {
                active = false;
            }
            break;
        }
        case(SDLK_x): {
            time = 40000;
            break;
        }
    }
    // switch (currentscreen) {

    // case 0: {

    //     switch (key) {
    //     case(SDLK_UP): {
    //         if (currentselection > 0) {
    //             currentselection = (currentselection - 1);
    //             Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
    //         }
    //         break;
    //     }
    //     case(SDLK_DOWN): {
    //         if (currentselection < selections - 1) {
    //             currentselection = (currentselection + 1);
    //             Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
    //         }
    //         break;
    //     }
    //     case(SDLK_z): {
    //         Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
    //         switch (currentselection) {
    //         case 0:
    //             loadgame = true;
    //             break;
    //         case 1:
    //             loadmenu = true;
    //             break;
    //         case 2:
    //             loadcreds = true;
    //             break;
    //         case 3:
    //             exit(0);
    //             break;
    //         }
    //         break;
    //     }

    //     }
    //     break;
    // }

    // case 1: {
    //     switch (key) {
    //     case(SDLK_z): {
    //         Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
    //         currentscreen = 0;
    //     }
    //                 break;
    //     }
    //     break;
    // }
    // case 2: {
    //     switch (key) {
    //     case(SDLK_UP): {
    //         if (currentselection > 0) {
    //             currentselection = (currentselection - 1);
    //         }
    //         Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
    //         break;
    //     }
    //     case(SDLK_DOWN): {
    //         if (currentselection < settingssize - 1) {
    //             currentselection = (currentselection + 1);
    //         }
    //         Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
    //         break;
    //     }
    //     case(SDLK_LEFT): {
    //         switch(currentsetting) {
    //             case 0:
    //             break;
    //             case 1: {
    //                 int currentvol = Mix_VolumeMusic(-1);
    //                 if(currentvol > 0) {
    //                     Mix_VolumeMusic(currentvol-1);
    //                 }
    //                 break;
    //             }
    //             case 2: {
    //                 int currentvol = Mix_Volume(-1,-1);
    //                 if(currentvol > 0) {
    //                     Mix_Volume(-1, currentvol-1);
    //                 }
    //                 break;

    //             }
    //         }
    //         break;
    //     }
    //     case(SDLK_RIGHT): {
    //         switch(currentsetting) {
    //             case 0:
    //             break;
    //             case 1: {
    //                 int currentvol = Mix_VolumeMusic(-1);
    //                 if(currentvol < MIX_MAX_VOLUME) {
    //                     Mix_VolumeMusic(currentvol+1);
    //                 }
    //                 break;
    //             }
    //             case 2: {
    //                 int currentvol = Mix_Volume(-1,-1);
    //                 if(currentvol < MIX_MAX_VOLUME) {
    //                     Mix_Volume(-1, currentvol+1);
    //                 }
    //                 break;

    //             }
    //         }
    //         break;
    //     }

    //     case(SDLK_z): {
    //         Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
    //         if (showerror || currentsetting > 0) {
    //             currentsetting = 0;                    
    //             showerror = false;
                
    //         }
    //         else {
    //             switch (currentselection) {
    //             case 0: {
    //                 Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    //                 bool IsFullscreen = SDL_GetWindowFlags(graphics::window) & FullscreenFlag;
    //                 SDL_SetWindowFullscreen(graphics::window, IsFullscreen ? 0 : FullscreenFlag);
    //                 SDL_ShowCursor(IsFullscreen);
    //                 break;
    //             }
    //             case 1:
    //                 currentsetting = 1;                    
    //                 break;
    //             case 2:
    //                 currentsetting = 2;                    
    //                 break;
    //             case 3:
    //                 currentscreen = 0;
    //                 currentselection = 2;
    //                 break;
    //             }
    //             break;
    //         }
    //     }

    //     }
    //     break;
    // }

    // case 3: {
    //     switch (key) {
    //     case(SDLK_z): {
    //         Mix_PlayChannel( -1, audio::sfx->at(0), 0 );
    //         currentscreen = 0;
    //             }
    //         break;
    //         }
    //     break;
    //     }
    // }
    debuginput(key);
}

void titlescreen::render()
{
    gg->startRender();

    graphics::backgrounds->at(bgnum).render();

    buff->enable();

    if(currentscreen == 0 ){
            if(time > 700) {
                graphics::fonts->at(3)->render("MEET THE GAME: ",16,480-48,false,255,255,255,0,false,0,0,0,1.0);
            }
            if(time > 2000) {
                graphics::fonts->at(3)->render("- A fun, block dropping experience",16,480-48-48,false,255,255,255,0,false,0,0,0,0.5);
            }
            if(time > 4000) {
                graphics::fonts->at(3)->render("- Probably one of the best Tetrimino based games Knuxfan's ever made",16,480-48-48-48,false,255,255,255,0,false,0,0,0,0.5);
            }
            if(time > 6000) {
                graphics::fonts->at(3)->render("- Better than getting hit with a truck?",16,480-48-48-48-48,false,255,255,255,0,false,0,0,0,0.5);
            }
            if(time > 8000) {
                graphics::fonts->at(3)->render("- Has graphics, has sound",16,480-48-48-48-48-48,false,255,255,255,0,false,0,0,0,0.5);
            }
            if(time > 10000) {
                graphics::fonts->at(3)->render("- looks like it was made by an escaped inmate",16,480-48-48-48-48-48-48,false,255,255,255,0,false,0,0,0,0.5);
            }
            if(time > 12000) {
                graphics::fonts->at(3)->render("- Who really pulled the trigger on JFK?",16,480-48-48-48-48-48-48-48,false,255,255,255,0,false,0,0,0,0.5);
            }
            if(time > 14000) {
                graphics::fonts->at(3)->render("THAT HAS CRITICS GOING CRAZY",16,480-48-48-48-48-48-48-48-48-48,false,255,255,255,0,false,0,0,0,1.0);
            }
            // if(time > 400000) {
            //     currentscreen++;
            // }
    }
    else {
        frontGame->render();
    }
    buff->disable(640,480,true);

    if(time > 18250.0f && currentscreen == 0) {
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
        glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
        view = glm::translate(view, glm::vec3(0.0, 0, 0.0)); 
        p->render(graphics::shaders[0],&buff->renderTexture,projection,view);
    }
    else {
        graphics::sprite->render(graphics::shaders.at(4),
    &buff->renderTexture, {0,0},{640,480},{0,0,0},{0,0},{640,480});
    }
    gg->render();

    if(time > 15000 && time < 19150) {
        graphics::sprite->render(graphics::shaders.at(4),
        graphics::sprites["tetriminos"],
        glm::vec2(320-graphics::sprites["tetriminos"]->w/2, 
        time<17000.0f?(-200+
            easeOutBounce((time-15000.0f)/(17000.0f-15000.0f))
            *((440)-graphics::sprites["tetriminos"]->h/2)):240-graphics::sprites["tetriminos"]->h/2
        ),
         glm::vec2(graphics::sprites["tetriminos"]->w,graphics::sprites["tetriminos"]->h)
         , 0, glm::vec2(0,0), glm::vec2(graphics::sprites["tetriminos"]->w,graphics::sprites["tetriminos"]->h));
    }
    if(time > 18000 && time < 19150) {
        graphics::sprite->render(graphics::shaders.at(4),
        graphics::sprites["knuxfans"],
        glm::vec2(
        100,
        172
        ),
        glm::vec2(
            time<18250.0f?(graphics::sprites["knuxfans"]->w*utils::lerp(10,1,(time-18000.0f)/(18250.0f-18000.0f))):graphics::sprites["knuxfans"]->w,
            time<18250.0f?(graphics::sprites["knuxfans"]->h*utils::lerp(10,1,(time-18000.0f)/(18250.0f-18000.0f))):graphics::sprites["knuxfans"]->h
        ),
        time<18250.0f?(time-18000)/2-130:(18250.0f-18000)/2-130,
        glm::vec2(0,0), glm::vec2(graphics::sprites["knuxfans"]->w,graphics::sprites["knuxfans"]->h));
    }
    if(time > 19000 && time < 19150) {
        graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{1,1,1,1},false,1,glm::vec4(1,1,1,1));
    }
    if(time > 40000) {
        float knfnscale = (640-((time)-40000)>320?640-((time)-40000):320)/320;
        graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["checkerboard"],{
            0,
            148+easeOutBounce((time-40000.0f)/(40000.0f-42000.0f))
            *((480))
        },
        {640,345-148},0,{-time/10.0,time/10.0},{640,345-148});
        graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["knfnbanner"],
        {
            0,
            easeOutBounce((time-40000.0f)/(40000.0f-42000.0f))
            *((480))
        },

        {640,480},0,{time/10.0,0},{640,480});

        // graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["checkerboard"],{time-40000,148},{640,345-148},0,{-time/10.0,time/10.0},{640,345-148});

    }
    if(time > 19150) {
        if(time < 40000) {
        graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["checkerboard"],{0,148},{640,345-148},0,{-time/10.0,time/10.0},{640,345-148});
        graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["knfnbanner"],{0,0},{640,480},0,{time/10.0,0},{640,480});
        }
        graphics::sprite->render(graphics::shaders.at(4), graphics::sprites["knfnlogo"],
    {0,0},{640,480},{0,0,0},{0,0},{640,480});   

        if(fmod(time,1000)<500) {
            graphics::fonts->at(0)->render(320,320,"PRESS START:ENTER",true);
        }

    }

    
// #ifdef __LEGACY_RENDER
//     SDL_RenderClear(graphics::render);
// #else
// #endif
//     graphics::backgrounds->at(bgnum).render();

// #ifdef __LEGACY_RENDER
//     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//     SDL_Rect bx = { 160, 250, 320, 210 };
//     SDL_RenderFillRect(graphics::render, &bx);
//     SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
//     SDL_RenderDrawRect(graphics::render, &bx);
//     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//     graphics::drawTexture(graphics::sprites.at("knfnlogo"), 0, 0, 0.0, 1.0, false);
// #else
//     graphics::rect->render(graphics::shaders.at(1),{160,250},{160+320,250+210},0,{0,0,0,1},true,1,glm::vec4(1,1,1,1));
//     graphics::sprite->render(graphics::shaders[4],graphics::sprites.at("knfnlogo"),glm::vec2(0,0),glm::vec2(640,480),0,glm::vec2(0,0),glm::vec2(640,480));
// #endif

//     for (int i = 0; i < selections; i++) {
//         graphics::fonts->at(1)->render(320, 300 + (i * 32),settings[i],  true, 255, 0, ((i == currentselection && currentscreen == 0)?0:255),-1,false,0,0,0);
//     }

//     graphics::fonts->at(2)->render(490, 165, "Return of the Block!", true,255,255,255,-1,true,time/100,1,5);
//     // graphics::fonts->at(0)->render("VERS 1.0 BABY!", 490, 165, true, 0, 0, 0, 0, true, time/100, 1, 5,1);
//     versfont->render(320, 280, "High Score: " + std::to_string(settings::maxscore), true);
//     versfont->render(320, 265, "Previous Score: " + std::to_string(settings::previousscore), true);
//     switch (currentscreen) {
//         case(1): {
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//             // SDL_Rect splashbox = { 0, 0, 640, 480 };
//             // SDL_RenderFillRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
//             // splashbox = { 320 - 240, 240 - 120, 480, 240 };
//             // SDL_RenderFillRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
//             // SDL_RenderDrawRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

//             // headerfont->render(320, 140, messagetitle, true, renderer);
//             // bodyfont->render(320, 180, messagebody, true, renderer,448);
//             // buttonfont->render(320, 300, messagebutton, true, renderer);
//             break;
//         }
//         case(2): {
//             //None, and i mean none of this is used anymore! it was basically only in there for legacy sake anyways
//             //that, and nostalgia value
//             //so if you're nostalgic for the old SDLTris that nobody played, you can reimplement it yourself
//             //have fun.
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//             // SDL_Rect splashbox = { 0, 0, 640, 480 };
//             // SDL_RenderFillRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
//             // splashbox = { 320 - 240, 80, 480, 320 };
//             // SDL_RenderFillRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
//             // SDL_RenderDrawRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

//             // // headerfont->render(320, 100, settingstitle, true, renderer);
//             // for (int i = 0; i < settingssize; i++) {
//             //     // buttonfont->render(renderer, sets[i], 320, 130 + (i * 16),  true, 255, ((i == currentselection)?0:255), 255);
//             // }
//             // if (showerror) {
//             //     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//             //     SDL_Rect splashbox = { 0, 0, 640, 480 };
//             //     SDL_RenderFillRect(graphics::render, &splashbox);
//             //     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
//             //     splashbox = { 320 - 240, 240 - 120, 480, 240 };
//             //     SDL_RenderFillRect(graphics::render, &splashbox);
//             //     SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
//             //     SDL_RenderDrawRect(graphics::render, &splashbox);
//             //     SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);

//             //     // headerfont->render(320, 140, messagetitle, true, renderer);
//             //     // bodyfont->render(320, 180, messagebody, true, renderer);
//             //     // buttonfont->render(320, 300, messagebutton, true, renderer);
//             // }
//             // else switch(currentsetting) {
//             //     case 0:
//             //     break;
//             //     case 1: { //these didnt work anyways, might as well not keep them around

//             //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//             //         // SDL_Rect splashbox = { 0, 0, 640, 480 };
//             //         // SDL_RenderFillRect(graphics::render, &splashbox);
//             //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
//             //         // splashbox = { 320 - 240, 240 - 120, 480, 240 };
//             //         // SDL_RenderFillRect(graphics::render, &splashbox);
//             //         // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
//             //         // SDL_RenderDrawRect(graphics::render, &splashbox);
//             //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
//             //         // double var = ((double) Mix_VolumeMusic(-1) / 128) * 58.0 + std::sin(time / 50) * 10;
//             //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 80,  360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
//             //         // double coz = ((double)Mix_VolumeMusic(-1) / 128) * 58.0 + std::cos(time / 50) * 10;
//             //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);
//             //         // headerfont->render(320, 140, "MUSIC VOLUME", true, renderer);
//             //         // bodyfont->render(renderer, std::to_string(Mix_VolumeMusic(-1)), 320, 180, true, 255, 0, 255);
//             //         // buttonfont->render(320, 300, messagebutton, true, renderer);
//             //         break;
//             //     }
//             //     case 2: {
//             //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//             //         // SDL_Rect splashbox = { 0, 0, 640, 480 };
//             //         // SDL_RenderFillRect(graphics::render, &splashbox);
//             //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
//             //         // splashbox = { 320 - 240, 240 - 120, 480, 240 };
//             //         // SDL_RenderFillRect(graphics::render, &splashbox);
//             //         // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
//             //         // SDL_RenderDrawRect(graphics::render, &splashbox);
//             //         // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
//             //         // double var = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + std::sin(time / 50) * 10;
//             //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 80, 360 - (int)var, 0.0, 1.0, false, 0, 0, 240, var);
//             //         // double coz = ((double)Mix_Volume(-1, -1) / 128) * 58.0 + std::cos(time / 50) * 10;
//             //         // graphics::drawTexture(graphics::sprites.at("visualizer"), 320, 360 - (int)coz, 0.0, 1.0, false, 0, 0, 240, coz);

//             //         // headerfont->render(320, 140, "SOUND VOLUME", true, renderer);
//             //         // bodyfont->render(renderer, std::to_string(Mix_Volume(-1, -1)), 320, 180, true, 255, 0, 255);
//             //         // buttonfont->render(320, 300, messagebutton, true, renderer);


//             //         break;
//             //     }

//             // }
//             break;
//         }
//         case(3): {
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 128);
//             // SDL_Rect splashbox = { 0, 0, 640, 480 };
//             // SDL_RenderFillRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 200);
//             // splashbox = { 320 - 240, 80, 480, 320 };
//             // SDL_RenderFillRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 255, 255, 255, 255);
//             // SDL_RenderDrawRect(graphics::render, &splashbox);
//             // SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 26);
//             // headerfont->render(320, 100, creditstitle, true);
//             // for (int i = 0; i < creditssize; i++) {
//             //     if (big[i] > 0) {
//             //         headerfont->render(credits[i], 320, 130 + (i * 32), true, 255, ((i == currentselection) ? 0 : 255), 255,-1,false,0,0,0,1);
//             //     }
//             //     else {
//             //         bodyfont->render(credits[i], 320, 130 + (i * 32), true, 255, ((i == currentselection) ? 0 : 255), 255,448,false,0,0,0,1);
//             //     }
//             // }
//             // buttonfont->render(320, 300, messagebutton, true);
//         }

//     }
    //SDL_RenderPresent(renderer);

}

void titlescreen::logic(double deltatime)
{
    graphics::backgrounds->at(bgnum).logic(deltatime);
    if(gg->logic(deltatime)) {
        currentscreen=gg->currentTransition.gamemode;
        if(currentscreen > 0) {
            frontGame->setupDemo(settings::demos.at(currentscreen-1));
        }
        else {
            time = 18000;
        }
    };
    if(currentscreen != 0) {
        frontGame->logic(deltatime);
        if(frontGame->demoEndLogic()) {
            Transition t;
            t.transition = true;
            t.fade = FADETYPE::BLOCKS;
            t.gamemode = (currentscreen+1)%(settings::demos.size()+1);
            gg->setFade({
                t
            });
        }
    }
    // background[bgnum].logic(deltatime);
    if(time > 18250.0f && p->rotation.x > -450.0f) {
        p->rotation = {p->rotation.x-=deltatime*0.04,p->rotation.y,p->rotation.z-=deltatime*0.05};
        p->position = {p->position.x,p->position.y-=deltatime*0.0005,p->position.z-=deltatime*0.01};
    }
    if(time > 40000.0f && !gg->active && currentscreen == 0) {
        Transition t;
        t.transition = true;
        t.gamemode = 1;
        t.speed = 0.001;
        t.fade = FADETYPE::FADE;
        gg->setFade({
            t
        });
    }
    time += deltatime;
}

Transition titlescreen::endLogic()
{
    Transition t = Transition();
    t.fade = FADETYPE::BLOCKS;
    t.gamemode = gameplay::gamemode+1;
    t.transition = !active;
    if(currentscreen != 0) {
        frontGame->endLogic();
    }
    return t;
}


void titlescreen::debuginput(SDL_Keycode key) {
    switch(key) {
        case SDLK_F1: {
            time = 16000;
            break;
        }

        case SDLK_F2: {
            rotationx--;
            break;
        }

    }
}
