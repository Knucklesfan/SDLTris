#pragma once

#include <string>

class Font {
    public:
        virtual int render(std::string words, int x, int y,bool center, 
        int red, int blue, int green, int wordwrap, bool sine, double pos,
        double multiplyin, double multiplyout, double scale,int scrwidth, int scrheight){};

        virtual int render(std::string words, int x, int y,bool center, 
        int red, int blue, int green, int wordwrap, bool sine, double pos,
        double multiplyin, double multiplyout, double scale){
            return render(words,x,y,center,red,blue,green,wordwrap,sine,pos,multiplyin,multiplyout,scale,640,480);
        };
        
        virtual int render(int x, int y,std::string words,
        bool center, int red, int blue, int green,
        int wordwrap, bool sine, double pos,
        double multiplyin, double multiplyout){
            return render(words,x,y,center,red,blue,green,wordwrap,sine,pos,multiplyin,multiplyout,1);
            };

        virtual int render(int x, int y, std::string strg, bool center){};
        virtual void logic(double deltatime){};

        int wordsize; 
        //very simple variable designed to wrap the width of a word for rendering purposes
        //designed to be ROUGH, only use this for calculations with pixfonts!
        //if a ttf font uses it, who knowssssss
};
