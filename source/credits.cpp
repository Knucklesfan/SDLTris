#include "credits.h";

credits::credits(SDL_Renderer* renderer,std::vector<font*> fonts) {
    headerfont = fonts[0];
    textfont = fonts[1];
    sineWave = new sine(renderer, 0, 120, 320, 240);

}
void credits::logic(double deltaTime) {
    wordsy -= deltaTime/25;
    sineWave->logic(deltaTime);
}
void credits::render(SDL_Renderer* render) {
    sineWave->render(render,255,255,255);
    int layers = 0;
    for(int i = 0; i < 26; i++) {
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