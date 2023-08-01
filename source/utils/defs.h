#pragma once

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#ifdef __SWITCH__
    #define filepath  "/"
    #include <switch.h>

#else
    #define filepath  "./"
#endif

#define JOY_A     0
#define JOY_B     1
#define JOY_X     2
#define JOY_Y     3
#define JOY_PLUS  10
#define JOY_L  6

#define JOY_LEFT  12
#define JOY_UP    13
#define JOY_RIGHT 14
#define JOY_DOWN  15

#define TICK_INTERVAL    7

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "../background.h"
//#include "Object.h"
#include "../font.h"

#include <map>
struct letter {
    char character = ' ';
    int width;
    int x;
    int y;
};

struct color { //technically didnt need this, but included it anyways because SDL_color is jank
public:
    color() {};
    color(int red, int green, int blue) { r = red; g = green; b = blue; }
    int r, g, b;

};
struct edge {
    int a,b;
    color c;
    edge(int x,int y, color z) {
        a=x;
        b=y;
        c = z;
    }
};
struct vect {
    float x,y,z;
    vect(float a, float b, float c) {
        x=a;
        y=b;
        z=c;
    }
};
class audio {
    public:
        static void generatemusic();
        static void generatesound();

        static std::vector<Mix_Music*>* music;
        static std::vector<Mix_Chunk*>* sfx;
        static int playSound(int index); //returns 1 if sound played, otherwise returns 0
        static int playMusic(int index); //returns 1 if music played, otherwise returns 0
};
class graphics {
    public:
        static SDL_Renderer* render;
        static SDL_Window* window;
        static std::vector<bg>* backgrounds;
        static std::map<std::string,SDL_Texture*> sprites;
        static std::vector<Font*>* fonts;
        const static Uint8 *state;
        static double deltaTime;
        static void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
        // static int generateobjects();
        static int generatefonts();
        static int generatebgs();
        static int generatesprites();
        static std::vector<SDL_Texture*>* blocks;
    //uhhh put other static stuff here.
};
namespace utils {
	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, bool center, int srcx, int srcy, int srcw, int srch, int scalex, int scaley);
	double lerp(double a, double b, double t);
	SDL_Texture* getSDLTexture(std::string path, SDL_Renderer* renderer);
    int sign(int);
    int unsign(int);
    double deg(double value);
    int getMouseX();
    int getMouseY();
    int mouseCheck(int);
    int keyboardCheck(int);
    float mean(float arr[], int from, int to);
    std::string wrap(std::string str, int pixels);
    std::vector<std::string> seperateWords(std::string string, char sep);
    std::vector<std::string> seperateWords(std::string string, char sep, int);
    vect rotate_to_point(vect object_position, vect point);

    double rad(double i);

};
