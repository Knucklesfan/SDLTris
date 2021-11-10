#include "replay.h"
#include <algorithm>    // std::sort
#include <cstring>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <SDL2/SDL.h>

replay::replay() {
    //init everything
    record = false;
    playback = false;

}

SDL_Keycode replay::logic(long long tick) {
    //TODO: This code is absolutely STICKY.

    //(that's fake slang i just came up with which stands for jesus christ this is an awful, memory wasting, ram leaking piece of crap idea)

    rapidxml::file<> xmlFile(path.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    if(playback) {
        std::string data = "k" + std::to_string(tick);
        if (doc.first_node(data.c_str()) != NULL) {
            std::cout << data << "\n";
            return (SDL_Keycode) atoi(doc.first_node(data.c_str())->value());
        }
    }
    return SDLK_UNKNOWN;
}

SDL_Keycode replay::logic(SDL_Keycode key, long long tick) {
    if(outfile && record) {
            outfile << "<k" << tick << ">" << key << "</k" << tick << ">\n";
    }
    return 0;

}

int replay::recordreplay(std::string path) {
	outfile = std::ofstream(path);
    unsigned int seed = (unsigned)time(0);
    //srand(1);
    srand(seed);
    outfile << "<seed>" << seed <<"</seed>\n";
    record = true;
    playback = false;
    return 0;
}

int replay::loadreplay(std::string path) {
    rapidxml::file<> xmlFile(path.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    unsigned int seed = 0;
    if (doc.first_node("seed") != NULL) {
        seed = atoi(doc.first_node("seed")->value());
    }
    //srand(1);
    srand(seed);
    playback = true;
    replay::path = path;
    return 2;
}

void replay::endlogic(bool gameactive) {
    if(!gameactive) {
        outfile.close();
        record = false;
        playback = false;
    }
}
