#include "pixfont.h"
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <map>
#include <utility>      // std::pair, std::make_pair
#include <math.h>


//TODO: Make this an actual define in the compiler instead of a janky def
//Also why does this have switch support if the switch port doesn't work
//TODO: actually make a decent switch port
#ifdef __SWITCH__
#define filepath  "/"
#include <switch.h>

#else
#define filepath  "./"
#endif


pixfont::pixfont() {

}

//this is um, also copied from background.h and modified lmao
//I sWeAr I'm A gOoD cOdEr gUyS

pixfont::pixfont(std::string path) {
    
    std::string p = "./fonts/" + path;

    std::string pth = filepath "fonts/" + path + "/fontdef.xml";

    rapidxml::file<> xmlFile(pth.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    std::string texturepath = (p+"/"+doc.first_node("filename")->value());
    txt = new texture(texturepath);

    width = atoi(doc.first_node("width")->value());
    height = atoi(doc.first_node("height")->value());
    shad = graphics::shaders.at(atoi(doc.first_node("shader")->value())); //specifies the shader slot number to pick from the shaders

    if (doc.first_node("spwidth") != NULL) {
        wordsize = atoi(doc.first_node("spwidth")->value());
    }
    else {
        wordsize = width;
    }
    rapidxml::xml_node<char>* parent = doc.first_node("letters");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
        letter let;

        let.x = atoi(child->first_node("posx")->value());
        let.y = atoi(child->first_node("posy")->value());
        let.width = atoi(child->first_node("charwid")->value());
        let.character = child->first_node("char")->value()[0];
        mapping[let.character] = let;
        std::cout << let.character << " " << let.x << "\n";
    }

    //handle opengl stuff now
    projection = glm::ortho(0.0f, static_cast<float>(COORDINATE_WIDTH), 
    static_cast<float>(COORDINATE_HEIGHT), 0.0f, -100.0f, 100.0f);

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
        // also set instance data
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * MAXSTRING, &translations[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.
    doc.clear();
}


int pixfont::render(int x, int y, std::string words, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout) {
return render(words, x, y, center, red, blue, green, wordwrap, sine, pos, multiplyin, multiplyout, 1);
}
int pixfont::render(int x, int y, std::string strg, bool center) {
return render(strg, x, y, center, 255, 255, 255, 0, false, 0, 0, 0, 1);
}
int pixfont::render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout, double scale) {
	txt->activate(0);

    shad->activate();
	shad->setVector("projection",glm::value_ptr(projection));
    glBindVertexArray(this->quadVAO);

    int finalwidth = 0;
    int drawcolor = 0;
    if(center) {
        if (wordwrap > 0 && words.length()*wordsize > wordwrap) {
            finalwidth = wordwrap;
        }
        else {
            finalwidth = words.length() * wordsize;
        }
    }
    glm::vec4 color = {0,0,0,1};
    //coloring yet to be supported by ogl renderer
    if(red > 0 || blue > 0 || green > 0) {
        color.r = red/255.0;
        color.g = green/255.0;
        color.b = blue/255.0;
        color.w = 1;
    }
    // std::cout << words << "\n";
    if(wordwrap > 0  && words.length()*wordsize > wordwrap) {
        words = wrap(words, wordwrap/ wordsize);

    } //sorry, not yet
    std::vector<std::string> wordVector = split(words,'\n');
    double tmpy = y;
    for(std::string word : wordVector) {
    int i = 0;
    int lines = 0;
    int tmpx = center?(x-(word.length() * wordsize)/2):x;
    for(char& c : word) {
        if (i >= 1 && word.at(i-1) == '@') {
            i++;
            continue;
        }
        if(i == 0 && lines > 0 && c == ' ') {
            continue;
        }
        #ifdef __LEGACY_RENDER
        if(red == 255 && green == 255 && blue == 255) {
            SDL_SetTextureColorMod(texture, colors[drawcolor].r, colors[drawcolor].g, colors[drawcolor].b);
        }
        else { //i feel sick writing code like this
            SDL_SetTextureColorMod(texture, red, green, blue);
        }
        #else
        shad->setVec4("spriteColor",glm::value_ptr(color));

        #endif
        char a = c;
        if (mapping.find(a) == mapping.end()) {
            a = std::toupper(c);
        }
        if (mapping.find(a) != mapping.end()) {
            double drawy = tmpy;
            if (sine) {
                drawy = (tmpy + (sin((pos + i) * multiplyin) * multiplyout));
            }
        #ifdef __LEGACY_RENDER
            drawTexture(txt, tmpx, drawy, 0, scale, false, mapping.at(a).x * width, mapping.at(a).y * height, mapping.at(a).width, height);
        #else
            // graphics::sprite->render(shad, txt,
            // glm::vec2(tmpx,drawy), //position to draw at
            // glm::vec2(mapping.at(a).width, height), //width and height to draw
            // 0, //rotation
            // glm::vec2(mapping.at(a).x * width,mapping.at(a).y * height), //where in texture to grab from
            // glm::vec2(mapping.at(a).width, height)); //width and height to grab

            glm::vec2 texOffset = glm::vec2((float)(mapping.at(a).x * width)/txt->w,(float)(mapping.at(a).y * height)/txt->h);
            glm::vec2 texScale = glm::vec2((float)(mapping.at(a).width)/txt->w,(float)(height)/txt->h);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(tmpx,drawy, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
            model = glm::scale(model, glm::vec3(mapping.at(a).width,height, 1.0f)); // last scale
            shad->setVector("model", glm::value_ptr(model));
            shad->setVec2("texOffset",glm::value_ptr(texOffset));
            shad->setVec2("scale",glm::value_ptr(texScale));
            shad->setInt("image",0);
            
            glDrawArrays(GL_TRIANGLES, 0, 6);


        #endif
            tmpx += (mapping.at(a).width);
        }
        else {
            //std::cout << "LOADED BAD CHAR!!\n";
            if (a == '\n') {
                tmpy += height;     
                lines++;  

            }
            else if (a == '@') {
                if (i + 1 < word.length()) {
                    drawcolor = atoi(&word.at(i + 1));
                    i++;
                }
                continue;
            }
            else {
                tmpx += wordsize;
            }
        }
        i++;
    }
    

    tmpy += wordsize;
    lines++;  

    #ifdef __LEGACY_RENDER
    SDL_SetTextureColorMod(txt, 255,255,255);
    #endif
    }
    glBindVertexArray(0);
    return wordVector.size();

}
//uses the modern drawtexture from background.h, hopefully there's no bugs?

//had to add a few more parameters becauuuuuse rendering sprite sheets is our new intention with drawTexture
#ifdef __LEGACY_RENDER

void pixfont::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
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
    SDL_RenderCopy(graphics::render, texture, &srcrect, &sprite);
}
#endif

bool pixfont::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me out https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}


//ALSO STOLEN FROM BACKGROUND.H LMAOOOO

//i still dont understand how this works, and that's terrifying because i entirely wrote this myself

//update: stealing from background.h is morally incorrect so i decided instead to recode this since pixfonts can literally only have one texture

//ADD MORE TEXTURES TO A pixfont AND I'LL BREAK YOU LEGS


//now that i mention it, adding more than one texture to a pixfont could be cool? but i'm not really designing an rpg text box, am i?

//...or am i?


void  pixfont::generateSurfaces(std::string path) {
    std::string tmppth = path + "/font.bmp";
    #ifdef __LEGACY_RENDER
    SDL_Surface* tmpsurf = SDL_LoadBMP(tmppth.c_str());
    txt = SDL_CreateTextureFromSurface(graphics::render, tmpsurf);
    SDL_FreeSurface(tmpsurf);
    #else
    #endif
}
color pixfont::tintColor(const color& baseColor, const color& tintColor) {
    float tintAmount = 0.5; // Adjust this value to control the tinting amount (0.0 to 1.0)
    
    int tintedRed = (baseColor.r + (tintColor.r - baseColor.r) * tintAmount);
    int tintedGreen = (baseColor.g + (tintColor.g - baseColor.g) * tintAmount);
    int tintedBlue = (baseColor.b + (tintColor.b - baseColor.b) * tintAmount);

    return color(tintedRed, tintedGreen, tintedBlue);
}

std::vector<std::string> pixfont::seperateWords(std::string string) {
    std::vector<std::string> parts;
    int startindex = 0;
    while(true) {
        int index = string.find_first_of(' ', startindex);
        if(index == -1) {
            parts.push_back(string.substr(startindex));
            return parts;
        }

        std::string word = string.substr(startindex,index-startindex);
        char nextchar = string.substr(index,1).at(0);
        if(nextchar = ' ') {

            parts.push_back(word);
            parts.push_back(std::string(1, nextchar));
        }
        startindex = index + 1;
    }
}

std::string pixfont::wrap(std::string str, int pixels) {
    
    std::vector<std::string> words = seperateWords(str);
    int currentline = 0;
    std::string output = "";
    
    for(int i = 0; i < words.size(); i++) {
        std::string word = words.at(i);
        int length = 0;
        bool next = false;
        for(int x = 0; x < word.length();x++) {
            if(word.at(x) == '@') {
                next = true;
                continue;
            }
            else if(next) {
                next = false;
                continue;
            }
            else {
                length++;
            }

        }
        if(currentline + length > pixels) {
            if(currentline > 0) {
                output += "\n";
                currentline = 0;
            }
            /*while(word.length() > pixels) {
                output += word.substr(0, pixels-1) + ".";
                word = word.substr(pixels-1);
                output += "\n";
            }
            word = word.substr(word.find_first_of(' ')+1);
            */
        }
        output += word;
        currentline += word.length();
    }
    
    return output;
}
std::vector<std::string> pixfont::split(const std::string& input, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    
    for (char c : input) {
        if (c == delimiter) {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        result.push_back(token);
    }
    
    return result;
}

