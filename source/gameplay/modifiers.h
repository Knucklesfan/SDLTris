#pragma once
#include <vector>
#include <string>
#include "../opengl/texture.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_keycode.h"
#include <cstdint>
#include <map>
enum TagQuality {
    GOOD,BAD,UGLY
};

enum SCORETYPE {
    GRAVITY,   // score added via gravity
    LINECLEAR, // score earned through clearing lines
    FORCEDROP, // score earned through performing forcedrops
    PASSIVE,   // score earned passively, through modifiers
    GARBAGE,   // score earned through clearing a garbage modifier's garbage
    LINE_SCORE_1, //each line combo
    LINE_SCORE_2, //represents one
    LINE_SCORE_3, //type of lineclear
    LINE_SCORE_4, //and its granted combo
};

enum SCOREOP {
    NOFUN,
	MUL,
	DIV,
	ADD,
	SUB,
	MULPERC,
	ADDPERC,
	SUBPERC,
    SET //do not use on score! (i mean, unless you want to)
};
enum COMBOOP {
    FILLUP_SPEED, // how fast it fills up once score starts being applied
    DRAIN_SPEED,  // how fast it drains once score has filled up
    THRESHOLD,    // the threshold required for combo to be considered "full"
                  // (default 8x)
    LINE_COMBO_1, //each line combo
    LINE_COMBO_2, //represents one
    LINE_COMBO_3, //type of lineclear
    LINE_COMBO_4, //and its granted combo

};
struct ScoreOperation {
	double value;
	SCOREOP operation;
	int times;	
};
struct ComboOperation {
	double value;
	SCOREOP operation;
	COMBOOP slot;	
};
struct blockChances {
    float square;
    float line;
    float zpiece;
    float spiece;
    float lpiece;
    float ipiece;
    float tpiece;
    bool set=false;
    blockChances operator+= (blockChances value) const //THIS IS INSANE THAT C++ CAN DO THIS
        {
            return {square+value.square,line+value.line,zpiece+value.zpiece,
            spiece+value.spiece,lpiece+value.lpiece,ipiece+value.ipiece,
            tpiece+value.tpiece};
        }
    blockChances operator*= (blockChances value) const //THIS IS INSANE THAT C++ CAN DO THIS
        {
            return {square*value.square,line*value.line,zpiece*value.zpiece,
            spiece*value.spiece,lpiece*value.lpiece,ipiece*value.ipiece,
            tpiece*value.tpiece};
        }

};
struct modifierTag {
    std::string tag;
    TagQuality quality;
};
struct modifierMeta {
    std::string name;
    std::string desc;
    uint32_t price;
    bool forSale;
    texture* tex;
    std::vector<modifierTag> tags;
};
enum OPTYPES { SCOREOPTYPE, GRAVITOPTYPE, COMBOOPTYPE, CHANCESOPTYPE, HARDCODEOPTYPE, SETTINGOPTYPE };
enum QUIRKTYPES {
  SHOTGUN,
  GRAVITYDROP,
  MULTIHOLD,
  HOLDBOOK, // hold book is a really funny idea i had that we give the player
            // "1000 hold slots",
            //  but they have to go through a list manually to pull it out, and
            //  you lose access to the default hold box
};
enum QUIRKPERMS { // quirks have permissions to hijack certain functions, but it
                  // very much depends on what you're trying to do here.
  QUIRKNONE,
  QUIRKINPUT,
  QUIRKLOGIC,
  QUIRKENDLOGIC, // SHOULD NEVER BE USED
  QUIRKRENDER,
  
};
class modifierUtils {
    public:
        static float performModOperation(float original, SCOREOP operation, float value, float max = 0.0);
        static std::map<std::string, SCOREOP> operations;
        static std::map<std::string, COMBOOP> combos;
        static std::map<std::string, OPTYPES> types;
        static std::map<std::string, QUIRKTYPES> quirksmap;
        static std::map<std::string, SCORETYPE> scoremap;

};
class game;
class Quirk { // quirks are hardcoded functions that get called in specific
              // spots. Technically, Lua will be a quirk when implemented.
public:
     // quirk functions also equally have return types of bools, if a function
      // returns a bool, then it cancels out the real function being overriden

    
        virtual bool input(game* game, SDL_Keycode key) {return false;};
        virtual bool logic(game *game, double deltaTime) {return false;};
        virtual bool render(game *game) {return false;};
        virtual int endLogic(game *game) { return false; };
        virtual QUIRKTYPES getType() { return QUIRKTYPES::GRAVITYDROP; };
        virtual int getPermission() {return 0;};

};
class shotgunQuirk : public Quirk {
  private:
    int maxiters = 0;
    int odds = 0;
    int iterations = 0;

  public:
    QUIRKTYPES getType() { return QUIRKTYPES::SHOTGUN; }; //THIS IS SO DUMB
    int getPermission() {return QUIRKPERMS::QUIRKINPUT;};

    bool input(game* game, SDL_Keycode key);
    
    shotgunQuirk(int max, int odd) {
        maxiters = max;
        odds = odd;
    }
};
class modifier {
    public:
        modifierMeta metadata;
        std::vector<ScoreOperation> scoreOperations;
        ScoreOperation gravitySpeed; //this works effectively the same as a scoreop, just like.. not
        std::vector<ComboOperation> comboOperations;
        std::vector<Quirk*> quirks;
        blockChances chances;
        modifier(std::string filename); //loads a mod into memory
};
