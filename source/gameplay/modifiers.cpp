#include "modifiers.h"
#include "../utils/defs.h"

#include <exception>
#include <iostream>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "../scenes/game.h"
modifier::modifier(std::string path) {
  std::cout << "Loading modifier " << path << "\n";
    rapidxml::file<> modFile((filepath"modifiers/"+path+"/modifier.xml").c_str());
    rapidxml::xml_document<> modDoc;
    modDoc.parse<0>(modFile.data());

    rapidxml::xml_node<char>* modParent = modDoc.first_node("modifier");

    std::string modName = modParent->first_node("meta")->first_node("name")->value();
    std::string modDesc = modParent->first_node("meta")->first_node("description")->value();
    Uint32 modPrice = atoi(modParent->first_node("meta")->first_node("price")->value());

    texture* modTex = new texture(filepath"modifiers/"+path+"/"+modParent->first_node("meta")->first_node("sprite")->value());
    bool modSale = std::string(modParent->first_node("meta")->first_node("forsale")->value()) == "true";
    
    std::vector<modifierTag> modTags;
    modifierTag tag;
    for (rapidxml::xml_node<char>* tagChild = modParent->first_node("meta")->first_node("tags")->first_node(); tagChild != NULL; tagChild = tagChild->next_sibling()) {
        std::string name = tagChild->name();
        if(name == "good") {
            std::cout << "good\n";
            modTags.push_back({tagChild->value(),TagQuality::GOOD});
        }
        else if(name == "bad") {
            std::cout << "bad\n";
            modTags.push_back({tagChild->value(),TagQuality::BAD});
        }
        else {
            std::cout << "ugly\n";
            modTags.push_back({tagChild->value(),TagQuality::UGLY});
        }

    }
    modifierMeta meta = {modName,modDesc,modPrice,modSale,modTex,modTags};

    for (rapidxml::xml_node<char>* tagChild = modParent->first_node("effects")->first_node(); tagChild != NULL; tagChild = tagChild->next_sibling()) {
        std::string name = tagChild->name();
        if (name == "trigger") {
          
            std::string value = tagChild->first_attribute("type")->value();
            std::string func = tagChild->first_attribute("func")->value();
            std::string type = tagChild->first_attribute("mods")->value();
            std::cout << "trigger " << value << " "  << func << " " << type << "\n";

            SCOREOP operation;
            if (value == "value") {
                operation = modifierUtils::operations.at(func);
            }
            OPTYPES operationtype = modifierUtils::types.at(type);
            switch (operationtype) {
            case OPTYPES::COMBOOPTYPE: {
                    COMBOOP key = modifierUtils::combos.at(tagChild->first_node("key")->value());
                    double value = atof((tagChild->first_node("value")->value()));
                    
                    comboOperations.push_back({value,operation,key});
                } break;
                case OPTYPES::CHANCESOPTYPE: {

                }break;
                case OPTYPES::GRAVITOPTYPE: {
                    double value = atof((tagChild->first_node("value")->value()));

                    gravitySpeed = {value,operation,0};

                }break;
                case OPTYPES::SCOREOPTYPE: {
                    SCORETYPE key = modifierUtils::scoremap.at(tagChild->first_node("key")->value());
                    double value = atof((tagChild->first_node("value")->value()));
                    
                    scoreOperations.push_back({value,operation,key});

                } break;
                case OPTYPES::HARDCODEOPTYPE: {
                        QUIRKTYPES key = modifierUtils::quirksmap.at(func);
                        switch (key) {
                        case QUIRKTYPES::SHOTGUN: {
                                std::cout << "loading shotgun quirk\n";
                                int odds = atof((tagChild->first_node("odds")->value()));
                                int iters = atof(
                                    (tagChild->first_node("iters")->value()));
                                shotgunQuirk* tmp = new shotgunQuirk(iters,odds);
                                quirks.push_back(tmp);
                            }break;
                            default: {
                                std::cout << "Unimplemented quirk! \""+func+"\"\n";

                            }break;
                        }

                }break;
                case OPTYPES::SETTINGOPTYPE: {

                
                }break;
            }
        }
    }
    std::cout << "quirks size: " << quirks.size() << "\n";

    metadata = meta;
}
float modifierUtils::performModOperation(float original, SCOREOP operation, float value, float max) {
    float newvalue = original;
    switch (operation) {
        case MUL: {
            newvalue *= value;
        } break;
        case DIV: {
            newvalue /= value;
        }break;
        case ADD: {
            newvalue += value;
        } break;
        case SUB: {
            newvalue -= value;
        }break;
        case MULPERC: {
            newvalue *= value/100.0f;
        } break;
        case ADDPERC: {
            newvalue += value/100.0f;

        }break;
        case SUBPERC: {
            newvalue -= value/100.0f;

        } break;
        case SET: {
            newvalue = value;
        }break;
        }
        return newvalue;
}

std::map<std::string, SCOREOP> modifierUtils::operations = {
    {"add", SCOREOP::ADD},
    {"addperc", SCOREOP::ADDPERC},
    {"div", SCOREOP::DIV},
    {"mul", SCOREOP::MUL},
    {"mulperc", SCOREOP::MULPERC},
    {"subperc", SCOREOP::SUBPERC},
    {"sub", SCOREOP::SUB},
    {"set", SCOREOP::SET},
    {"none", SCOREOP::NOFUN}
};
std::map<std::string, SCORETYPE> modifierUtils::scoremap = {
    {"forcedrop", SCORETYPE::FORCEDROP},
    {"garbage", SCORETYPE::GARBAGE},
    {"gravity", SCORETYPE::GRAVITY},
    {"line_score_1", SCORETYPE::LINE_SCORE_1},
    {"line_score_2", SCORETYPE::LINE_SCORE_2},
    {"line_score_3", SCORETYPE::LINE_SCORE_3},
    {"line_score_4", SCORETYPE::LINE_SCORE_4},
    {"passive", SCORETYPE::PASSIVE},
    {"lineclear", SCORETYPE::LINECLEAR},

};

std::map<std::string, COMBOOP> modifierUtils::combos = {
    {"fillup_speed", COMBOOP::FILLUP_SPEED},
    {"drain_speed", COMBOOP::DRAIN_SPEED},
    {"threshold", COMBOOP::THRESHOLD},

    {"line_combo_1", COMBOOP::LINE_COMBO_1},
    {"line_combo_2", COMBOOP::LINE_COMBO_2},
    {"line_combo_3", COMBOOP::LINE_COMBO_3},
    {"line_combo_4", COMBOOP::LINE_COMBO_4},

};
std::map<std::string, OPTYPES> modifierUtils::types = {
    {"chance", OPTYPES::CHANCESOPTYPE},
    {"combo", OPTYPES::COMBOOPTYPE},
    {"gravity", OPTYPES::GRAVITOPTYPE},
    {"score", OPTYPES::SCOREOPTYPE},
    {"hardcoded", OPTYPES::HARDCODEOPTYPE},
    {"settings", OPTYPES::SETTINGOPTYPE},

};
std::map<std::string, QUIRKTYPES> modifierUtils::quirksmap = {
    {"shotgun", QUIRKTYPES::SHOTGUN},
    {"gravitydrop", QUIRKTYPES::GRAVITYDROP},
    {"multihold", QUIRKTYPES::MULTIHOLD},
    {"holdbook", QUIRKTYPES::HOLDBOOK},

};
bool shotgunQuirk::input(game *game, SDL_Keycode key) {
  std::cout<< "hijacked input " << odds << " "<< maxiters<< " " << iterations << "\n";
    if (key == SDLK_z) {
        iterations++;
        
        if ((rand() % odds) == 0) {
            game->t.forcedrop();
            iterations = 0;
            return true;
            }
        }
    return false;
}
