#pragma once

#include <string>

class Font {
    public:
        virtual void render(std::string words, int x, int y,
        bool center, int red, int blue, int green,
        int wordwrap, bool sine, double pos,
        double multiplyin, double multiplyout, int scale){};
        
        virtual void render(int x, int y,std::string words,
        bool center, int red, int blue, int green,
        int wordwrap, bool sine, double pos,
        double multiplyin, double multiplyout){
            render(words,x,y,center,red,blue,green,wordwrap,sine,pos,multiplyin,multiplyout,1);
            };

        virtual void render(int x, int y, std::string strg, bool center){};
        virtual void logic(double deltatime){};

};
