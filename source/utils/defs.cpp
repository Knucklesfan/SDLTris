#include "defs.h"
#ifdef __SWITCH__
    #define filepath  "/"
    #include <switch.h>

#else
    #define filepath  "./"
#endif
#include "../pixfont.h"
#include "../ttffont.h"
#include <cmath>
SDL_Renderer* graphics::render = nullptr;
SDL_Window* graphics::window = nullptr;
double graphics::deltaTime = 0;
std::vector<bg>* graphics::backgrounds = new std::vector<bg>();
std::map<std::string,SDL_Texture*> graphics::sprites = std::map<std::string,SDL_Texture*>();
// std::vector<ObjectTemplate>* graphics::objects = new std::vector<ObjectTemplate>();
std::vector<Font*>* graphics::fonts = new std::vector<Font*>();
std::vector<Mix_Music*>* audio::music = new std::vector<Mix_Music*>();
std::vector<Mix_Chunk*>* audio::sfx = new std::vector<Mix_Chunk*>();
std::vector<SDL_Texture*>* graphics::blocks = new std::vector<SDL_Texture*>();


const Uint8 *graphics::state = SDL_GetKeyboardState(nullptr);
SDL_Texture* utils::getSDLTexture(std::string path, SDL_Renderer* renderer) {
        SDL_Surface* surf = IMG_Load(path.c_str());
        if (!surf) {
            printf("Failed to load image at %s: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_Texture* ret = SDL_CreateTextureFromSurface(renderer, surf);
        if (!ret) {
            printf("Failed to generate texture at %s: %s\n", path.c_str(), SDL_GetError());
        }

        SDL_FreeSurface(surf);
        return ret;
}
int graphics::generatefonts() {
    rapidxml::file<> bgFile((filepath"fonts/fonts.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("fonts");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != nullptr; child = child->next_sibling()) {

        std::cout << "loading font " << child->value() << "\n";
        //std::cout << "HELP ME:" << p.path().filename() << "\n";
        std::string type = child->first_node("type")->value();
        Font* f;
        if (type == "ttffont") {
            f = new ttffont(child->first_node("path")->value(), atoi(child->first_node("size")->value()));
        }
        else {
            f = new pixfont(child->first_node("path")->value());
        }
        fonts->push_back(f);
    }
    return 0;

}
int graphics::generatebgs() {
   rapidxml::file<> bgFile((filepath"backgrounds/backgrounds.xml"));
   rapidxml::xml_document<> bgDoc;
   bgDoc.parse<0>(bgFile.data());
   rapidxml::xml_node<char>* parent = bgDoc.first_node("backgrounds");
   for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

       std::cout << "loading background " << child->value() << "\n";
       //std::cout << "HELP ME:" << p.path().filename() << "\n";
       if(child->value() != "") {
           bg backg(child->value(), false, render);
           backgrounds->push_back(backg);
       }

   }
   return 0;

}

// int graphics::generateobjects() {
//     rapidxml::file<> bgFile((filepath"objects/objects.xml"));
//     rapidxml::xml_document<> bgDoc;
//     bgDoc.parse<0>(bgFile.data());
//     rapidxml::xml_node<char>* parent = bgDoc.first_node("objects");
//     for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

//         std::cout << "loading sprite " << child->first_node("path")->value() << "\n";
//         //std::cout << "HELP ME:" << p.path().filename() << "\n";
//         objects->push_back({child->first_node("path")->value()});
//     }
//     return 0;

// }
int audio::playMusic(int index) {
    Mix_HaltMusic();
    if( Mix_PlayingMusic() == 0 )
    {
        //Play the music
        Mix_PlayMusic( music->at(index), -1 );
    }
    //If music is being played
    else
    {
        //If the music is paused
        if( Mix_PausedMusic() == 1 )
        {
            //Resume the music
        Mix_ResumeMusic();
        }
        //If the music is playing
        else
        {
        //Pause the music
            Mix_PauseMusic();
        }
    }

}
int audio::playSound(int index) {
    Mix_PlayChannel( -1, sfx->at(index), 0 );
}
void audio::generatemusic() {
    // rapidxml::file<> musFile((path+"/music.xml").c_str());
    // rapidxml::xml_document<> musDoc;
    // musDoc.parse<0>(musFile.data());
    // Mix_Music* temp;
    // int musics = atoi(musDoc.first_node("songs")->value());
    // for(int i = 0; i < musics; i++) {
    //         std::string tmp = "mus" + std::to_string(i);
    //         std::cout << tmp << "\n";
    //         std::string musPath = path + "/" + musDoc.first_node(tmp.c_str())->value();
    //         temp = Mix_LoadMUS(musPath.c_str());
    //         if (!temp) {
    //             printf("Failed to load music at %s: %s\n", musPath, SDL_GetError());
    //         }
    //         audio::music->push_back(temp);
    //         printf("Successfully loaded music at %s\n", musPath.c_str());

    // }
    rapidxml::file<> bgFile((filepath"music/music.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("music");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

        std::cout << "loading music " << child->first_node("path")->value() << "\n";
        //std::cout << "HELP ME:" << p.path().filename() << "\n";        
        music->push_back(Mix_LoadMUS(child->first_node("path")->value()));
    }

}


void audio::generatesound() {
    // rapidxml::file<> musFile((path+"/sound.xml").c_str());
    // rapidxml::xml_document<> musDoc;
    // musDoc.parse<0>(musFile.data());
    // std::vector<Mix_Chunk*> music;
    // Mix_Chunk* temp;
    // int musics = atoi(musDoc.first_node("sounds")->value());
    // for(int i = 0; i < musics; i++) {
    //         std::string tmp = "snd" + std::to_string(i);
    //         std::cout << tmp << "\n";
    //         std::string musPath = path + "/" + musDoc.first_node(tmp.c_str())->value();
    //         temp = Mix_LoadWAV(musPath.c_str());
    //         if (!temp) {
    //             printf("Failed to load sound at %s: %s\n", musPath, SDL_GetError());
    //         }
    //         music.push_back(temp);
    //         printf("Successfully loaded sound at %s\n", musPath.c_str());

    // }
    rapidxml::file<> bgFile((filepath"sound/sound.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("sounds");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

        std::cout << "loading sound " << child->first_node("path")->value() << "\n";
        //std::cout << "HELP ME:" << p.path().filename() << "\n";        
        Mix_Chunk* sound = Mix_LoadWAV(child->first_node("path")->value());
        if(!sound) {
            printf("Failed to load sound at %s: %s\n", child->first_node("path")->value(), SDL_GetError());
        }
        else {
            printf("Loaded sound %s!\n",child->first_node("path")->value());
            sfx->push_back(sound);
        }
    }
}


int graphics::generatesprites() {
    rapidxml::file<> bgFile((filepath"sprites/sprites.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("sprites");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

        std::cout << "loading sprite " << child->first_node("path")->value() << "\n";
        //std::cout << "HELP ME:" << p.path().filename() << "\n";        
        sprites[child->first_node("name")->value()]=(utils::getSDLTexture(child->first_node("path")->value(),graphics::render));
    }
    blocks->push_back(sprites["bblock"]);
    blocks->push_back(sprites["pblock"]);
    blocks->push_back(sprites["mblock"]);
    blocks->push_back(sprites["rblock"]);

    return 0;

}

void utils::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, bool center, int srcx, int srcy, int srcw, int srch, int scalex, int scaley) {
    SDL_Rect sprite;
    SDL_Rect srcrect = {srcx, srcy, srcw, srch};
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
    };
    sprite.w = srcw;
    sprite.h = srch;
    if (center) {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else {
        sprite.x = x;
        sprite.y = y;
    }
    SDL_RenderCopy(renderer, texture, &srcrect, &sprite);
}

double utils::lerp(double a, double b, double t)    {
        if (t <= 0.5)
            return a+(b-a)*t;
        else
            return b-(b-a)*(1.0-t);
    }
int utils::sign(int x) { //generic sign function
    return (x > 0) - (x < 0);
}

void graphics::drawTexture(SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
    SDL_Rect sprite;
    SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h);
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;
    if (center) {
        sprite.x = x - oldwidth / 2;
        sprite.y = y - oldheight / 2;
    }
    else {
        sprite.x = x + oldwidth / 2 - sprite.w / 2;
        sprite.y = y + oldheight / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(render, texture, NULL, &sprite, angle, NULL, SDL_FLIP_NONE);
}
float utils::mean(float arr[], int from, int to) {
    float ret = 0.0;
    for(int i = from; i < to; i++) {
        ret += arr[i];
    }
    return ret/to-from;
}
int utils::unsign(int a) {
    if(a < 0) {
        return 0;
    }
    return a;
}
//THE FOLLOWING IS ESPECIALLY THANKS TO ICR ON STACKOVERFLOW
//thank you so much dude for the explaination..
//translated from C# to C++ by me tho
//great read: https://stackoverflow.com/questions/17586/best-word-wrap-algorithm

std::string utils::wrap(std::string str, int pixels) {

    std::vector<std::string> words = seperateWords(str, ' ');
    int currentline = 0;
    std::string output = "";
    int currentpix = pixels;
    for (int i = 0; i < words.size(); i++) {
        std::string word = words.at(i);
        int count = 0;
        for (int i = 0; (i = word.find('@', i)) != std::string::npos; i++) {
            count++;
        }
        currentpix += count*2;
        //std::cout << word << count*2 << "\n";
        if (currentline + word.length() > currentpix) {
            if (currentline > 0) {
                currentpix = pixels;
                output += "\n";
                currentline = 1;
            }
            while(word.length() > currentpix) {
                currentpix = pixels;
                output += word.substr(0, currentpix-1) + "";
                word = word.substr(currentpix-1);
                output += "\n";
            }
            word = word.substr(word.find_first_of(' ')+1);
            
        }
        output += word;
        currentline += word.length();
    }

    return output;
}
std::vector<std::string> utils::seperateWords(std::string string, char sep) {

    return seperateWords(string, sep, 0);
}
int utils::keyboardCheck(int keycode) {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    return keys[keycode];
}
int utils::getMouseX() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return x;
}
int utils::getMouseY() {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return y;
}
int utils::mouseCheck(int keycode) {
    Uint32 buttons = SDL_GetMouseState(NULL, NULL);
    return (buttons & keycode) != 0;
}
std::vector<std::string> utils::seperateWords(std::string string, char sep, int x) {
    std::vector<std::string> parts;
    int startindex = 0;
    while (true) {
        int index = string.find_first_of(sep, startindex);
        if (index == -1) {
            parts.push_back(string.substr(startindex));
            return parts;
        }

        std::string word = string.substr(startindex, index - startindex-x);
        char nextchar = string.substr(index, 1).at(0);
        if (nextchar = sep) { //WHY

            parts.push_back(word);
            parts.push_back(std::string(1, nextchar));
        }
        startindex = index + 1;
    }
}
double utils::rad(double i) {
    return i*M_PI/180;
}
double utils::deg(double i) {
    return i*180/M_PI;
}
vect utils::rotate_to_point(vect object_position, vect point) {
    // Calculate direction vector
    float direction[3];
    direction[0] = point.x - object_position.x;
    direction[1] = point.y - object_position.y;
    direction[2] = point.z - object_position.z;

    // Calculate magnitude of direction vector
    float magnitude = sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);

    // Normalize direction vector
    direction[0] /= magnitude;
    direction[1] /= magnitude;
    direction[2] /= magnitude;

    // Calculate rotation angle using dot product
    float reference[3] = {0.0f, 0.0f, 1.0f}; // use a fixed reference vector
    float dotProduct = direction[0]*reference[0] + direction[1]*reference[1] + direction[2]*reference[2];
    float rotationAngle = acos(dotProduct);

    // Calculate rotation axis using cross product
    float rotationAxis[3];
    rotationAxis[0] = direction[1]*reference[2] - direction[2]*reference[1];
    rotationAxis[1] = direction[2]*reference[0] - direction[0]*reference[2];
    rotationAxis[2] = direction[0]*reference[1] - direction[1]*reference[0];

    // Normalize rotation axis
    float axisMagnitude = sqrt(rotationAxis[0]*rotationAxis[0] + rotationAxis[1]*rotationAxis[1] + rotationAxis[2]*rotationAxis[2]);
    rotationAxis[0] /= axisMagnitude;
    rotationAxis[1] /= axisMagnitude;
    rotationAxis[2] /= axisMagnitude;

    // Set rotation vector
    vect rotation(0,0,0);
    rotation.x = deg(rotationAxis[0] * rotationAngle);
    rotation.y = deg(rotationAxis[1] * rotationAngle);
    rotation.z = deg(rotationAxis[2] * rotationAngle);
}
