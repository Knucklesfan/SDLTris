#pragma once
#include <vector>
#include <string>
#include "../opengl/texture.h"
#include <cstdint>

enum TagQuality {
    GOOD,BAD,UGLY
};

enum SCORETYPE {
	GRAVITY, //score added via gravity
	LINECLEAR, //score earned through clearing lines
	FORCEDROP, //score earned through performing forcedrops
	PASSIVE, //score earned passively, through modifiers
	GARBAGE //score earned through clearing a garbage modifier's garbage
};

enum SCOREOP {
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
    FILLUP_SPEED, //how fast it fills up once score starts being applied
    DRAIN_SPEED, //how fast it drains once score has filled up
    THRESHOLD //the threshold required for combo to be considered "full" (default 8x)
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
    blockChances operator+= (blockChances value) const //THIS IS INSANE THAT C++ CAN DO THIS
        {
            return {square+value.square,line+value.line,zpiece+value.zpiece,
            spiece+value.spiece,lpiece+value.lpiece,ipiece+value.ipiece,
            tpiece+value.tpiece};
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
class modifier {
    public:
        modifierMeta metadata;
        std::vector<ScoreOperation> scoreOperations;
        ScoreOperation gravitySpeed; //this works effectively the same as a scoreop, just like.. not
        std::vector<ComboOperation> comboOperations;
        blockChances chances;
        modifier(std::string filename); //loads a mod into memory
};
