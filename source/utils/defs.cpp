#include "defs.h"
#include "SDL2/SDL_stdinc.h"
#include <stdexcept>
#include <string>
#ifdef __SWITCH__
    #define filepath  "/"
    #include <switch.h>

#else
    #define filepath  "./"
#endif
#include "../opengl/pixfont.h"
#include "../opengl/ttffont.h"
#include "../opengl/stb_image.h"
#include "../scenes/knuxfanscreen.h"
#include "../scenes/titlescreen.h"
#include "../scenes/gameplaymenu.h"
#include "../scenes/credits.h"

#include "../scenes/nullscene.h"
#include "../scenes/game.h"
#include "../scenes/results.h"
#include "../scenes/classicmenu.h"
#include "../scenes/highscore.h"

#include <cmath>
#include <sstream> //std::stringstream
#include <sys/stat.h>
#include <sys/types.h>
#include <filesystem>

SDL_Window* graphics::window = nullptr;
std::vector<modifier> gameplay::modifiers = std::vector<modifier>();
std::vector<Gamemode*> gameplay::gamemodes = std::vector<Gamemode*>();
int gameplay::gamemode = 0;
bool gameplay::transitioning = false;
int gameplay::Pieces[7][4][16] = {
            //S PIECE 
            {
            {0,1,1,0,
             0,0,1,1,
             0,0,0,0,
             0,0,0,0
            },
            {0,0,1,0,
             0,1,1,0,
             0,1,0,0,
             0,0,0,0
            },
            {0,0,0,1,
             0,0,1,1,
             0,0,1,0,
             0,0,0,0
            },
            {0,0,0,0,
             0,1,1,0,
             0,0,1,1,
             0,0,0,0
            }
        },
            //T PIECE
            {
            {
             0,0,2,0,
             0,2,2,0,
             0,0,2,0,
             0,0,0,0
            },
            {
             0,0,2,0,
             0,2,2,2,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,2,0,
             0,0,2,2,
             0,0,2,0,
             0,0,0,0
            },
            {
             0,0,0,0,
             0,2,2,2,
             0,0,2,0,
             0,0,0,0
            },

        },
            //B PIECE
            {
            {
             0,3,0,0,
             0,3,3,0,
             0,0,3,0,
             0,0,0,0
            },
            {
             0,0,3,3,
             0,3,3,0,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,3,0,0,
             0,3,3,0,
             0,0,3,0,
             0,0,0,0
            },
            {
             0,0,0,0,
             0,0,3,3,
             0,3,3,0,
             0,0,0,0
            },

        },
            //C PIECE
            {
            {
             0,0,0,0,
             0,4,4,0,
             0,4,4,0,
             0,0,0,0
            },
            {
             0,0,0,0,
             0,4,4,0,
             0,4,4,0,
             0,0,0,0
            },
            {
             0,0,0,0,
             0,4,4,0,
             0,4,4,0,
             0,0,0,0
            },
            {
             0,0,0,0,
             0,4,4,0,
             0,4,4,0,
             0,0,0,0
            },

        },
            //L PIECE
            {
            {
             0,0,0,0,
             0,5,5,5,
             0,5,0,0,
             0,0,0,0
            },
            {
             0,5,5,0,
             0,0,5,0,
             0,0,5,0,
             0,0,0,0
            },
            {
             0,0,0,5,
             0,5,5,5,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,5,0,
             0,0,5,0,
             0,0,5,5,
             0,0,0,0
            },

        },
            //P PIECE
            {
            {
             0,6,0,0,
             0,6,6,6,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,6,6,
             0,0,6,0,
             0,0,6,0,
             0,0,0,0
            },
            {
             0,6,6,6,
             0,0,0,6,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,6,0,
             0,0,6,0,
             0,6,6,0,
             0,0,0,0
            },

        },
            //R PIECE
            {
            {
             0,0,0,0,
             7,7,7,7,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,7,0,
             0,0,7,0,
             0,0,7,0,
             0,0,7,0
            },
            {
             0,0,0,0,
             0,0,0,0,
             7,7,7,7,
             0,0,0,0
            },
            {
             0,7,0,0,
             0,7,0,0,
             0,7,0,0,
             0,7,0,0
            },
        }
        };
#ifdef __LEGACY_RENDER
SDL_Renderer* graphics::render = nullptr;
std::map<std::string,SDL_Texture*> graphics::sprites = std::map<std::string,SDL_Texture*>();
std::vector<SDL_Texture*>* graphics::blocks = new std::vector<SDL_Texture*>();

#else
    rectRenderer* graphics::rect = NULL;
    lineRenderer* graphics::line = NULL;
    buffermanager* graphics::globalbuffer = NULL;
    bool settings::globalDebug = false;
    bool settings::usePreciseTiming = false;
    spriteRenderer* graphics::sprite = NULL;
    std::vector<texture*>* graphics::blocks = new std::vector<texture*>();
    std::vector<std::string> settings::demos = std::vector<std::string>();
    std::string settings::saveload = "";
    std::vector<save> settings::saveCache = std::vector<save>();

    std::map<std::string,texture*> graphics::sprites = std::map<std::string,texture*>();
    std::vector<shader *> graphics::shaders = std::vector<shader *>();
    
    rpcimplement* networking::globalRPC = NULL;

    std::map<std::string, layertype> bgconverters::layermap = {
        {"background", layertype::BACKGROUND}, //
        {"legacy", layertype::LEGACY},
        {"bg3d", layertype::BG3D},
        {"bg2d", layertype::BG2D},
        {"shader", layertype::SHADER}};
    ;
    std::map<std::string, headerdata> bgconverters::headermap =
        {
            {"title", headerdata::TITLE},
            {"version", headerdata::VERSION},
            {"music", headerdata::BGMUSIC},
            {"filename", headerdata::FILENAME},
            {"creator", headerdata::CREATOR}

    };

#endif
double graphics::deltaTime = 0;
int settings::maxscore = 0;
int settings::previousscore = 0;
int settings::lastlevel = 0;
int settings::lastlines = 0;
int settings::lasttime = 0;
std::string settings::configDir = "";
std::string settings::saveDir = "";

std::vector<bg>* graphics::backgrounds = new std::vector<bg>();
// std::vector<ObjectTemplate>* graphics::objects = new std::vector<ObjectTemplate>();
std::vector<Font*>* graphics::fonts = new std::vector<Font*>();
std::vector<Mix_Music*>* audio::music = new std::vector<Mix_Music*>();
std::vector<Mix_Chunk*>* audio::sfx = new std::vector<Mix_Chunk*>();
std::vector<unsigned int>* graphics::cubemaps = new std::vector<unsigned int>();

std::array<std::array<int, 12>, 5> settings::defaults = {{
		{ //GAMEPLAY
		1, //Ghost Piece
		1, //Hold Piece
		0, //Block Speed-Up
		1, //Fast drop
		1, //Scoring System
		0, //Repeat Holding
        0, //Hold scoring
        20, //Level Length
        0, //Further memory added in case of expansion
        0,
        0,
        0,
		},

		{ //DISPLAY
		1, //BG Cycle Mode
		0, //First Background selection
		1, //Line Clear Animation
		1, //Moving background animation
		1, //Near Top Flash
		0, //Low performance mode
        0, //Further memory added in case of expansion
        0, 
        0,
        0,
        0,
        0,
		},

		{ //SYSTEM
		0, //FULLSCREEN
		0, //MUSIC
		0, //SOUND
		0, //RESET
		0, //Further memory added in case of expansion
		0,
        0,
        0,
        0,
        0,
        0,
        0

		},

		{ //EXTRA
		0, //Rotate Board
		0, //Bigger Board
		0, //Blind Mode
		0, //Anti-Gravity (Blocks fall opposite direction)
		0, //Bomb block (explosion destroys blocks in region around drop site)
		0, //Mysteryblock!! (covers the block with a censor so you can't see it)
        0, //Mirror Mode
        0, //Acid bath (Acid falls on intrevals from random sides of the board, ocassionally covering large parts and consuming pieces and chunks in the process)
        0,
        0,
        0,
        0

		}, //DEBUG
		{
		0, //Show FPS
		0, //Show 
		0,
		0,
		0,
		0,
        0,
        0,
        0,
        0,
        0,
        0
		}

    }};
std::array<std::array<int, 12>, 5> settings::activations = std::array<std::array<int, 12>, 5>();
const Uint8 *graphics::state = SDL_GetKeyboardState(nullptr);
#ifdef __LEGACY_RENDER
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
#else
#endif
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
            f = new ttffont(child->first_node("path")->value());
        }
        else {
            f = new pixfont(child->first_node("path")->value());
        }
        fonts->push_back(f);
    }
    return 0;

}
std::string utils::getenv( const std::string & var ) {
     const char * val = std::getenv( var.c_str() );
     if ( val == nullptr ) { // invalid to assign nullptr to std::string
         return "";
     }
     else {
         return val;
     }
}
void settings::clearSaveData() {
    for(int i = 0; i < settings::saveCache.size(); i++) {
        glDeleteTextures(1, &settings::saveCache.at(i).t->id); //free up that texture
        delete(settings::saveCache.at(i).t); //go ahead and kill that texture in mem then
        std::vector<save>().swap(settings::saveCache); //kill up all them elements
    }
}
void settings::loadSaveData() {
    #ifdef _LINUX
        GLuint framebuffer;
    	glGenFramebuffers(1, &framebuffer);
	    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //handles running the game at the set resolution
        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 40, 96);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        

        std::cout << "Caching save\nFILENAME\tLINES\tLEVELS\n";
        //since we are on linux, the other platforms are only 
        //gonna get implemented when needed
        int testblocks[480];
        memset(testblocks,0,sizeof(testblocks));

        for (const auto & entry : std::filesystem::directory_iterator(saveDir)) {
            std::string savename = entry.path().string().substr(saveDir.length()+1);
            std::streampos size;
            char * memblock;
            std::ifstream file (entry.path().string(), std::ios::in|std::ios::binary|std::ios::ate);
            if (file.is_open())
            {
                int piece = 0;
                int hold = 0;
                int level = 0;
                int lines = 0;
                Uint32 score = 0;
                texture* t = new texture();
                t->w = 40;
                t->h = 96;
                GLint previousFB = 0;
                glGenTextures(1, &t->id); //generates a texture to render to
	            glBindTexture(GL_TEXTURE_2D, t->id);
		        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 40, 96, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, t->id, 0);
                GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
                glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
                if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                    throw("buffer error");
                }

                
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFB); //store previous framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                glViewport(0,0,40,96); // Activate and render at texture size.
                glColorMask(1, 1, 1, 1);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClearDepth(1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                size = file.tellg();
                memblock = new char [size];
                file.seekg (0, std::ios::beg);
                file.read (memblock, size);
                file.close();

                Uint32 version = 0; //the version of the current save type, in case i update this

                size_t offset = 0;
                memcpy(&version, memblock+offset, sizeof(uint)); //loads the version number first, so we know what we're working with here
                offset += sizeof(Uint32);
                std::cout << "-----\nVersion: "<< version << "\n";
                switch(version) {
                    case 1: {//early alpha save, doesnt record player save
                        offset += sizeof(uint); //time
                        offset += sizeof(uint); //randomiters

                        memcpy(&piece, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //piece
                        offset += sizeof(int); //x
                        offset += sizeof(int); //y
                        offset += sizeof(int); //rot
                        offset += sizeof(int); //next block
                        memcpy(&hold, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //hold block

                        memcpy(&level, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //level num
                        memcpy(&lines, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //lines number
                        offset += sizeof(Uint32); //gamestart
                        std::cout << savename << "\t" << level << "\t" << lines << "\n";

                        memcpy(&testblocks, memblock+offset, sizeof(int[480])); 
                    }break;
                    case 2: { //we're still in alpha, but has score saving now
                        offset += sizeof(uint); //time
                        offset += sizeof(uint); //randomiters

                        memcpy(&piece, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //piece
                        offset += sizeof(int); //x
                        offset += sizeof(int); //y
                        offset += sizeof(int); //rot
                        offset += sizeof(int); //next block
                        memcpy(&hold, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //hold block

                        memcpy(&level, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //level num
                        memcpy(&lines, memblock+offset, sizeof(int)); 
                        offset += sizeof(int); //lines number
                        memcpy(&score, memblock+offset, sizeof(Uint32)); 
                        offset += sizeof(Uint32); //score
                        offset += sizeof(Uint32); //gamestart
                        std::cout << savename << "\t" << level << "\t" << lines << "\n";

                        memcpy(&testblocks, memblock+offset, sizeof(int[480])); 

                    }break;
                }
                for(int j = 0; j < 240; j++) {
                    if(testblocks[j] > 0 && testblocks[j]-1 < graphics::blocks->size()) {
                        graphics::shaders.at(4)->activate();
                        graphics::sprite->render(
                            graphics::shaders.at(4),
                            graphics::blocks->at(testblocks[j]-1),
                            {
                                (j%10)*4,
                                (j/10)*4
                            },
                            {
                                4,
                                4
                            },
                            {0,0,0},
                            {0,0},
                            {4,4},
                            glm::vec2(40,96),
                            0.0f
                        );
                    }
                }
                glBindFramebuffer(GL_FRAMEBUFFER, previousFB); //restore previous framebuffer

                saveCache.push_back({
                    savename,
                    level,
                    lines,
                    hold,
                    piece,
                    score,
                    t
                });


                
            // saveCache.push_back({

            // })
        }
    }

    #elif _WIN32

    #elif _OSX

    #elif _MOBILE

    #endif
}
void settings::loadDemos() {
    rapidxml::file<> bgFile((filepath"demos/demos.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("demos");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != nullptr; child = child->next_sibling()) {

        std::cout << "loading demo " << child->value() << "\n";
        //std::cout << "HELP ME:" << p.path().filename() << "\n";
        demos.push_back(child->value());
    }
}
void graphics::screenshot() {
    SDL_Surface * temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

    char * pixels = new char [3 * 640 * 480];

    glReadPixels(0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (int i = 0 ; i < 480 ; i++)
        std::memcpy( ((char *) temp->pixels) + temp->pitch * i, pixels + 3 * 640 * (480-i - 1), 640*3 );

    delete [] pixels;
    printf("Screenshot taken!\n");
    std::string filename = "pic"+std::to_string(time(NULL))+".bmp";
    SDL_SaveBMP(temp, filename.c_str());

    SDL_FreeSurface(temp);
}
void graphics::generatecubemaps() {
    rapidxml::file<> bgFile((filepath"skyboxes/skyboxes.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("skyboxes");
    int i = 0;
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != nullptr; child = child->next_sibling()) {

        std::cout << "loading skybox #" << i << "\n";
        //std::cout << "HELP ME:" << p.path().filename() << "\n";
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
        std::string faces[6] = {
            child->first_node("right")->value(),
            child->first_node("left")->value(),
            child->first_node("bottom")->value(),
            child->first_node("top")->value(),
            child->first_node("front")->value(),
            child->first_node("back")->value()
        };
        int width, height, nrChannels;
        for (unsigned int i = 0; i < 6; i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        cubemaps->push_back(textureID);
        i++;
    }

}
int graphics::generatebgs() {
   try {
        rapidxml::file<> bgFile((filepath"backgrounds/backgrounds.xml"));
        rapidxml::xml_document<> bgDoc;
        bgDoc.parse<0>(bgFile.data());
        rapidxml::xml_node<char>* parent = bgDoc.first_node("backgrounds");
        for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

            std::cout << "loading background " << child->value() << "\n";
            //std::cout << "HELP ME:" << p.path().filename() << "\n";
            if(std::string(child->value()) != "") {
                #ifdef __LEGACY_RENDER
                bg backg(child->value(), false, render);
                #else
                bg backg(child->value(), false);
                #endif
                backgrounds->push_back(backg);
            }

        }
    }
    catch(std::runtime_error& e) { //if that fails, at least try loading in ANY background
        bg backg("null", false);
        backgrounds->push_back(backg); //but if this fails, then we just throw in the towel and finally crash.
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
void audio::playMusic(int index) {
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
void audio::playSound(int index) {
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
        #ifdef __LEGACY_RENDER
        sprites[child->first_node("name")->value()]=(utils::getSDLTexture(child->first_node("path")->value(),graphics::render));
        #else
        sprites[child->first_node("name")->value()]=new texture(child->first_node("path")->value());
        #endif
    }
    blocks->push_back(sprites["bblock"]);
    blocks->push_back(sprites["pblock"]);
    blocks->push_back(sprites["mblock"]);
    blocks->push_back(sprites["rblock"]);
    blocks->push_back(sprites["gblock"]);
    blocks->push_back(sprites["yblock"]);
    blocks->push_back(sprites["tblock"]);

    return 0;
}
int graphics::generateshaders() {
    rapidxml::file<> bgFile((filepath"shaders/shaders.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("shaders");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

        std::cout << "loading shader " << child->first_node("vertex")->value() << " " << child->first_node("fragment")->value() << "\n";
        //std::cout << "HELP ME:" << p.path().filename() << "\n";        
        #ifdef __LEGACY_RENDER
        sprites[child->first_node("name")->value()]=(utils::getSDLTexture(child->first_node("path")->value(),graphics::render));
        #else
        shaders.push_back(new shader(child->first_node("vertex")->value(),child->first_node("fragment")->value()));
        #endif
    }

    return 0;
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
#ifdef __LEGACY_RENDER
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
void graphics::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, bool center, int srcx, int srcy, int srcw, int srch, int scalex, int scaley) {
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

#endif

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
void settings::loadSettings() {
    std::string file = configDir+"/save.xml";
    std::ifstream f(file); //load in the save file first, get our high scores and whatnot

	if (f.good()) { //if save file does exist
        
		rapidxml::file<> xmlFile(file.c_str()); //load it
		rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
		maxscore = atoi(doc.first_node("scoring")->first_node("highscore")->value());
		previousscore = atoi(doc.first_node("scoring")->first_node("highscore")->value());

	}
	else { //otherwise, we gotta make it ourselves
		std::ofstream outfile(file); //making this bad boy
		outfile << "<scoring><lastscore>0</lastscore>\n" 
			<< "<highscore>0</highscore></scoring>"; //HARDCODED BECAUSE IM A SIMP
		outfile.close();
		maxscore = 0;
		previousscore = 0;

	}

	f.close(); //always close your shit
    file = configDir+"/settings.xml";
    std::ifstream s(file); //load in the save file first, get our high scores and whatnot
    if (s.good()) { //if file exists, time to load
        rapidxml::file<> xmlFile(file.c_str()); //load it
        rapidxml::xml_document<> doc;
		doc.parse<0>(xmlFile.data());
        for (rapidxml::xml_node<char>* child = doc.first_node("settings")->first_node(); child != NULL; child = child->next_sibling()) {
            std::string name = child->name();
            std::cout << "loading " << child->name() << "\n";
            //INCOMING! NASTY YANDEREDEV IF CASES!!!
            if(name == "gameplay") { //handle gameplay
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::GHOSTPIECE] = atoi(child->first_node("ghost")->value());
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::HOLDPIECE] = atoi(child->first_node("hold")->value());
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::BLOCKSPEED] = atoi(child->first_node("speedup")->value());
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::FASTDROP] = atoi(child->first_node("fastdrop")->value());
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::SCORING] = atoi(child->first_node("scoring")->value());
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::REPEATHOLD] = atoi(child->first_node("repeathold")->value());
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::HOLDSCORING] = atoi(child->first_node("holdscoring")->value());
                activations[OPTIONTYPE::GAMEPLAY][GAMEPLAYOPTIONS::LEVELLENGTH] = atoi(child->first_node("levellength")->value());
            }
            else if(name == "display") {
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::BGMODE] = atoi(child->first_node("bgcycle")->value());
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::FIRSTBG] = atoi(child->first_node("firstbg")->value());
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::LINECLEAR] = atoi(child->first_node("lineclear")->value());
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::MOVINGBG] = atoi(child->first_node("motion")->value());
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::NEARTOPFLASH] = atoi(child->first_node("neartop")->value());
                activations[OPTIONTYPE::DISPLAY][DISPLAYOPTIONS::LOWPERF] = atoi(child->first_node("lowperf")->value());

            }
            else if(name == "system") {
                activations[OPTIONTYPE::SYSTEM][SYSTEMOPTIONS::FULLSCREEN] = atoi(child->first_node("fullscreen")->value());
                activations[OPTIONTYPE::SYSTEM][SYSTEMOPTIONS::MUSIC] = atoi(child->first_node("music")->value());
                activations[OPTIONTYPE::SYSTEM][SYSTEMOPTIONS::SOUNDS] = atoi(child->first_node("sound")->value());
                activations[OPTIONTYPE::SYSTEM][SYSTEMOPTIONS::RESET] = atoi(child->first_node("reset")->value());

            }
            else if(name == "extra") {
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::ROTATEBOARD] = atoi(child->first_node("rotate")->value());
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::BIGGERBOARD] = atoi(child->first_node("bigger")->value());
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::BLINDMODE] = atoi(child->first_node("blind")->value());
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::ANTIGRAVITY] = atoi(child->first_node("antigravity")->value());
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::BOMB] = atoi(child->first_node("bomb")->value());
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::MYSTERYBLOCK] = atoi(child->first_node("mysteryblock")->value());
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::MIRROR] = atoi(child->first_node("mirror")->value());
                activations[OPTIONTYPE::EXTRA][EXTRAOPTIONS::ACIDBATH] = atoi(child->first_node("acidbath")->value());

            }
            else if(name == "debug") {
                activations[OPTIONTYPE::DEBUG][DEBUGOPTIONS::DEBUGENABLED] = atoi(child->first_node("showfps")->value());
            }

        }


	}
    else { //if file does not exist, then dont' have any settings
        activations = defaults;
    }
    f.close();
    //lmao ugly ass debug code coming
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
        if (nextchar == sep) { //WHY

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

//I have NO idea what this code does, so uncommenting for now...
// \/\/\/\/

// vect utils::rotate_to_point(vect object_position, vect point) {
//     // Calculate direction vector
//     float direction[3];
//     direction[0] = point.x - object_position.x;
//     direction[1] = point.y - object_position.y;
//     direction[2] = point.z - object_position.z;

//     // Calculate magnitude of direction vector
//     float magnitude = sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);

//     // Normalize direction vector
//     direction[0] /= magnitude;
//     direction[1] /= magnitude;
//     direction[2] /= magnitude;

//     // Calculate rotation angle using dot product
//     float reference[3] = {0.0f, 0.0f, 1.0f}; // use a fixed reference vector
//     float dotProduct = direction[0]*reference[0] + direction[1]*reference[1] + direction[2]*reference[2];
//     float rotationAngle = acos(dotProduct);

//     // Calculate rotation axis using cross product
//     float rotationAxis[3];
//     rotationAxis[0] = direction[1]*reference[2] - direction[2]*reference[1];
//     rotationAxis[1] = direction[2]*reference[0] - direction[0]*reference[2];
//     rotationAxis[2] = direction[0]*reference[1] - direction[1]*reference[0];

//     // Normalize rotation axis
//     float axisMagnitude = sqrt(rotationAxis[0]*rotationAxis[0] + rotationAxis[1]*rotationAxis[1] + rotationAxis[2]*rotationAxis[2]);
//     rotationAxis[0] /= axisMagnitude;
//     rotationAxis[1] /= axisMagnitude;
//     rotationAxis[2] /= axisMagnitude;

//     // Set rotation vector
//     vect rotation(0,0,0);
//     rotation.x = deg(rotationAxis[0] * rotationAngle);
//     rotation.y = deg(rotationAxis[1] * rotationAngle);
//     rotation.z = deg(rotationAxis[2] * rotationAngle);

// }
std::string utils::loadFile(std::string filename)
{
    std::ifstream inFile;
    inFile.open(filename); // open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); // read the file
    return strStream.str();      // str holds the content of the file
}

void memory::freeBackgrounds() {

}
void memory::freeFonts() {

}
void memory::freeShaders() {

}
void memory::freeSprites() {

}
void gameplay::loadGamemodes() {
    nullscene* nul = new nullscene(); //from now on, first scene will always be a nullscene
    gamemodes.push_back(nul); //reason? this makes it way softer to crash. if the code references a non-existent scene for whatever reason, we send em to scene 0 instead.

    rapidxml::file<> bgFile((filepath"scenes/scenes.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("scenes");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {

        std::cout << "loading gamemode " << child->first_node("name")->value() << "\n";
        std::string type = child->first_node("type")->value();
        std::string name = child->first_node("name")->value();

        if(type == "knuxfanscreen") {
            knuxfanscreen* tmp = new knuxfanscreen();
            tmp->name = name;
            gamemodes.push_back(tmp);
        }
        else if(type == "titlescreen") {
            titlescreen* tmp = new titlescreen();
            tmp->name = name;
            gamemodes.push_back(tmp);

        }
        else if(type == "gameplaymenu") {
            gameplaymenu* tmp = new gameplaymenu();
            tmp->name = name;
            gamemodes.push_back(tmp);

        }
        else if(type == "game") {
            game* tmp = new game();
            tmp->name = name;
            gamemodes.push_back(tmp);

        }
        else if(type == "results") {
            results* tmp = new results();
            tmp->name = name;
            gamemodes.push_back(tmp);

        }
        else if(type == "credits") {
            credits* tmp = new credits();
            tmp->name = name;
            gamemodes.push_back(tmp);

        }
        else if(type == "classicmenu") {
            classicmenu* tmp = new classicmenu();
            tmp->name = name;
            gamemodes.push_back(tmp);

        }
        else if(type == "highscore") {
            highscore* tmp = new highscore();
            tmp->name = name;
            gamemodes.push_back(tmp);

        }

        else if(type == "scene") {
                        // gamemodes.push_back(new titlescreen());
        }
        else {
            std::cout << "Scene type not found \""<<type<<"\", adding null scene in its place...\n";
            nullscene* nul = new nullscene();
            gamemodes.push_back(nul);
        }

    }
}

void gameplay::loadModifiers() {
    std::cout << "loading modifiers\n"; 
    rapidxml::file<> bgFile((filepath"modifiers/modifiers.xml"));
    rapidxml::xml_document<> bgDoc;
    bgDoc.parse<0>(bgFile.data());
    rapidxml::xml_node<char>* parent = bgDoc.first_node("modifiers");
    for (rapidxml::xml_node<char>* child = parent->first_node(); child != NULL; child = child->next_sibling()) {
        std::string path = child->value();
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
        //TODO: add actual parsing of effects and make mods do stuff

        //add to modifier array
        modifiers.push_back({meta});
    }

}
double math::easeOutBounce(double x) {
    double n1 = 7.5625;
    double d1 = 2.75;

    if (x < 1 / d1) {
        return n1 * x * x;
    } else if (x < 2 / d1) {
        return n1 * (x -= 1.5 / d1) * x + 0.75;
    } else if (x < 2.5 / d1) {
        return n1 * (x -= 2.25 / d1) * x + 0.9375;
    } else {
        return n1 * (x -= 2.625 / d1) * x + 0.984375;
    }

}
float math::easeInOutCubic(float x) {
    return -std::pow((2*x)-1, 2)+1;
}

int math::numActive(Uint64* a) { //counts the number of zeroes in a 64bit int ARRAY!
    int count = 0;
    for(int j = 0; j < 8; j++) {
        for(Uint64 i = 0; i < 64; i++) {
            if(a[j]>>i&1) {
                count++;
            }
        }

    }
    return count;
}