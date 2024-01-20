#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <string>

/*
Very basic type definitions for animations, including the animation ticking system
*/
enum interpolation {
    LINEAR,
    CUBIC,
    IMMEDIATE
};
enum actiontype {
    TRANSFORMANIMATION, //stupid easy transformations
    SHADERANIMATION, //applies the transform to a shader supplied value
};
enum modifiertype {
    POSITION,
    ROTATION,
    SCALE,
    SHADEREFFECT
};
class animConverters {
    public:
    static std::map<std::string, actiontype> actionmap;
    static std::map<std::string, interpolation> interpolationmap;
    static std::map<std::string, modifiertype> modifiermap;

};

struct transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 shader;
};

struct action { //used for animations and a description of how to use them
    int framenumber; //the frame that this action happens on
    interpolation interpolate; //the method to interpolate from the previous frame to this one
    actiontype type; //the type of action
    std::vector<glm::vec3> dataToSet; //the value to set
    std::vector<modifiertype> effectedParameters; //a pointer to the vec3 that is effected by each slot of the previous vector (basically, it's assigned the transform etc of an object, then applies the animation to that.)

};
class animation {
    public:
        animation(std::vector<action> actions, transform t);
        void tick(double, transform*);
        int currentAction = 0;

    private:
        std::vector<action> actions;
        double currentTick;
        transform snapshot;
};
