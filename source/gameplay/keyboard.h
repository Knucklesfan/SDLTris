#pragma once
// since the keyboard is used more than once, we'll move it to it's own
// class for ease of use and also extendability.

#include "SDL2/SDL_keycode.h"
#include <SDL2/SDL.h>
#include <string>
#define KEYBOARD_KEYS 40
class keyboard {
    public: 
        void logic(double deltatime);
        void render();
        void input(SDL_Keycode);
        std::string value;
        keyboard();
        bool open = false;
        void reset(int max_chars);
        int endlogic(); //yeah, i know adding an endlogic kinda turns this into a scene but whatever
    private:
        char displayKeys[KEYBOARD_KEYS] = { //the keys to render to the screen
			'1','2','3','4','5','6','7','8','9','0',
			'Q','W','E','R','T','Y','U','I','O','P',
			'A','S','D','F','G','H','J','K','L','-',
			'Z','X','C','V','B','N','M','<','>','$'
		};
		char keyboardKeys[KEYBOARD_KEYS] = { //the actual keys that are added to the string
			'1','2','3','4','5','6','7','8','9','0',
			'Q','W','E','R','T','Y','U','I','O','P',
			'A','S','D','F','G','H','J','K','L',32,
			'Z','X','C','V','B','N','M',37,38,39
		};
        SDL_Keycode physicalKeys[KEYBOARD_KEYS] = { //the physical keys on a real keyboard that these correspond to
            SDLK_1,SDLK_2,SDLK_3,SDLK_4,SDLK_5,SDLK_6,SDLK_7,SDLK_8,SDLK_9,SDLK_0,
            SDLK_q,SDLK_w,SDLK_e,SDLK_r,SDLK_t,SDLK_y,SDLK_u,SDLK_i,SDLK_o,SDLK_p,
            SDLK_a,SDLK_s,SDLK_d,SDLK_f,SDLK_g,SDLK_h,SDLK_j,SDLK_k,SDLK_l,SDLK_SPACE,
            SDLK_z,SDLK_x,SDLK_c,SDLK_v,SDLK_b,SDLK_n,SDLK_m,SDLK_BACKSPACE,SDLK_BACKSLASH,SDLK_EQUALS
        };

        int   max_chars = 0;
        int   keyboard_age = 0;
        int   selectedKey = 0;
        int   currentChar = 0;
        bool  active = false;
        float transition = 1;
        bool transitionDirection = 0; //0 up, 1 down

};