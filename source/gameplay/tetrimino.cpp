#include "tetriminos.h"
#include <cstring>
#include "../utils/defs.h"
tetrimino::tetrimino() {
    x = 0;
    y = 0;
    rot = 0;
    //array = new int[240];
    width = 10;
    height = 24;
    alive = false;
    piece = 0;
    lastx = 0;
    lasty = 0;
    lastrot = 0;
}
tetrimino::tetrimino(int xspawn, int yspawn, int(barray)[240], int bwidth, int bheight, int block) {
    x = xspawn;
    y = yspawn;
    lastx = xspawn;
    lasty = yspawn;
    rot = 0;
    lastrot = 0;
    array = barray;
    width = bwidth;
    height = bheight;
    alive = true;
    piece = block;
}
bool tetrimino::rebirth(int xspawn, int yspawn, int block) {
    x = xspawn;
    y = yspawn;
    lastx = xspawn;
    lasty = yspawn;
    rot = 0;
    lastrot = 0;
    alive = true;
    piece = block;
    return collides(x + 1, y, rot);
}

void tetrimino::movedown() {
    if (alive) {
        removeolddraw();
        lastx = x;
        lasty = y;
        lastrot = rot;
        bool movable = collides(x, y + 1, rot);
        if (y <= 24 && movable) {
            y += 1;
        }
        else {
            alive = false;
        }
        redraw();
    }
}
int tetrimino::forcedrop() {
    int iterations = 0;
    if (alive) {
        while (collides(x, y + 1, rot) && y <= 24) {
            removeolddraw();
            lastx = x;
            lasty = y;
            lastrot = rot;
            y += 1;
            iterations++;
            redraw();
        }
        alive = false;
        return iterations*2;
    }
    return 0;
}
void tetrimino::hold() {
    removeolddraw();
    alive = false;
}

void tetrimino::moveleft() {
    if (alive) {
        removeolddraw();
        lastx = x; //always preserve value before moving
        lasty = y; //to ensure that undrawing works.
        lastrot = rot;

        if (collides(x - 1, y, rot)) {
            x -= 1;
        }
        redraw();
    }
}
void tetrimino::moveright() {
    if (alive) {
        removeolddraw();
        lastx = x;
        lasty = y;
        lastrot = rot;

        if (collides(x + 1, y, rot)) {
            x += 1;
        }
        redraw();
    }
}
bool tetrimino::collides(int colx, int coly, int colrot) {
    lastx = x; //always preserve value before moving
    lasty = y; //to ensure that undrawing works.
    lastrot = rot;
    removeolddraw();
    if (coly > height+1) {
        return false;
    }
    for (int i = 0; i < 16; i++) {
        int ycalc = (i / 4) * width;
        if ((gameplay::Pieces[piece][colrot][i] > 0)) {
            if ((ycalc + (coly * width) >= height * width)) {
                return false;
            }
            if ((colx + i % 4 >= width) || colx + i % 4 <= -1) {
                return false;
            }
            if (array[colx + (coly * width) + i % 4 + ycalc] > 0) {
                return false;
            }
        }
    }
    redraw();
    return true;
}
void tetrimino::rotate() {
    if (alive) {
        removeolddraw();
        lastx = x; //always preserve value before moving
        lasty = y; //to ensure that undrawing works.
        lastrot = rot;
        if (collides(x, y, rot + 1)) {
            rot = (rot + 1) % 4;
        }
        //printf("current rot: %i last rot: %i\n", rot, lastrot);
        redraw();
    }
}
void tetrimino::draw() {
    //Pieces[piece][rot][actual data here]
    removeolddraw();
    redraw();
    lastx = x; //always preserve value before moving
    lasty = y; //to ensure that undrawing works.
    lastrot = rot;

}

void tetrimino::removeolddraw() {
    for (int i = 0; i < 16; i++) {
        //std::cout << "Pieces[" << piece << "][" <<lastrot << "][" << i << "] > 0\n";
        int ycalc = (i / 4) * width;
        if (gameplay::Pieces[piece][lastrot][i] > 0) {
            array[lastx + (lasty * width) + i % 4 + ycalc] = 0;
        }
    }

}

void tetrimino::redraw() {
    for (int i = 0; i < 16; i++) {
        int ycalc = (i / 4) * width;
        if (gameplay::Pieces[piece][rot][i] > 0) {
            array[x + (y * width) + i % 4 + ycalc] = gameplay::Pieces[piece][rot][i];
        }

    }

}
