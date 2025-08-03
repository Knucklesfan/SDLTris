#pragma once

#include "SDL2/SDL_stdinc.h"
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
#include <string>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "../opengl/background.h"
#include "../engine/gamemode.h"
#include "../gameplay/modifiers.h"
#ifndef __LEGACY_RENDER
    #include "../opengl/shader.h"
    #include "../opengl/texture.h"
    #include "../opengl/sprite.h"
    #include "../opengl/rect.h"
    #include "../opengl/line.h"

#endif
//#include "Object.h"
#include "../opengl/font.h"
#include "../engine/rpcimplement.h"

#include <map>
#include "enums.h"
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
        static void playSound(int index); //returns 1 if sound played, otherwise returns 0
        static void playMusic(int index); //returns 1 if music played, otherwise returns 0
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
        static std::vector<unsigned int>* cubemaps;
        static spriteRenderer* sprite;
        static rectRenderer* rect;
        static lineRenderer* line;
        static buffermanager* globalbuffer;
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
            static void generatecubemaps();
            static void screenshot(); //takes a screenshot and saves it to the running folder
        #endif

    //uhhh put other static stuff here.
};
struct save {
    std::string name;
    int level;
    int lines;
    int hold;
    int piece;
    Uint32 score;
    texture* t;
};

class settings {
    public:
    static int previousscore;
    static int maxscore;
    static int lastlines;
    static int lastlevel;
    static int lasttime;
    static bool usePreciseTiming; //if enabled, this will make the user use well... precise timing (aka every frame is 60fps, rather than dynamic deltatime)
    //...updates the high score
    //@param prev the Previous score
    static void updateHighScore(int prev); 
    static std::vector<std::string> demos;
    static void loadSettings();
    static void loadDemos();
    static void loadSaveData();
    static void clearSaveData();
    static std::string saveload;
    static std::string configDir;
    static std::string saveDir;
    static bool globalDebug;


    static std::vector<save> saveCache;
	static std::array<std::array<int, 12>, 5> defaults;
	static std::array<std::array<int, 12>, 5> activations; 

};
namespace utils {
    std::string getenv( const std::string & var );
	double lerp(double a, double b, double t);
	#ifdef __LEGACY_RENDER
    SDL_Texture* getSDLTexture(std::string path, SDL_Renderer* renderer);
    #else
    #endif
    std::string loadFile(std::string filename);
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
    // vect rotate_to_point(vect object_position, vect point);
    double rad(double i);

};
struct gameplayActivations {
    Uint64 startingLevel;
    Uint64 stage; //theres no way im making INT64_MAX of these but whatever covering bases
    Uint8 difficulty;
    std::array<Uint64,16> activeMods; 
    //this is quite literally a ridiculous amount of modifiers anyways,
    //nobody needs 1024 modifiers, but just covering bases since memory is CHEAP
    Uint64 texturepackIndex;
    bool playAnimations;
    bool noSpeedup;
    bool noGravity;
    std::string seed;
};
class gameplay { //class that contains stuff that globally is used for managing gamemodes
    public:
        static bool transitioning;
        static int gamemode;
        static std::vector<Gamemode*> gamemodes;
        static void loadGamemodes();
        static void loadModifiers();
        static std::vector<modifier> modifiers;
        static int Pieces[7][4][16];
	    static gameplayActivations activations; //kinda like activations, but different
        static void resetActivations();
};
class networking { //class that contains global network objects, such as server and rpc connections
    public:
        static rpcimplement* globalRPC;
};
//File full of a bunch of really simple and easy things to free up memory
namespace memory {
    void freeSprites();
    void freeShaders();
    void freeBackgrounds();
    void freeFonts();
}
class math {
    public:
        static int numActive(Uint64* x);
        static double easeOutBounce(double x);
        static float easeInOutCubic(float x);
};