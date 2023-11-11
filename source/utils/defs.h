#pragma once

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define INTERNAL_WIDTH 640
#define INTERNAL_HEIGHT 480
#define COORDINATE_WIDTH INTERNAL_WIDTH
#define COORDINATE_HEIGHT INTERNAL_HEIGHT

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
#ifndef __LEGACY_RENDER
    #include "../opengl/shader.h"
    #include "../opengl/texture.h"
    #include "../opengl/sprite.h"
    #include "../opengl/rect.h"

#endif
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


enum layertype {
    BACKGROUND,
    LEGACY,
    BG3D,
    BG2D,
    SHADER
};
enum headerdata {
    TITLE,
    VERSION,
    CREATOR,
    BGMUSIC,
    FILENAME
};

enum OPTIONTYPE
{
	GAMEPLAY = 0,
	DISPLAY = 1,
	SYSTEM = 2,
	EXTRA = 3,
	DEBUG = 4
};

enum GAMEPLAYOPTIONS
{
	GHOSTPIECE = 0,
	HOLDPIECE = 1,
	BLOCKSPEED = 2,
	FASTDROP = 3,
	SCORING = 4,
    REPEATHOLD = 5,
    HOLDSCORING = 6,
    LEVELLENGTH = 7
};

enum DISPLAYOPTIONS
{
	BGMODE = 0,
	FIRSTBG = 1,
	LINECLEAR = 2,
	MOVINGBG = 3,
	NEARTOPFLASH = 4,
    LOWPERF = 5
};
enum SYSTEMOPTIONS
{
	FULLSCREEN = 0,
	MUSIC = 1,
	SOUNDS = 2,
	RESET = 3,
};

enum EXTRAOPTIONS
{
	ROTATEBOARD = 0,
	BIGGERBOARD = 1,
	BLINDMODE = 2,
    ANTIGRAVITY = 3,
    BOMB,
    MYSTERYBLOCK,
    MIRROR,
    ACIDBATH

};

enum DEBUGOPTIONS {
	DEBUGENABLED = 0
};
class bgconverters { //i *sighs the worlds deepest sigh of human history* love background.h
    public:
        static std::map<std::string, layertype> layermap;
        static std::map<std::string, headerdata> headermap;
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
    static SDL_Window* window;

    #ifdef __LEGACY_RENDER
        static SDL_Renderer* render;
        static std::map<std::string,SDL_Texture*> sprites;
        static std::vector<SDL_Texture*>* blocks;
    #else
        static std::vector<shader*> shaders;
        static std::map<std::string,texture*> sprites;
        static std::vector<texture*>* blocks;
        static spriteRenderer* sprite;
        static rectRenderer* rect;
    #endif
        static std::vector<bg>* backgrounds;
        static std::vector<Font*>* fonts;
        const static Uint8 *state;
        static double deltaTime;
     #ifdef __LEGACY_RENDER
        static void drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
	    static void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, bool center, int srcx, int srcy, int srcw, int srch, int scalex, int scaley);
    #endif
        static int generatefonts();
        static int generatebgs();
        static int generatesprites();
        #ifndef __LEGACY_RENDER
            static int generateshaders();
            static int generatetextures();
        #endif

    //uhhh put other static stuff here.
};
class settings {
    public:
    static int previousscore;
    static int maxscore;
    //...updates the high score
    //@param prev the Previous score
    static void updateHighScore(int prev); 

    static void loadSettings();
	static std::array<std::array<int, 12>, 5> defaults;

	static std::array<std::array<int, 12>, 5> activations;
};
namespace utils {
	double lerp(double a, double b, double t);
	#ifdef __LEGACY_RENDER
    SDL_Texture* getSDLTexture(std::string path, SDL_Renderer* renderer);
    #else
    #endif
    std::string loadFile(std::string filename);
    static unsigned int renderFB;
    static unsigned int renderTexture;
    bool checkNode(rapidxml::xml_node<char>* node);

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
//File full of a bunch of really simple and easy things to free up memory
namespace memory {
    void freeSprites();
    void freeShaders();
    void freeBackgrounds();
    void freeFonts();
}