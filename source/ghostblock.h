#pragma once
class ghostblock
{
public:
    ghostblock();
    void rebirth(int xspawn, int yspawn, int block, int(barray)[240]);
    void changePos(int ax, int ay, int arot);
    bool collides(int colx, int coly, int colrot);
    void draw();
    int x, y, lastx, lasty, rot, lastrot, width, height, piece;
    int* array;
    int* outsidearray;
    bool alive;
    ghostblock(int, int, int(barray)[], int bwidth, int bheight, int block, int(outsidearray)[]);
    void removeolddraw();
    void redraw();
};

