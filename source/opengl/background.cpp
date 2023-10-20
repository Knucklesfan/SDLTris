#include "background.h"
#include <string>
#include <vector>
#include <algorithm>    // std::sort
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <iostream>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <cmath>

#ifdef __SWITCH__
#define pth  "/"
#include <switch.h>

#else
#define pth  "./"
#endif
#include "utils.h"

staticlayer::staticlayer(std::string path) {
    t = texture(path);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void staticlayer::render() {
    if(graphics::shaders.size() > 0) {
        glDepthMask(GL_FALSE);
        t.activate(0);
        graphics::shaders[2]->activate();
        graphics::shaders[2]->setInt("texture1",0);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
    }
}

legacylayer::legacylayer(std::string path, glm::vec2 vel, glm::vec4 sn) {
    std::cout << path << "\n";
    t = texture(path);
    velocity = vel;
    sine = sn;
    //generates the VBO and stuff
    unsigned int VBO;

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void legacylayer::logic(double deltatime) { //this is copied exactly how SDLTris did it, so any effects that work there should work here.
    angle += deltatime / sine.w;
    if(velocity.x != 0) { //this is dumb, and i will fix this, it's just... its late and im tired.
        position.x -= (deltatime)/(velocity.x);
        position.x = fmod(position.x,t.w);
    }
    if(velocity.y != 0) {
        position.y -= (deltatime)/(velocity.y);
        position.y = fmod(position.y,t.h);

    } //TODO: ask a friend that knows more about math (probably mason)
}
void legacylayer::render() {
    glDepthMask(GL_FALSE);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(COORDINATE_WIDTH), 
    static_cast<float>(COORDINATE_HEIGHT), 0.0f, -1.0f, 1.0f);
	glm::vec2 texOffset = glm::vec2(0,0);
	glm::vec2 texScale = glm::vec2(3,3);

	t.activate(0);
    graphics::shaders[4]->activate();
	graphics::shaders[4]->setVector("projection",glm::value_ptr(projection));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position.x-t.w,position.y-t.h, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    model = glm::translate(model, glm::vec3(0.5f * t.w, 0.5f * t.h, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, glm::vec3(-0.5f * t.w, -0.5f * t.h, 0.0f)); // move origin back

    model = glm::scale(model, glm::vec3(t.w*3,t.h*3, 1.0f)); // last scale

    graphics::shaders[4]->setVector("model", glm::value_ptr(model));
	graphics::shaders[4]->setVec2("texOffset",glm::value_ptr(texOffset));
	graphics::shaders[4]->setVec2("scale",glm::value_ptr(texScale));
    glm::vec4 sinedata = glm::vec4(sine.x,sine.y,sine.z,angle);
    std::cout << sinedata.x << " " << sinedata.y << " " << sinedata.z << " " << sinedata.w << "\n"; 
    graphics::shaders[4]->setVec4("sineinfo",glm::value_ptr(sinedata));
    glm::vec2 texinfo = glm::vec2(t.w,t.h);
    graphics::shaders[4]->setVec2("texinfo",glm::value_ptr(texinfo));

    graphics::shaders[4]->setInt("image",0);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

}



bg::bg() {}
bg::bg(std::string path, bool folder) {

    std::string filepath = pth "backgrounds/" + path + "/theme.xml";
    std::cout << filepath <<"\n";
    if(folder) {
        filepath = path + "/theme.xml";
    }

    rapidxml::file<> xmlFile(filepath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    rapidxml::xml_node<char>* header = doc.first_node("background")->first_node("head");
    if(header == nullptr) {
        std::cout << "Failed to load head for " << path << ".\n";
        return;
    }
    for (rapidxml::xml_node<char>* child = header->first_node(); child != NULL; child = child->next_sibling()) {
        switch(bgconverters::headermap[child->name()]) {
            case headerdata::CREATOR: {
                creator = child->value();
                break;
            }
            case headerdata::TITLE: {
                name = child->value();
                break;
            }
            case headerdata::MUSIC: {
                for (rapidxml::xml_node<char>* muskid = child->first_node(); muskid != NULL; muskid = muskid->next_sibling()) {
                    switch(bgconverters::headermap[muskid->name()]) {
                        case headerdata::TITLE: {
                            songname = muskid->value();
                            break;
                        }
                        case headerdata::CREATOR: {
                            artist = muskid->value();
                            break;
                        }
                        case headerdata::FILENAME: {
                            std::string musicpath = pth "backgrounds/" + path + "/" + muskid->value();
#ifdef CLIENT
                            music = Mix_LoadMUS(musicpath.c_str());
                            if (!music) {
                                printf("Failed to load music at %s: %s\n", musicpath.c_str(), SDL_GetError());
                            }
#endif
                            break;
                        }
                        default: {
                            std::cout << "unknown musical element! Ignored.\n";
                        }
                    }
                }
                break;
            }
            case headerdata::VERSION: {
                vers = child->value();
                break;
            }
            default: {
                std::cout << "unknown header element! Ignored.\n";
            }
        }
    }
    rapidxml::xml_node<char>* lyrs = doc.first_node("background")->first_node("layers");
    if(lyrs == nullptr) {
        std::cout << "Failed to load layers for " << path << ".\n";
        return;
    }

    for (rapidxml::xml_node<char>* child = lyrs->first_node(); child != NULL; child = child->next_sibling()) {
        std::string name = child->name();
        layer* l;
        if(name == "layer") {
            switch(bgconverters::layermap[child->first_attribute("type")->value()]) {
                case layertype::BACKGROUND: {
                    rapidxml::xml_node<char>* tmppath = child->first_node("filename");
                    if(tmppath == nullptr) {
                        std::cout << "Failed to load texture for " << path << ".\n";
                        return;
                    }   
                    std::string layerpath = pth "backgrounds/" + path + "/" + tmppath->value();
                    l =new staticlayer(layerpath);
                }break;
                case layertype::LEGACY: {
                    rapidxml::xml_node<char>* tmppath = child->first_node("filename");
                    if(tmppath == nullptr) {
                        std::cout << "Failed to load texture for " << path << ".\n";
                        return;
                    }   
                    std::string layerpath = pth "backgrounds/" + path + "/" + tmppath->value();
                    rapidxml::xml_node<char>* movement = child->first_node("movement");
                    if(tmppath == nullptr) {
                        std::cout << "Failed to load movement for " << path << ".\n";
                        return;
                    }   
                    rapidxml::xml_node<char>* x = movement->first_node("x");
                    rapidxml::xml_node<char>* y = movement->first_node("y");
                    
                    rapidxml::xml_node<char>* sine = movement->first_node("sine");
                    glm::vec4 sinedata = glm::vec4(0.0f,0.0f,1.0f,0.0f);
                    if(sine != nullptr) {
                        std::cout << "detected\n";
                        sinedata.x = atoi(sine->first_node("width")->value());
                        sinedata.y = atoi(sine->first_node("height")->value());
                        sinedata.z = atoi(sine->first_node("layerheight")->value());
                        sinedata.w = atoi(sine->first_node("rate")->value());

                    }   

                    l = new legacylayer(layerpath, glm::vec2(atoi(x->value()),atoi(y->value())),sinedata);
                }break;

                default: std::cout<<"unimplemented layer type\n";break;
            }
            if(l != nullptr) {
                layers.push_back(l);
            }
        }
        else {
            std::cout << "unknown element " << child->name() <<"! Ignored.\n";
        }
    }

    std::cout << "Finished loading: " << name << "\n";
    doc.clear();
}

void bg::render(shader* shad) {
    //BACKGROUND.H RETURNS!!!
    for(int i = 0; i < layers.size(); i++) {
        layers[i]->render();
    }
}
void bg::logic(double deltatime)
{
    for(int i = 0; i < layers.size(); i++) {
        layers[i]->logic(deltatime);
    }

}
