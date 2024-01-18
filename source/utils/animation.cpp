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
    {"scale",modifiertype::SCALE}
};

animation::animation(std::vector<action> actions) {
    currentTick = 0;
    currentAction = 0;
    this->actions = actions;
}
void animation::tick(double deltatime) {
    
    //handle animation ticking right here
    currentTick += deltatime/100; //convert to deciseconds
    action CURRENT = actions.at(currentAction);
    action NEXT;
    if(currentAction+1 < actions.size()) {
        NEXT = actions.at(0);
    }
    else {
        NEXT = actions.at(currentAction);
    }
    switch(actions.at(currentAction).interpolate) {
        case CUBIC:{

        }break;
        case LINEAR: {
            for(int i= 0; i < CURRENT.effectedParameters.size();i++) {
                std::cout << "size: " << CURRENT.effectedParameters.size() << " i: " << i << " current action: " << currentAction << " actions: " << actions.at(currentAction).dataToSet.size() << " \n";
                CURRENT.effectedParameters.at(i)->x = 
                utils::lerp(actions.at(currentAction).effectedParameters.at(i)->x,
                actions.at(currentAction).dataToSet.at(i).x,
                (1/NEXT.framenumber-CURRENT.framenumber)*(deltatime/100)
                );
            }
        }break;
        case IMMEDIATE: {
            
        }break;
    }
}