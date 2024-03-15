#include "animation.h"
#include <iostream>
#include "defs.h"

std::map<std::string, interpolation> animConverters::interpolationmap = {
    {"LINEAR", interpolation::LINEAR},
    {"CUBIC", interpolation::CUBIC},
    {"IMMEDIATE", interpolation::IMMEDIATE},
};
std::map<std::string, actiontype> animConverters::actionmap = {
    {"transform",actiontype::TRANSFORMANIMATION},
    {"shader",actiontype::SHADERANIMATION}
};
std::map<std::string, modifiertype> animConverters::modifiermap = {
    {"position", modifiertype::POSITION},
    {"rotation",modifiertype::ROTATION},
    {"scale",modifiertype::SCALE},
    {"selfpos",modifiertype::SHADEREFFECT}
};

animation::animation(std::vector<action> actions, transform t) {
    currentTick = 0;
    currentAction = 0;
    this->actions = actions;
    this->snapshot= t;

}
void animation::tick(double deltatime, transform* t) {
    //handle animation ticking right here
    action CURRENT = actions.at(currentAction);
    action NEXT;
    if(currentAction+1 >= actions.size()) {
        NEXT = actions.at(0);
    }
    else {
        NEXT = actions.at(currentAction+1);
    }

    int animationlength=NEXT.framenumber-CURRENT.framenumber;

    if(currentTick >= animationlength) {
        if(currentAction+1 >= actions.size()) {
            currentAction = 0;
            t->position = actions.at(0).dataToSet.at(0);
            t->rotation = actions.at(0).dataToSet.at(1);
            t->scale = actions.at(0).dataToSet.at(2);
            t->shader = actions.at(0).dataToSet.at(3);


        }
        else {
            currentAction++;
        }
        currentTick = 0;
        
        snapshot.position= {t->position.x,t->position.y,t->position.z};
        snapshot.rotation= {t->rotation.x,t->rotation.y,t->rotation.z};
        snapshot.scale= {t->scale.x,t->scale.y,t->scale.z};
        snapshot.shader= {t->shader.x,t->shader.y,t->shader.z};
    }

    currentTick += deltatime/100; //convert to deciseconds
    switch(NEXT.interpolate) {
        case CUBIC:{
            for(int i= 0; i < NEXT.effectedParameters.size();i++) {
                switch(NEXT.effectedParameters.at(i)) {
                    case POSITION: {
                        t->position.x= utils::lerp(
                            t->position.x,
                            
                            NEXT.dataToSet.at(i).x,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->position.y= utils::lerp(
                            t->position.y,
                            
                            NEXT.dataToSet.at(i).y,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->position.z= utils::lerp(
                            t->position.z,
                            
                            NEXT.dataToSet.at(i).z,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );


                    }break;
                    case ROTATION: {
                        t->rotation.x= utils::lerp(
                            t->rotation.x,
                            
                            NEXT.dataToSet.at(i).x,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->rotation.y= utils::lerp(
                            t->rotation.y,
                            
                            NEXT.dataToSet.at(i).y,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->rotation.z= utils::lerp(
                            t->rotation.z,
                            
                            NEXT.dataToSet.at(i).z,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );

                    }break;
                    case SCALE: {
                        t->scale.x= utils::lerp(
                            t->scale.x,
                            
                            NEXT.dataToSet.at(i).x,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->scale.y= utils::lerp(
                            t->scale.y,
                            
                            NEXT.dataToSet.at(i).y,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->scale.z= utils::lerp(
                            t->scale.z,
                            
                            NEXT.dataToSet.at(i).z,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );


                    }break;
                    case SHADEREFFECT: {
                        t->shader.x= utils::lerp(
                            t->shader.x,
                            
                            NEXT.dataToSet.at(i).x,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->shader.y= utils::lerp(
                            t->shader.y,
                            
                            NEXT.dataToSet.at(i).y,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );
                        t->shader.z= utils::lerp(
                            t->shader.z,
                            
                            NEXT.dataToSet.at(i).z,
                            
                            (1.0/(animationlength))*(deltatime/25)
                        );

                    }break;
                }
            }
        }break;
        case LINEAR: {
            for(int i= 0; i < NEXT.effectedParameters.size();i++) {
                switch(NEXT.effectedParameters.at(i)) {
                    case POSITION: {
                        t->position.x = utils::lerp(snapshot.position.x,NEXT.dataToSet.at(i).x,currentTick/animationlength);
                        t->position.y = utils::lerp(snapshot.position.y,NEXT.dataToSet.at(i).y,currentTick/animationlength);
                        t->position.z = utils::lerp(snapshot.position.z,NEXT.dataToSet.at(i).z,currentTick/animationlength);


                    }break;
                    case ROTATION: {
                        t->rotation.x = utils::lerp(snapshot.rotation.x,NEXT.dataToSet.at(i).x,currentTick/animationlength);
                        t->rotation.y = utils::lerp(snapshot.rotation.y,NEXT.dataToSet.at(i).y,currentTick/animationlength);
                        t->rotation.z = utils::lerp(snapshot.rotation.z,NEXT.dataToSet.at(i).z,currentTick/animationlength);

                    }break;
                    case SCALE: {
                        t->scale.x = utils::lerp(snapshot.scale.x,NEXT.dataToSet.at(i).x,currentTick/animationlength);
                        t->scale.y = utils::lerp(snapshot.scale.y,NEXT.dataToSet.at(i).y,currentTick/animationlength);
                        t->scale.z = utils::lerp(snapshot.scale.z,NEXT.dataToSet.at(i).z,currentTick/animationlength);


                    }break;
                    case SHADEREFFECT: {
                        t->shader.x = utils::lerp(snapshot.shader.x,NEXT.dataToSet.at(i).x,currentTick/animationlength);
                        t->shader.y = utils::lerp(snapshot.shader.y,NEXT.dataToSet.at(i).y,currentTick/animationlength);
                        t->shader.z = utils::lerp(snapshot.shader.z,NEXT.dataToSet.at(i).z,currentTick/animationlength);

                    }break;

                }
            }

        }break;
        case IMMEDIATE: {
            for(int i= 0; i < NEXT.effectedParameters.size();i++) {
                switch(NEXT.effectedParameters.at(i)) {
                    case POSITION: {
                        t->position.x= NEXT.dataToSet.at(i).x;
                        t->position.y= NEXT.dataToSet.at(i).y;
                        t->position.z= NEXT.dataToSet.at(i).z;


                    }break;
                    case ROTATION: {
                        t->rotation.x= NEXT.dataToSet.at(i).x;
                        t->rotation.y= NEXT.dataToSet.at(i).y;
                        t->rotation.z= NEXT.dataToSet.at(i).z;

                    }break;
                    case SCALE: {
                        t->scale.x= NEXT.dataToSet.at(i).x;
                        t->scale.y= NEXT.dataToSet.at(i).y;
                        t->scale.z= NEXT.dataToSet.at(i).z;


                    }break;
                    case SHADEREFFECT: {
                        t->shader.x= NEXT.dataToSet.at(i).x;
                        t->shader.y= NEXT.dataToSet.at(i).y;
                        t->shader.z= NEXT.dataToSet.at(i).z;
                    }break;

                }
            }
 
        }break;
    }

}