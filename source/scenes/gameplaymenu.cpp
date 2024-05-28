#include "gameplaymenu.h"
float easeOutQuint(float x) {
    return x * x * x * x * x;
}
float easeInOutCubic(float x) {
    return -std::pow((2*x)-1, 2)+1;
}
gameplaymenu::gameplaymenu() {
    cd = new plane({0.75,-0.5,-1.5},{1,1,1},{0,0,0});
    lamp = new model("models/lamp.kmf",{0,6,-16.5},{1,1,1},{0,0,0});
    ball = new model("models/amigaball.kmf",{0,6,-16.5},{0.5,0.5,0.5},{0,0,0});

    cdPos = {0.75,-0.5,-80.5};
    startTime = SDL_GetTicks64();
}
void gameplaymenu::input(SDL_Keycode keysym) {
    Uint32 time = SDL_GetTicks64()-startTime;
    if(time < 5000) {
        if(keysym == SDLK_z) {
            startTime-=5000;

        }
    }
    else {
            switch(keysym) {
                case SDLK_LEFT: {
                    if(currentscreen != 0) {
                        if(subselection > 0) {
                            subselection--;
                            Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                        }
                    }
                    else
                    if(selection > 0) {
                        selection--;
                        Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                    }
                }break;
                case SDLK_RIGHT: {
                    if(currentscreen != 0) {
                        if(subselection < subbuttons) {
                            subselection++;
                            Mix_PlayChannel( -1, audio::sfx->at(1), 0 );
                        }
                    }
                    else
                    if(selection < NUMBUTTONS) {
                        selection++;
                        Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                    }
                }break;
                case SDLK_UP: {
                    if(selection >= ROWWIDTH && currentscreen <= 0) {
                        selection=(selection/2)-1;
                        Mix_PlayChannel( -1, audio::sfx->at(1), 0 );


                    }
                }break;
                case SDLK_DOWN: {
                    if(selection < ROWWIDTH && currentscreen <= 0) {
                        selection=(selection+1)*2;
                        Mix_PlayChannel( -1, audio::sfx->at(1), 0 );

                    }
                }break;
                case SDLK_z: {
                    switch(currentscreen) {
                        case 0: {
                        switch(selection) {
                            case 0: {
                                if(!doDownReturnTransition && !doDownTransition) {
                                    doDownTransition = true;
                                    Mix_FadeOutMusic(500);
                                    Mix_HookMusicFinished([](){ //very cool lambda function to replace crossfademusicstream
                                        Mix_FadeInMusic(audio::music->at(2),-1,500); //dear C++, please explain: how does this work?
                                        Mix_HookMusicFinished(NULL); //THIS IS INSANITY!
                                    });
                                    // Mix_CrossFadeMusicStream(audio::music->at(1),audio::music->at(2),-1,1000,0);
                                }
                            }break;
                            case 1: {
                                t.gamemode = 3;
                                t.transition = true;
                                Mix_HaltMusic();
                                Mix_PlayMusic(audio::music->at(3),-1);
                            }break;
                            case 2: {

                            }break;
                            case 3: {

                            }break;
                            case 4: {
                                t.gamemode = 6;
                                t.transition = true;
                                Mix_FadeOutMusic(1000);
                            }break;
                            case 5: {
                                currentscreen = 1;
                                subbuttons = 1;

                                currentscreenAge = SDL_GetTicks64();
                            }break;
                        }
                        }break;
                        case 1: {
                            if(subselection) {
                                exit(0);
                            }
                            else {
                                currentscreen = 0;
                                subselection = 0;
                            }
                        }break;
                    }

                }break;
                case SDLK_x: {
                    switch(currentscreen) {
                        case 1: {
                            currentscreen = 0;
                            subselection = 0;
                        }break;
                        case 4: {
                            if(!doDownReturnTransition && !doDownTransition) {
                                doDownReturnTransition = 1;
                            }
                            
                        }break;
                    }
                }break;
            }
    }
}
Transition gameplaymenu::endLogic() {
    return t;
}
void gameplaymenu::logic(double deltatime) {
    Uint32 time = SDL_GetTicks64()-startTime;
    if(time < 5000) {
        cdPos = glm::vec3(0.75-11.5*easeInOutCubic(time/5000.0), -0.5, utils::lerp(-80.5,-1.5,time/5000.0));
    }
    else {
        cdPos = {0.75,-0.5+(transition*2),-1.5};
        if(buttonx < 312) {
            buttonx = utils::lerp(buttonx,340,deltatime*0.005);
        }
        else {
            buttonx = 312;
        }
    }
    if(doDownTransition) {
        if(transition > 1 && currentscreen != 4) {
            currentscreen =  4;
            currentscreenAge = SDL_GetTicks64();
            subbuttons = 2;
            subselection = 1;
        }
        else if(transition < 1 && currentscreen != 4){
            transition += deltatime/2500.0f;
        }
        else if(transition > 0 && currentscreen == 4){
            transition-= deltatime/2500.0f;
        }
        else {
            transition = 0;
            doDownTransition = false;
        }
    }
    if(doDownReturnTransition) {
        if(transition > 1 && currentscreen != 0) {
            currentscreen =  0;
            currentscreenAge = SDL_GetTicks64();
            Mix_FadeOutMusic(500);
            Mix_HookMusicFinished([](){ //very cool lambda function to replace crossfademusicstream
                Mix_FadeInMusic(audio::music->at(1),-1,500); //dear C++, please explain: how does this work?
                Mix_HookMusicFinished(NULL); //THIS IS INSANITY!
            });

        }
        else if(transition < 1 && currentscreen != 0){
            transition += deltatime/2500.0f;
        }
        else if(transition > 0 && currentscreen == 0){
            transition-= deltatime/2500.0f;
        }
        else {
            transition = 0;
            doDownReturnTransition = false;
        }
    }
}
void gameplaymenu::render() {
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
    glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
    // view = glm::translate(view, glm::vec3(0.0, 0, 0.0)); 

    if(currentscreen < 4) {
        Uint32 time = SDL_GetTicks64()-startTime;
        if(time > 5000) {
            glm::vec4 backgroundShade = {1,1,1,1-transition};
            graphics::shaders.at(5)->activate();
            graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(backgroundShade));
            graphics::sprite->render(graphics::shaders.at(5),graphics::sprites.at("menubackground"),{0,0},{640,480},0,{SDL_GetTicks64()/10.0,SDL_GetTicks64()/10.0},{640,480});
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("ring"),{-164,-164-transition*1000},{328,328},-(SDL_GetTicks64()/100.0),{0,0},{328,328});
        }
        cd->position = cdPos;
        cd->rotation = glm::vec3(-70,0,(SDL_GetTicks64()/10)%360);
        cd->scale = {1,1,1};
        graphics::sprites.at("beachgridwords")->activate(1);
        graphics::sprites.at("cdrom-reflectionmap")->activate(2);
        cd->render(graphics::shaders.at(7),graphics::sprites.at("cdrom"),projection,view);
        if(time > 5000) {
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("storymenubutton"),{-300+buttonx+(selection==0?((sin(SDL_GetTicks64()/100.0f+M_PIf64)))*8:0),selection==0?116+sin(SDL_GetTicks64()/500.0f)*8-transition*1000:116-transition*1000},{300+(selection==0?((sin(SDL_GetTicks64()/100.0f)))*16:0),128},{0,0,0},{selection==0?300:0,0},{300,128});
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("classicmenubutton"),{640-buttonx+(selection==1?((sin(SDL_GetTicks64()/100.0f+M_PIf64)))*8:0),selection==1?116+sin(SDL_GetTicks64()/500.0f)*8-transition*1000:116-transition*1000},{300+(selection==1?((sin(SDL_GetTicks64()/100.0f)))*16:0),128},0,{selection==1?300:0,0},{300,128});
            
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("optionsmenubutton"),{52+(selection==2?((sin(SDL_GetTicks64()/100.0f+M_PIf64)))*8:0),selection==2?260+sin(SDL_GetTicks64()/500.0f)*8-transition*1000:260-transition*1000},{128+(selection==2?((sin(SDL_GetTicks64()/100.0f)))*16:0),64},0,{selection==2?128:0,0},{128,64});
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("highscoremenubutton"),{52+128+8+(selection==3?((sin(SDL_GetTicks64()/100.0f+M_PIf64)))*8:0),selection==3?260+sin(SDL_GetTicks64()/500.0f)*8-transition*1000:260-transition*1000},{128+(selection==3?((sin(SDL_GetTicks64()/100.0f)))*16:0),64},0,{selection==3?128:0,0},{128,64});
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("creditsmenubutton"),{52+128+8+128+8+(selection==4?((sin(SDL_GetTicks64()/100.0f+M_PIf64)))*8:0),selection==4?260+sin(SDL_GetTicks64()/500.0f)*8-transition*1000:260-transition*1000},{128+(selection==4?((sin(SDL_GetTicks64()/100.0f)))*16:0),64},0,{selection==4?128:0,0},{128,64});
            graphics::sprite->render(graphics::shaders.at(4),graphics::sprites.at("exitmenubutton"),{52+128+8+128+8+128+8+(selection==5?((sin(SDL_GetTicks64()/100.0f+M_PIf64)))*8:0),selection==5?260+sin(SDL_GetTicks64()/500.0f)*8-transition*1000:260-transition*1000},{128+(selection==5?((sin(SDL_GetTicks64()/100.0f)))*16:0),64},0,{selection==5?128:0,0},{128,64});
        }
    }
    switch(currentscreen) {
        case 1: { //show the "do you wish to exit?" screen
            graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{0,0,0,(SDL_GetTicks64()-currentscreenAge)/1000.0f>=0.5?0.5f:(SDL_GetTicks64()-currentscreenAge)/1000.0f},false,0,{0,0,0,0});
            // float movement = (SDL_GetTicks64()-currentscreenAge)/250.0f>=1.0f?1.0f:(SDL_GetTicks64()-currentscreenAge)/250.0f;

            graphics::rect->render(graphics::shaders.at(1),{80,180},{80+480,(180+120)},0,{0,0,0,200},true,1,{255,255,255,255});

            graphics::fonts->at(1)->render(320, 180+16, "Do you really want to exit?", true);
            graphics::fonts->at(0)->render((448+0), 180+120-32, "Yes", true,255,!subselection?255:0,255,-1,false,0,0,0);
            graphics::fonts->at(0)->render((192+0), 180+120-32, "No", true,255,subselection?255:0,255,-1,false,0,0,0);


        }break;
        case 4: {
            view = glm::translate(view, glm::vec3(0.0, 0-(easeOutQuint(transition)*60), 0.0)); 

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS); 

            cd->position = {0,-1,-6.5};
            cd->rotation = {-90,0,0};
            cd->scale = {6,6,1};
            float y = sin(SDL_GetTicks64()/500.0f);
            float z = cos(SDL_GetTicks64()/250.0f);
            lamp->rotation = {y*0.0f,-y*10.0f,z*45.0f};
            lamp->render(graphics::shaders.at(0),projection,view);
            glm::vec3 lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);

            graphics::shaders.at(10)->activate();
            glm::vec3 lightpos = glm::vec3(z*2, 5.75, -7.5+(-y)*2);
            graphics::shaders.at(10)->setVec3("lightColor", glm::value_ptr(lightcolor));
            graphics::shaders.at(10)->setVec3("lightPos", glm::value_ptr(lightpos));
            graphics::shaders.at(10)->setFloat("alpha",1);
            ball->scale = {0.5,0.5,0.5};
            ball->rotation = {0,SDL_GetTicks64()/1.0f,0};
            ball->position = {cos(SDL_GetTicks64()/1000.0f)*4,-0.70+abs(sin(SDL_GetTicks64()/250.0f)),-6.5+sin(SDL_GetTicks64()/500.0f)*4}; //fastball
            ball->render(graphics::shaders.at(10),projection,view);
            ball->rotation = {0,SDL_GetTicks64()/100.0f,0};
            ball->position = {sin(SDL_GetTicks64()/1000.0f)*4,-0.70+abs(cos(SDL_GetTicks64()/250.0f)),-6.5+cos(SDL_GetTicks64()/500.0f)*2}; //friendly ball
            ball->scale = {0.5,
            0.5,0.5};
            ball->render(graphics::shaders.at(10),projection,view);
            ball->rotation = {0,SDL_GetTicks64()/1.0f,0};
            ball->position = {cos(-1.0*SDL_GetTicks64()/1000.0f+M_PI)*4,-0.70+abs(sin(SDL_GetTicks64()/300.0f)),-6.5+sin(-1.0*SDL_GetTicks64()/500.0f+M_PI)*4}; //inverse fastball
            ball->render(graphics::shaders.at(10),projection,view);
            
            ball->rotation = {0,SDL_GetTicks64()/1.0f,0};
            ball->position = {cos(SDL_GetTicks64()/1000.0f)*4,-1.50-abs(sin(SDL_GetTicks64()/250.0f)),-6.5+sin(SDL_GetTicks64()/500.0f)*4}; //mirror fastball
            ball->render(graphics::shaders.at(10),projection,view);
            ball->rotation = {0,SDL_GetTicks64()/100.0f,0};
            ball->position = {sin(SDL_GetTicks64()/1000.0f)*4,-1.50-abs(cos(SDL_GetTicks64()/250.0f)),-6.5+cos(SDL_GetTicks64()/500.0f)*2}; //mirror friendly ball
            ball->render(graphics::shaders.at(10),projection,view);
            ball->rotation = {0,SDL_GetTicks64()/1.0f,0};
            ball->position = {cos(-1.0*SDL_GetTicks64()/1000.0f+M_PI)*4,-1.50-abs(sin(SDL_GetTicks64()/300.0f)),-6.5+sin(-1.0*SDL_GetTicks64()/500.0f+M_PI)*4}; //mirror inverse fastball
            ball->render(graphics::shaders.at(10),projection,view);
            graphics::shaders.at(10)->activate();
            lightpos = glm::vec3(z*2, 0.5, -7.5+(-y)*2);

            graphics::shaders.at(10)->setVec3("lightPos", glm::value_ptr(lightpos));
            graphics::shaders.at(10)->setFloat("alpha",0.85);
            cd->render(graphics::shaders.at(10),graphics::sprites.at("checkerboard"),projection,view);

            ball->scale = {15,15,15};
            ball->rotation = {0,SDL_GetTicks64()/100.0f,0};
            ball->position = {20,-1,-60};
            lightpos = glm::vec3(20, 60, -60);
            graphics::shaders.at(10)->setVec3("lightPos", glm::value_ptr(lightpos));

            ball->render(graphics::shaders.at(10),projection,view);

            ball->scale = {2,2,2};
            ball->rotation = {0,SDL_GetTicks64()/1000.0f,0};
            ball->position = {20+sin(SDL_GetTicks64()/1000.0f)*30,-1,-60+cos(SDL_GetTicks64()/1000.0f)*30};
            lightpos = glm::vec3(20, 60, -60);
            ball->render(graphics::shaders.at(10),projection,view);

            glDisable(GL_DEPTH_TEST);
            if(transition < 0.15) {
                graphics::shaders.at(5)->activate();
                glm::vec4 backgroundShade = {1,1,1,(subselection==0?1.0:0.25)-(transition*(1/0.15))};
                graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(backgroundShade));
                graphics::sprite->render(graphics::shaders.at(5),graphics::sprites.at("storymode2"),{57,40},{163,400},0,{0,0},{163,400});
                backgroundShade = {1,1,1,(subselection==1?1.0:0.25)-(transition*(1/0.15))};
                graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(backgroundShade));
                graphics::sprite->render(graphics::shaders.at(5),graphics::sprites.at("storymode1"),{236+2,40},{163,400},0,{0,0},{163,400});
                backgroundShade = {1,1,1,(subselection==2?1.0:0.25)-(transition*(1/0.15))};
                graphics::shaders.at(5)->setVec4("spriteColor",glm::value_ptr(backgroundShade));

                graphics::sprite->render(graphics::shaders.at(5),graphics::sprites.at("storymodemore"),{415+2,40},{163,400},0,{0,0},{163,400});

            }

        }break;
    }
};
void gameplaymenu::reset() {
    networking::globalRPC->update("In the menu.", "Top high score: " + std::to_string(settings::maxscore), "icon6", std::time(nullptr));

    Mix_HaltMusic();
    Mix_HaltChannel(-1);

    Mix_PlayMusic( audio::music->at(1), -1 );
    startTime = SDL_GetTicks64();
    t = Transition();
}
