#include "keyboard.h"
#include "../utils/defs.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_timer.h"
void keyboard::logic(double deltatime) { 
    //this isnt really used, but i think personally its a bad idea
    //to iterate in the render function sooo sorry :/
    if(transition > 0 && active) {
        double lerpval = utils::lerp(transition,0,deltatime*0.05);
        if(std::abs(lerpval) < 0.01) {
            transition = 0;
        }
        else {
            transition = lerpval;
        }
    }

}
void keyboard::reset(int max_char, std::string prompt) {
    active = true;
    max_chars = max_char;
    value = "test";
    selectedKey = 39;
    currentChar = 0;
    open = true;
    keyboard_age = SDL_GetTicks64();
}
void keyboard::render() {
    if(active) {
        float transLerp = (transition*480)*(transitionDirection);
        graphics::rect->render(graphics::shaders.at(1),{0,0+transLerp},{640,480},0,{0,0,0,0.5},false,-1,{0,0,0,0});
        graphics::fonts->at(0)->render(320,48+transLerp, "Please enter text.",true);
        for(int i = 0; i < value.length(); i++) {
            std::string str(1,value.at(i));

            // header->render(320-(16*4)+i*16,80, str,true);
            graphics::fonts->at(0)->render(320-(16*(max_chars/2))+i*16,80+transLerp,str,true,255,currentChar==i?0:255,255,-1,false,0,0,0);

        }
        for(int i = 0; i < 40; i++) {
            std::string str(1,displayKeys[i]);
            graphics::fonts->at(0)->render(200+(i%10)*24,160+(i/10)*32+transLerp,str,true,255,selectedKey==i?96-64*sin(SDL_GetTicks()/500.0f):255,255,-1,false,0,0,0);
        }
    }

}
void keyboard::input(SDL_Keycode key) {
    if(active) {
            std::cout << currentChar << " " << value.length() << "\n";
            switch(key) {
                case(SDLK_UP): {
                    if (selectedKey > 10) {
                        selectedKey-=10;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_DOWN): {
                    if (selectedKey < KEYBOARD_KEYS-10) {
                        selectedKey+=10;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_LEFT): {
                    if (selectedKey > 0) {
                        selectedKey--;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_RIGHT): {
                    if (selectedKey < KEYBOARD_KEYS) {
                        selectedKey++;
                    }
                    Mix_PlayChannel(-1, audio::sfx->at(1), 0);
                    break;
                }
                case(SDLK_RETURN): {
                    if(selectedKey < 37) { //if we arent the two move keys, then we're fine to keep going
                        if(currentChar < max_chars) {
                            if(currentChar > value.length()-1 || value.empty()) {
                                value.push_back(keyboardKeys[selectedKey]);
                            }
                            else {
                                value.at(currentChar) = keyboardKeys[selectedKey];
                            }

                            currentChar++;
                        }

                    }
                    else { //otherwise, we gotta actually handle those...
                        if(selectedKey == 37) { //go back one key
                            if(currentChar > 0) {
                                currentChar--;
                            }
                        }
                        else if(selectedKey == 38) { //go forward
                            if(currentChar < 8) {
                                currentChar++;
                            }
                        }
                        else if(selectedKey == 39) {
                            active = false;
                            //INSERT SUBMISSION CODE HERE
                        }
                    }
                }
                default: {
                    int keypressed = -1;
                    for(int i = 0; i < KEYBOARD_KEYS; i++) {
                        if(physicalKeys[i] == key) {
                            keypressed = i;
                            break;
                        }
                    }
                    if(keypressed < 37 && keypressed >= 0) { //if we arent the two move keys, then we're fine to keep going
                        if(currentChar < max_chars) {
                            if(currentChar > value.length()-1 || value.empty()) {
                                value.push_back(keyboardKeys[keypressed]);
                            }
                            else {
                                value.at(currentChar) = keyboardKeys[keypressed];
                            }
                            std::cout << keyboardKeys[keypressed] << "\n";

                            currentChar++;
                        }
                    }
                    else { //otherwise, we gotta actually handle those...
                        if(keypressed == 37) { //go back one key
                            if(currentChar > 0 && !value.empty()) {
                                value.pop_back();
                                currentChar--;
                            }
                        }
                        else if(keypressed == 38) { //go forward
                            if(currentChar < 8) {
                                currentChar++;
                            }
                        }
                        else if(keypressed == 39) {
                            active = false;
                            transition = 1;
                            transitionDirection = 1;
                            //INSERT SUBMISSION CODE HERE
                        }
                    }

                }break;

            }
    }

}
int keyboard::endlogic() {
    return active;
}
keyboard::keyboard() {
    max_chars = 0;
    keyboard_age = 0;
    selectedKey = 0;
    currentChar = 0;
    active = false;
    transition = 1;
}