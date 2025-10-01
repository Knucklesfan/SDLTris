#include "modifiers.h"
#include "../utils/defs.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

modifier::modifier(std::string path) {
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

    // for (rapidxml::xml_node<char>* tagChild = modParent->first_node("meta")->first_node("tags")->first_node(); tagChild != NULL; tagChild = tagChild->next_sibling()) {
    //     std::string name = tagChild->name();
    //     if(name == "good") {
    //         std::cout << "good\n";
    //         modTags.push_back({tagChild->value(),TagQuality::GOOD});
    //     }
    //     else if(name == "bad") {
    //         std::cout << "bad\n";
    //         modTags.push_back({tagChild->value(),TagQuality::BAD});
    //     }
    //     else {
    //         std::cout << "ugly\n";
    //         modTags.push_back({tagChild->value(),TagQuality::UGLY});
    //     }

    // }
    metadata = meta;

}