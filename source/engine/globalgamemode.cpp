#include "globalgamemode.h"
#include <SDL2/SDL.h>
#include "../utils/defs.h"
#include <cmath>

GlobalGamemode::GlobalGamemode() {
    float outervers[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //final side
            0.5f, -0.5f, -0.5f,  -1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  -1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  -1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //original side
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    };
    float innerverts[] = {
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, //demo side
            -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  -1.0f, -1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    };

    outer = new //cube definitions
        cube(
            glm::vec3(0.0f,0.0f,-1.705-(1.0/6.0)), //prepose
            glm::vec3(0.0f,0.0f,0.0f), //postpos
            glm::vec3((4.0000/3.0000),1.0f,(4.0000/3.0000)), //scale
            glm::vec3((0.0f),0.0f,0.0f), //rotation
            outervers, //model
            60 //size (not sure if needed)
            );
    inner = new //cube definitions
        cube(
            glm::vec3(0.0f,0.0f,-1.705-(1.0/6.0)), //prepose
            glm::vec3(0.0f,0.0f,0.0f), //postpos
            glm::vec3((4.0000/3.0000),1.0f,(4.0000/3.0000)), //scale
            glm::vec3((0.0f),0.0f,0.0f), //rotation
            innerverts, //model
            30 //size (not sure if needed)
            );
    glass = new
            model("models/glass.kmf",
            glm::vec3(0.0f,0.0f,-2.25-(1.0/6.0)), //prepose
            glm::vec3((4.0000/3.0000),1.0f,1), //scale
            glm::vec3((90.0f),0.0f,0.0f)); //rotation

}
int GlobalGamemode::logic(double deltatime) {
    if(gameplay::transitioning) {
        switch(currentTransition.fade) {
            case BARS: {
                if(alpha >= 1 && fade) {
                    fade = false;
                    return 1;
                }
                if(alpha >= 2) {
                    alpha = 0;
                    gameplay::transitioning = false;
                }
                if(alpha <= 2) {
                    alpha += deltatime*currentTransition.speed; //why is dividing deltatime a bad practice??
                }

            }break;
            default: {
                if(!fade && alpha > 0) {
                    alpha -= deltatime*0.001;
                }
                if(alpha < 0) {
                    alpha = 0;
                    gameplay::transitioning = false;
                }
                if(fade && alpha <= 1) {
                    alpha += deltatime*currentTransition.speed; //why is dividing deltatime a bad practice??
                }
                if(fade && alpha >= 1) {
                    fade = false;
                    alpha = 1;
                    return 1;
                }
            }break;
        }
    }
    return 0;
}
void GlobalGamemode::setFade(Transition resp) {
    if(!fade) {
        alpha = 0;
        fade = true;
        gameplay::transitioning = true;
        currentTransition = resp;
        switch(resp.fade) {
            case GLASS: {
                for(int i = 0; i < 64; i++) {
                    random[i] = rand();
                }
            }break;
        }
    }
    
}

void GlobalGamemode::startRender() {
    #ifndef __LEGACY_RENDER
        buffer.enable();
    #endif
}
void GlobalGamemode::render() {
    buffer.disable(640,480,true);
    if(gameplay::transitioning) {
        
        switch(currentTransition.fade) {
            case FADE: {
                #ifdef __LEGACY_RENDER
                SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255 * alpha);
                SDL_Rect splashbox = { 0, 0, 640, 480 };
                SDL_RenderFillRect(graphics::render, &splashbox);
                #else
                buffer.render(graphics::shaders[3],0,0,false);
                graphics::rect->render(graphics::shaders.at(1),{0,0},{640,480},0,{0,0,0,alpha},false,-1,{0,0,0,0});
                #endif
                break;
            }
            case BLOCKS: {
                #ifdef __LEGACY_RENDER
                SDL_SetRenderDrawColor(graphics::render, 0, 0, 0, 255);
                #else
                    buffer.render(graphics::shaders[3],0,0,false);
                #endif
                if(fade) {
                    #ifdef __LEGACY_RENDER
                    SDL_Rect splashbox = { 0, -480+math::easeOutBounce((alpha*0.75))*480, 640, 480};
                    SDL_RenderFillRect(graphics::render, &splashbox);
                    #else
                    graphics::rect->render(graphics::shaders.at(1),{
                        0,
                        0
                    },{
                        640,
                        math::easeOutBounce((alpha*0.75))*480
                        },0,{0,0,0,1},false,-1,{0,0,0,0});
                    #endif
                }
                else {
                    #ifdef __LEGACY_RENDER
                    SDL_Rect splashbox = { 
                        0,
                        -480+(1-math::easeOutBounce(1-alpha))*480,
                        640,
                        480};
                    SDL_RenderFillRect(graphics::render, &splashbox);
                    #else
                    graphics::rect->render(graphics::shaders.at(1),{
                        0,
                        0
                    },{
                        640,
                        (1-math::easeOutBounce(1-alpha))*480
                        },0,{0,0,0,1},false,-1,{0,0,0,0}
                    );
                    // graphics::rect->render(graphics::shaders.at(1),{0,-480+(1-math::easeOutBounce(1-alpha))*480},{640,480},0,{0,0,0,1},false,-1,{0,0,0,0});
                    #endif

                }
                break;
            }
            case BARS: {

                glEnable(GL_DEPTH_TEST);  
                glm::mat4 projection;
                projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
                glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
                view = glm::translate(view, glm::vec3(0.0, 0, 0.0)); 
                outer->rotation[1] = (alpha * 90.0);
                inner->rotation[1] = (alpha * 90.0);
                // cub.rotation[1] = (float)SDL_GetTicks()/10 * glm::radians(50.0f);
                inner->render(graphics::shaders[0],graphics::sprites["homophobicdog"],projection,view);
                outer->render(graphics::shaders[0],&buffer.renderTexture,projection,view);
                glDisable(GL_DEPTH_TEST);  
            }break;
            case GLASS: {
                // buffer.render(graphics::shaders[3],0,0,false);
                std::cout << "rendering glass\n";
                glEnable(GL_DEPTH_TEST);  
                glm::mat4 projection;
                projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
                glm::mat4 view = glm::mat4(1.0f); //view is the **Camera**'s perspective
                view = glm::translate(view, glm::vec3(0.0, 0, 0.0)); 
                graphics::shaders.at(0)->activate();
                buffer.renderTexture.activate(0);
                graphics::shaders.at(0)->setVector("projection", glm::value_ptr(projection));
                graphics::shaders.at(0)->setVector("view", glm::value_ptr(view));

                int randindex = 0;
                for(int i = 0; i < glass->meshes.size(); i++) {
                    glm::mat4 transform = glm::mat4(1.0f); //the actual transform of the model itself
                    transform = glm::translate(transform,glm::vec3(
                        glass->meshes.at(i).origin.x*4*alpha,
                        -glass->meshes.at(i).origin.z*8*alpha-12*alpha*alpha,
                    -2.25-(1.0/6.0)));
                    transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                    transform = glm::rotate(transform, glm::radians((float)alpha*90), glm::vec3(0.0f, 1.0f, 0.0f));
                    transform = glm::rotate(transform, glm::radians(-(float)alpha), glm::vec3(0.0f, 0.0f, 1.0f));

                    transform = glm::scale(transform, glm::vec3((4.0000/3.0000),1.0f,1.0));

                    glm::mat3 normal = glm::mat3(1.0f);
                    normal = glm::transpose(glm::inverse(transform)); //calculate normals

                    graphics::shaders.at(0)->setVector("model", glm::value_ptr(transform));
                    graphics::shaders.at(0)->setMat3("worldspace", glm::value_ptr(normal));

                    glass->meshes[i].render(graphics::shaders.at(0));

                    randindex+=2;
                }


                glDisable(GL_DEPTH_TEST);  

            }break;
        }
    }
    else {
        buffer.render(graphics::shaders[3],0,0,false);
    }

}
