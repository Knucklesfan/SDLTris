#include "ttffont.h"
#include "utils/defs.h"
ttffont::ttffont(std::string name, int size) {
    font = FC_CreateFont();  
    std::string pth = "./fonts/" + name+ ".ttf";
    // FC_LoadFont(font, graphics::render, pth.c_str(), size, FC_MakeColor(255,255,255,255), TTF_STYLE_NORMAL);
}

void ttffont::render(int x, int y, std::string strg, bool center) {
    render(strg, x, y, center, 0, 0, 0);
}
void ttffont::render(int x, int y, std::string strg, bool center, bool sine, double pos, double multiplyin, double multiplyout) {
    render(strg, x, y, center, 0, 0, 0, 0, sine, pos, multiplyin, multiplyout, 1.0);
}

void ttffont::render(int x, int y, std::string strg, bool center, int wordwrap) {
    render(strg, x, y, center, 0, 0, 0, wordwrap);
}
void ttffont::render(std::string words, int x, int y, bool center, int red, int blue, int green) {
    render(words, x, y, center, red, blue, green, 0);
}
void ttffont::render(std::string strg, int x, int y, bool center) {
    render(strg, x, y, center, 0, 0, 0);
}
void ttffont::render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap) {
    render(words, x, y, center, red, blue, green, wordwrap, false, 0, 0, 0, 1.0);
}
void ttffont::render(std::string words, int x, int y, bool center, int red, int blue, int green, int wordwrap, bool sine, double pos, double multiplyin, double multiplyout, double scale) {
    // FC_Draw(font, graphics::render, x, y, words.c_str()); 
}