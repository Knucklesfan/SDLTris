#include "credits.h";

credits::credits(std::vector<font*> fonts) {
    headerfont = fonts[0];
    textfont = fonts[1];
}
void credits::logic(double deltaTime) {
    wordsy -= deltaTime/25;
}
void credits::render(SDL_Renderer* render) {
    int layers = 0;
    for(int i = 0; i < 25; i++) {
        switch(wordprops[i]) {
            case 1: {
                textfont->render(320, wordsy+layers, words[i], true, render,580);
                layers+=textfont->height;
                break;
            };
            case 0: {
                headerfont->render(320, wordsy+layers, words[i], true, render,580);
                layers+=headerfont->height;
                break;
            };
            case 2: {
                headerfont->render(320, wordsy+layers, words[i], true, render,580);
                layers+=headerfont->height;
                break;
            }

        }
        layers+=wordspacing[i];
    }
}