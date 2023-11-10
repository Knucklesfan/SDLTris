#ifdef __LEGACY_RENDER
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
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#ifdef __SWITCH__
#define pth  "/"
#include <switch.h>

#else
#define pth  "./"
#endif

bg::bg() {}
bg::bg(std::string path, bool folder, SDL_Renderer* renderer) {
    std::string filepath = pth "backgrounds_legacy/" + path + "/theme.xml";
    if(folder) {
        filepath = path + "/theme.xml";
    }

    rapidxml::file<> xmlFile(filepath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    layers = atoi(doc.first_node("layers")->value());

    std::string p = pth "backgrounds_legacy/" + path;
    if(folder) {
        p = path;
    }
    
    generateSurfaces(p, renderer); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!

    
    name = doc.first_node("name")->value();
    creator = doc.first_node("creator")->value();
    vers = doc.first_node("vers")->value();
    songname = doc.first_node("musicname")->value();
    artist = doc.first_node("musicartist")->value();
    rotation = 0;

    if (doc.first_node("rotation") != NULL) {
        rotation = atoi(doc.first_node("rotation")->value());
    }
    if (doc.first_node("sine") != NULL) {
        sine = true;
        sinelayer = atoi(doc.first_node("sine")->value());
        snheight = atoi(doc.first_node("sineheight")->value());
        snwidth = atoi(doc.first_node("sinewidth")->value());
        snwid = atoi(doc.first_node("sinelayerheight")->value());\
        rate = atoi(doc.first_node("sinerate")->value());
    }


    if (doc.first_node("fglayer") != NULL) {
        std::cout << "fglayer detected\n";
        fglayer = atoi(doc.first_node("fglayer")->value());
    }
    if (doc.first_node("thumbnail") != NULL) {
        std::cout << "thumbnail detected\n";
        std::string thmbpath = pth "backgrounds_legacy/" + path + "/";
        thmbpath += doc.first_node("thumbnail")->value();
        thumbnail = SDL_CreateTextureFromSurface(renderer, IMG_Load(thmbpath.c_str()));
    }
    else {
        std::string thmbpath = pth "backgrounds_legacy/nullbg.png";
        thumbnail = SDL_CreateTextureFromSurface(renderer, IMG_Load(thmbpath.c_str()));

    }

    int array[10];
    std::cout << "LAYERS: " << layers;
    for(int i = 0; i < layers; i++) {

        std::string sr = "layer";
        sr += std::to_string(i);
        std::cout << "TESTING::: " << sr << "\n";
        incrementsx[i] = atoi(doc.first_node(sr.c_str())->value());
        std::string sy = "layer";
        sy += std::to_string(i);
        sy += "y";
        incrementsy[i] = atoi(doc.first_node(sy.c_str())->value());
        //std::cout << "INFORMATION!!!: " << atoi(doc.first_node(sy.c_str())->value()) << "\n";
    }

    std::string muspath = pth "backgrounds_legacy/" + path + "/";
    if(folder) {
        muspath = path + "/";
    }

    muspath += doc.first_node("music")->value();
    music = Mix_LoadMUS(muspath.c_str());
    if (!music) {
        printf("Failed to load music at %s: %s\n", muspath.c_str(), SDL_GetError());
    }

    std::cout << "Finished loading: " << name << "\n";
    doc.clear();
}

void  bg::generateSurfaces(std::string path, SDL_Renderer* renderer) {
    std::vector<SDL_Surface*> surfaces;
    for(int i = 0; i < layers; i++) {
        char buff[12];
        snprintf(buff, sizeof(buff), "%02d", i);
        std::string temppath = path + "/" + buff + ".png";

        SDL_Surface* temp = IMG_Load(temppath.c_str());
        if (!temp) {
            printf("Failed to load image at %s: %s\n", temppath.c_str(), SDL_GetError());
        }
        surfaces.push_back(temp);
        printf("Successfully loaded image at %s\n", temppath.c_str());
    }
    for (SDL_Surface* surf : surfaces) {
        SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, surf);
        if(temp != NULL) {
            textures.push_back(temp);
            printf("pushed texture!!");
            SDL_FreeSurface(surf);
        }
        else {
            fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        }
    }

}
void bg::render(SDL_Renderer* renderer, bool layer) {
    //OKAY EXPLAINATION FOR MY ACTIONS:
    //dear whoever is reading this:
    //this code is a bad practice.

    int addition = 0;
    int max = layers;
    if (fglayer > 0) {
        max = fglayer;
        if (layer) {
            max = layers;
            addition = fglayer;
        }
    }
    else if (layer && fglayer == 0) {
        return;
    }

    for (int i = addition; i < max; i++) {
        int width, height;
        SDL_QueryTexture(textures[i], NULL, NULL, &width, &height);
        double tempx = 0;
        double tempy = 0; //yuck
        int multiplerx = 1; //this is really bad practice but it's currently 11pm and i wanna feel accomplished
        int multiplery = 1;
        //std::cout << incrementsx[i] << i << "\n";
        if (incrementsx[i] != 0) {
            tempx = fmod(layerposx[i], width); //ew
        }
        if (incrementsy[i] != 0) {
            tempy = fmod(layerposy[i], height); //GROSS CODE
        }
        if (layerposx[i] > 0) {
            multiplerx = -1;
        }
        if (layerposy[i] > 0) {
            multiplery = -1;
        }
        bool dothis = sine && i == sinelayer;
        drawLayer(renderer, textures[i],tempx,tempy,multiplerx,multiplery,width,height,
        dothis,
        snwid, //wave width in pixels
        snwidth, //sine width
        snheight, //sine height
        angle //increment. 
        );
        }
}
void bg::logic(double deltatime)
{
    angle += deltatime / rate;
    //std::cout << angle << "\n";

    for(int i = 0; i < layers; i++) {
        if(incrementsx[i] != 0) {
            layerposx[i] -= (deltatime)/(incrementsx[i]);
        }
        if(incrementsy != 0) {
            layerposy[i] -= (deltatime)/(incrementsy[i]);
        }

    }
}

void bg::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
    SDL_Rect sprite;
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;
    if (center) {
        sprite.x = x - oldwidth / 2;
        sprite.y = y - oldheight / 2;
    }
    else {
        sprite.x = x + oldwidth / 2 - sprite.w / 2;
        sprite.y = y + oldheight / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(renderer, texture, NULL, &sprite, 0, NULL, SDL_FLIP_NONE);
}

//lol i stole more code from font.h
//I had this great idea for doing waves through like modifying pixel data, and then I realized "oh crap, that'll use up way too much GPU to handle"

//so then i cried

void bg::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
    SDL_Rect sprite;
    SDL_Rect srcrect = {srcx, srcy, srcw, srch};
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    sprite.w = srcw * scale;
    sprite.h = srch * scale;
    if (center) {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else {
        sprite.x = x + srcw / 2 - sprite.w / 2;
        sprite.y = y + srch / 2 - sprite.h / 2;
    }
    SDL_RenderCopy(renderer, texture, &srcrect, &sprite);
    //since the angle system doesnt even work
    //BUT I SWEAR GUYS ILL GET IT TO WORK EVENTUALLY
}
//anyways so that's the new and totally awesome relevant modern new background system thing that everyone definitely loves


//(edit added pun)
bool bg::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me right https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

void bg::drawLayer(SDL_Renderer* renderer, SDL_Texture* texture, int tempx, int tempy, int multiplerx, int multiplery, int width, int height, bool wavy, int wavywidth, int sinewidth, int sineheight, double sinepos) {
    if (wavy) {
        for(int i = 0; i < height; i+=wavywidth) {
            double sinex = (sin((sinepos + i) * sinewidth) * sineheight);

            drawTexture(renderer, texture, tempx+sinex, (tempy+i), fmod(angle, 360), 1.0, false,0,i,width,wavywidth);

            drawTexture(renderer, texture, tempx+sinex + (width * multiplerx), (tempy+i) + (height * multiplery), fmod(angle, 360), 1.0, false,0,i,width,wavywidth);
            
            drawTexture(renderer, texture, tempx+sinex, (tempy+i)+(height * multiplery), fmod(angle, 360), 1.0, false,0,i,width,wavywidth);

            drawTexture(renderer, texture, tempx+sinex + (width * multiplerx), tempy+i, fmod(angle, 360), 1.0, false,0,i,width,wavywidth);
        }
    }
    else {
        drawTexture(renderer, texture, tempx, tempy, fmod(angle, 360), 1.0, false);
        drawTexture(renderer, texture, tempx + (width * multiplerx), tempy + (height * multiplery), fmod(angle, 360), 1.0, false);
        drawTexture(renderer, texture, tempx + 0, tempy + (height * multiplery), fmod(angle, 360), 1.0, false);
        drawTexture(renderer, texture, tempx + (width * multiplerx), tempy, fmod(angle, 360), 1.0, false);
    }

}
#else

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
#include "utils/defs.h"

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
    graphics::shaders[4]->setVec4("sineinfo",glm::value_ptr(sinedata));
    glm::vec2 texinfo = glm::vec2(t.w,t.h);
    graphics::shaders[4]->setVec2("texinfo",glm::value_ptr(texinfo));

    graphics::shaders[4]->setInt("image",0);
    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

}
shaderlayer::shaderlayer(std::string vert,std::string frag, std::vector<texture*> textures) {
    shad = new shader(vert,frag);
    data = textures;
    //generates the VBO and stufff
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
void shaderlayer::logic(double deltatime) {
}
void shaderlayer::render() {
    glDepthMask(GL_FALSE);
    int i = 0;
    // std::cout << data.size() << "\n";
    shad->activate();
    for(texture* t : data) {
        t->activate(i);
        shad->setInt("texture"+std::to_string(i),i);
        i++; //forgot to increment this... wtf is wrong with me
    }
        shad->setFloat("time",(float)SDL_GetTicks()/(float)1000);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
}

flatlayer::flatlayer(std::string vert,std::string frag, std::vector<texture*> textures, transform t) {
    //this class has a lot in common with the shader layer, but with the key difference that the shader layer is designed to just show shaders, while this one can show a LOT more
    shad = new shader(vert,frag);
    data = textures;
    trans = t;
    //generates the VBO and stufff
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
	projection = glm::perspective(glm::radians(45.0f), (float)INTERNAL_WIDTH / (float)INTERNAL_HEIGHT, 0.001f, 10000.0f);
	view = glm::mat4(1.0f); //view is the **Camera**'s perspective
	view = glm::translate(view, glm::vec3(0.0, 0, -6.0)); 
}
void flatlayer::render() {
    matTrans = glm::mat4(1.0f); //the actual transform of the model itself

    matTrans = glm::rotate(matTrans, glm::radians(trans.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));  
	matTrans = glm::rotate(matTrans, glm::radians(trans.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));  
	matTrans = glm::rotate(matTrans, glm::radians(trans.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));  
	matTrans = glm::scale(matTrans, trans.scale);
	matTrans = glm::translate(matTrans,trans.position);

    shad->activate();
    int i = 0;
    for(texture* t : data) {
        t->activate(i);
        shad->setInt("texture"+std::to_string(i),i);
        i++; //forgot to increment this... wtf is wrong with me
    }
    shad->setVector("model", glm::value_ptr(matTrans));
    shad->setVector("projection", glm::value_ptr(projection));
	shad->setVector("view", glm::value_ptr(view));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

};
void flatlayer::logic(double deltatime) {

}

bg::bg() {
    buffer = new buffermanager(640,480);
}
bg::bg(std::string path, bool folder) {
    postprocess = false;
    buffer = new buffermanager(640,480);

    std::string filePath = pth "backgrounds/" + path + "/theme.xml";
    std::cout << filePath <<"\n";
    if(folder) {
        filePath = path + "/theme.xml";
    }

    rapidxml::file<> xmlFile(filePath.c_str());
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
            case headerdata::BGMUSIC: {
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
                case layertype::SHADER: {
                    rapidxml::xml_node<char>* shaderpath = child->first_node("shader");
                    if(shaderpath == nullptr) {
                        std::cout << "Failed to load shader for " << path << ".\n";
                        return;
                    }   
                    std::vector<texture*> textures;
                    rapidxml::xml_node<char>* texturesPath = child->first_node("textures");
                    if(texturesPath == nullptr) {
                        std::cout << "Failed to load texture for " << path << ".\n";
                        return;
                    }  
                    for (rapidxml::xml_node<char>* chlds = child->first_node("textures")->first_node(); chlds != NULL; chlds = chlds->next_sibling()) {
                        texture* tmp = new texture(pth "backgrounds/" + path + "/" +chlds->first_node("path")->value());
                        if(tmp != nullptr) {
                            textures.push_back(tmp);
                        }
                    }
                    std::string vertpath = pth "backgrounds/" + path + "/" +shaderpath->first_node("vertex")->value();
                    std::string fragpath = pth "backgrounds/" + path + "/" +shaderpath->first_node("fragment")->value();
                    l = new shaderlayer(vertpath,fragpath,textures);
                    
                }break;
                case layertype::BG2D: {
                    rapidxml::xml_node<char>* shaderpath = child->first_node("shader");
                    if(shaderpath == nullptr) {
                        std::cout << "Failed to load shader for " << path << ".\n";
                        return;
                    }   
                    std::vector<texture*> textures;
                    rapidxml::xml_node<char>* texturesPath = child->first_node("textures");
                    if(texturesPath == nullptr) {
                        std::cout << "Failed to load texture for " << path << ".\n";
                        return;
                    }  
                    rapidxml::xml_node<char>* transformPath = child->first_node("transform");
                    if(texturesPath == nullptr) {
                        std::cout << "Failed to load transform for " << path << ".\n";
                        return;
                    }  

                    for (rapidxml::xml_node<char>* chlds = child->first_node("textures")->first_node(); chlds != NULL; chlds = chlds->next_sibling()) {
                        texture* tmp = new texture(pth "backgrounds/" + path + "/" +chlds->first_node("path")->value());
                        if(tmp != nullptr) {
                            textures.push_back(tmp);
                        }
                    }
                    std::string vertpath = pth "backgrounds/" + path + "/" +shaderpath->first_node("vertex")->value();
                    std::string fragpath = pth "backgrounds/" + path + "/" +shaderpath->first_node("fragment")->value();
                    transform t;
                    t.position.x = atoi(transformPath->first_node("position")->first_node("x")->value());
                    t.position.y = atoi(transformPath->first_node("position")->first_node("y")->value());
                    t.position.z = atoi(transformPath->first_node("position")->first_node("z")->value());
                    t.rotation.x = atoi(transformPath->first_node("rotation")->first_node("x")->value());
                    t.rotation.y = atoi(transformPath->first_node("rotation")->first_node("y")->value());
                    t.rotation.z = atoi(transformPath->first_node("rotation")->first_node("z")->value());
                    t.scale.x = atoi(transformPath->first_node("scale")->first_node("x")->value());
                    t.scale.y = atoi(transformPath->first_node("scale")->first_node("y")->value());
                    t.scale.z = atoi(transformPath->first_node("scale")->first_node("z")->value());
                    
                    l = new flatlayer(vertpath,fragpath,textures,t);
                    
                }break;

                default: std::cout<<"unimplemented layer type\n";break;
            }
            if(l != nullptr) {
                if(child->first_attribute("depth") != nullptr) {
                    l->depth = atoi(child->first_attribute("depth")->value());
                }
                layers.push_back(l);
            }
        }
        else {
            std::cout << "unknown element " << child->name() <<"! Ignored.\n";
        }
    }
    
    rapidxml::xml_node<char>* framebuffer = doc.first_node("background")->first_node("framebuffer");
    if(framebuffer != nullptr) {
        rapidxml::xml_node<char>* shaderpath = framebuffer->first_node("shader");
        if(shaderpath == nullptr) {
            std::cout << "Failed to load shader for " << path << ".\n";
            return;
        }   
        std::vector<texture*> shaderlayertextures;
        rapidxml::xml_node<char>* texturesPath = framebuffer->first_node("textures");
        if(texturesPath == nullptr) {
            std::cout << "Failed to load texture for " << path << ".\n";
            return;
        }  
        shaderlayertextures.push_back(&buffer->renderTexture);
        for (rapidxml::xml_node<char>* chlds = framebuffer->first_node("textures")->first_node(); chlds != NULL; chlds = chlds->next_sibling()) {
            texture* tmp = new texture(pth "backgrounds/" + path + "/" +chlds->first_node("path")->value());
            if(tmp != nullptr) {
                shaderlayertextures.push_back(tmp);
            }
        }
        std::string vertpath = pth "backgrounds/" + path + "/" +shaderpath->first_node("vertex")->value();
        std::string fragpath = pth "backgrounds/" + path + "/" +shaderpath->first_node("fragment")->value();
        postproc = new shaderlayer(vertpath,fragpath,shaderlayertextures);
        postprocess = true;

    }

    std::cout << "Finished loading: " << name << "\n";
    doc.clear();
}

void bg::render() {
    //BACKGROUND.H RETURNS!!!
    buffer->enable();
    for(int i = 0; i < layers.size(); i++) {
        layers[i]->render();
    }
    buffer->disable(640,480,true);
    if(postprocess) {

        postproc->render();
    }
    else {
        buffer->render(graphics::shaders[3],0,0,false);
    }
}
void bg::logic(double deltatime)
{
    for(int i = 0; i < layers.size(); i++) {
        layers[i]->logic(deltatime);
    }
    
}

#endif