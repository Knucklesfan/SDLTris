#include "utils.h"
#include "rapidxml_utils.hpp"

//#include "pixfont.h"
#ifdef __SWITCH__
#define filepath "/"
#include <switch.h>

#else
#define filepath "./"
#endif
std::vector<texture *> graphics::textures = std::vector<texture *>();
std::vector<shader *> graphics::shaders = std::vector<shader *>();
#ifdef CLIENT
std::vector<bg *> graphics::backgrounds = std::vector<bg *>();
#endif
//std::vector<Font *> graphics::fonts = std::vector<Font *>();

std::map<std::string, actiontype> bgconverters::actionmap =
    {
        {"move", actiontype::MOVE},
        {"scale", actiontype::SCALE},
        {"rotate", actiontype::ROTATE}};
std::map<std::string, layertype> bgconverters::layermap = {
    {"background", layertype::BACKGROUND}, //
    {"legacy", layertype::LEGACY},
    {"bg3d", layertype::BG3D},
    {"bg2d", layertype::BG2D}};
;
std::map<std::string, headerdata> bgconverters::headermap =
    {
        {"title", headerdata::TITLE},
        {"version", headerdata::VERSION},
        {"music", headerdata::MUSIC},
        {"filename", headerdata::FILENAME},
        {"creator", headerdata::CREATOR}

};

std::string utils::loadFile(std::string filename)
{
    std::ifstream inFile;
    inFile.open(filename); // open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); // read the file
    return strStream.str();      // str holds the content of the file
}
float utils::mean(float arr[], int from, int to)
{
    float ret = 0.0;
    for (int i = from; i < to; i++)
    {
        ret += arr[i];
    }
    return ret / to - from;
}
int utils::unsign(int a)
{
    if (a < 0)
    {
        return 0;
    }
    return a;
}

bool checkNode(rapidxml::xml_node<char> *node)
{
    if (node == nullptr)
    {
        std::cout << "Failed to parse node\n";
    }
    return node != nullptr;
}
#ifdef CLIENT
int graphics::generatebgs()
{
    rapidxml::file<> bgFile((filepath"backgrounds/backgrounds.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char> *parent = bgDoc.first_node("backgrounds");
    for (rapidxml::xml_node<char> *child = parent->first_node(); child != NULL; child = child->next_sibling())
    {

        std::cout << "loading background " << child->value() << "\n";
        // std::cout << "HELP ME:" << p.path().filename() << "\n";
        if (child->value() != "")
        {
            bg *backg = new bg(child->value(), false);
            backgrounds.push_back(backg);
        }
    }
    return 0;
}

#endif
