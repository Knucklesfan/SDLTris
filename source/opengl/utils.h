#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
#include <vector>
#include "shader.h"
#include "texture.h"
#ifdef CLIENT
#include "background.h"
#endif
#include <map>
#include <rapidxml.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"

#define INTERNAL_WIDTH 640
#define INTERNAL_HEIGHT 480
#define COORDINATE_WIDTH INTERNAL_WIDTH
#define COORDINATE_HEIGHT INTERNAL_HEIGHT

namespace utils {
    std::string loadFile(std::string filename);
    static unsigned int renderFB;
    static unsigned int renderTexture;

    static double deltaTime;
	double lerp(double a, double b, double t);
    int sign(int);
    int unsign(int);
    float mean(float arr[], int from, int to);        
    bool checkNode(rapidxml::xml_node<char>* node);

}
class graphics {
    public:
        static std::vector<shader*> shaders;
        static std::vector<texture*> textures;
#ifdef CLIENT
        static std::vector<bg*> backgrounds;
#endif
        //static std::vector<Font*> fonts;
        static int generateshaders();
        static int generatetextures();
        //static int generatefonts();
#ifdef CLIENT
        static int generatebgs();
#endif
};


enum actiontype {
    MOVE,
    SCALE,
    ROTATE
};
enum layertype {
    BACKGROUND,
    LEGACY,
    BG3D,
    BG2D
};
enum headerdata {
    TITLE,
    VERSION,
    CREATOR,
    MUSIC,
    FILENAME
};

class bgconverters { //i *sighs the worlds deepest sigh of human history* love background.h
    public:
        static std::map<std::string, actiontype> actionmap;
        static std::map<std::string, layertype> layermap;
        static std::map<std::string, headerdata> headermap;
};

 //these classes originate from KnuxfanPong, but I found them useful here.
struct letter {
    char character = ' ';
    int width;
    int x;
    int y;
};

struct color { //technically didnt need this, but included it anyways because SDL_color is jank (and useless in opengl)
public:
    color() {};
    color(int red, int green, int blue) { r = red; g = green; b = blue; }
    int r, g, b;

};
#endif
