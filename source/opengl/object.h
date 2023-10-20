/*
 * Object.h, and the future of computing
 * By Knuckles F. Videos
 *
 * Object.h was originally written at exactly 1/1/23 at 12:01AM.
 * What was the purpose of Object.h? Why, to declare everything that would be needed for objects.
 * As the first lines of code written in the new year, Object.h was created as the future of all code going forward.
 * So, what standard did I set myself to acheieve this year with my future code?
 *
 * Tis but a simple answer... As Object.h is utter and disgusting garbage, I find it only fitting that Object.h kicks off the year
 * As you see, I write awful code.
 * Yes, indeed, you read that correctly. Awful code. Straight repugnant even! As a matter of fact, I couldn't see myself writing worse code if I tried!
 *
 * Here's to the new year, lets hope I find what I'm looking for...
 *
 *
 * (...that being good code of course.)
*/
#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader.h"
#include "texture.h"
#include <iostream>
//this big file contains the definitions and default values for ALL objects in the game (and what the generic classes they extend)
//To generically access an object, all you would really need is it's logic and it's rendering.
//Most objects that are static wrappers do not need logic and therefore do not have them.
//ALSO pro tip for anyone writing objects that AREN'T lua scripted
//make sure to have objects in lua capitalize each word in their names
//this is to set them apart from their lua competitors, which are written in camelcase
//just a suggestion, but i'd follow it imo

enum LIGHTTYPE {
    POINT,
    SPOTLIGHT,
    DIRECTIONAL
};

class TransformObject {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        virtual void render(shader* shad, glm::mat4 projection, glm::mat4 view, int slot){
            shad->setVector("projection", glm::value_ptr(projection));
            shad->setVector("view", glm::value_ptr(view));
        };
        virtual void logic(double delta) {};
        std::vector<TransformObject*> children;
        TransformObject* parent;
};
class Light : public TransformObject {
    public:
        Light(LIGHTTYPE type, glm::vec3 position, glm::vec3 ambient=glm::vec3(0.1f,0.1f,0.1f), glm::vec3 diffuse=glm::vec3(1.0f,1.0f,1.0f), glm::vec3 specular=glm::vec3(0,0,0), float constant=0, float linear=0, float quadratic=0);
        void update(shader*, int);
        void render(shader* shad, glm::mat4 projection, glm::mat4 view, int slot);
        LIGHTTYPE type;
        float constant;
        float linear;
        float quadratic;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

};
class Camera: public TransformObject  {
    //yes, cameras ARE gameobjects, and they are handled just as one would be.
    //no render texture means it renders to the screen
    //don't render multiple cameras at once unless they're to render textures because that's just redundant and also will be glitchy
    //really all you kinda need to know outside of the kinda built in input.
    //Remember, if the rotation changes, PLEASE update the camera. POSITION IS FINE.
    // These default cameras are not designed to be updated,
    //so consider them static,  unless you implement some way to update their vectors every frame.
    //(i'll have an example just in case.)

public:

    Camera(glm::vec3 position, glm::vec3 rotation, int fov, texture* rendertexture, int w, int h, float near, float far) {
        this->position = position;
        this->rotation = rotation;
        this->fov = fov;
        this->render = rendertexture;
        width = w;
        height = h;
        projection = glm::perspective(glm::radians((float)fov), (float)w / (float)h, near, far);
    };
    glm::mat4 projection;

    glm::mat4 view() { //gets the view for usage
        return glm::lookAt(position, position+front, up);
    };
    int width, height, fov; //all used for projection calculations

    //the following is shamelessly stolen from learnopengl.com, and i will be rewriting this (yknow, if i need to.)
    void updateView() {
        glm::vec3 tempfront;
        tempfront.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
        tempfront.y = sin(glm::radians(rotation.y));
        tempfront.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
        front = glm::normalize(tempfront);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        //up    = glm::normalize(glm::cross(right, front));
        std::cout << rotation.x << " " << rotation.y << "\n";

    };

protected:
    //updates the FPS vectors in case of movement.
    texture* render; //if nullptr, then it just doesnt render to a texture lmao
    //the following is only *truly* necessary if used in an FPS context (which I do want to create soon, hence the unused code for now)
    //in theory, you could implement a way to do this without these 3 vectors, but having them makes it easier soooo
    //BTW, highly encourage writing your own camera, lights etc and NOT using mine in some situations.

    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up    = glm::vec3(0.0f, 1.0f,  0.0f);
    glm::vec3 right    = glm::vec3(0.0f, 1.0f,  0.0f);

};

#endif // OBJECT_H
