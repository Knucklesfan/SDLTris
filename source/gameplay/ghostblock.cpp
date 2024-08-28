#include "ghostblock.h"
#include <stdio.h>
#include "../utils/defs.h"
ghostblock::ghostblock() {
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

ghostblock::ghostblock(int xspawn, int yspawn, int(barray)[240], int bwidth, int bheight, int block, int(coutsidearray)[240]) {
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
    outsidearray = coutsidearray;
    while (collides(x, y + 1, rot) && y <= 24) {
         removeolddraw();
         lastx = x;
         lasty = y;
         lastrot = rot;
         y += 1;
         redraw();
    }

}
void ghostblock::rebirth(int xspawn, int yspawn, int block, int(barray)[240]) {
    x = xspawn;
    y = yspawn;
    lastx = xspawn;
    lasty = yspawn;
    rot = 0;
    lastrot = 0;
    alive = true;
    piece = block;
    array = barray;
    while (collides(x, y + 1, rot) && y <= 24) {
        removeolddraw();
        lastx = x;
        lasty = y;
        lastrot = rot;
        y += 1;
        redraw();
    }

}
void ghostblock::changePos(int ax, int ay, int arot) {
    removeolddraw();
    lastx = x; //always preserve value before moving
    lasty = y; //to ensure that undrawing works.
    lastrot = rot;
    x = ax;
    y = ay-1;
    rot = arot;
    while (collides(x, y + 1, rot) && y <= 24) {
        removeolddraw();
        lastx = x;
        lasty = y;
        lastrot = rot;
        y += 1;
        redraw();
    }
    redraw();
}
bool ghostblock::collides(int colx, int coly, int colrot) {
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
void ghostblock::draw() {
    //Pieces[piece][rot][actual data here]
    removeolddraw();
    redraw();
    lastx = x; //always preserve value before moving
    lasty = y; //to ensure that undrawing works.
    lastrot = rot;

}

void ghostblock::removeolddraw() {
    for (int i = 0; i < 16; i++) {
        int ycalc = (i / 4) * width;
        if (gameplay::Pieces[piece][lastrot][i] > 0) {
            outsidearray[lastx + (lasty * width) + i % 4 + ycalc] = 0;
        }
    }

}

void ghostblock::redraw() {
    for (int i = 0; i < 16; i++) {
        int ycalc = (i / 4) * width;
        if (gameplay::Pieces[piece][rot][i] > 0) {
            outsidearray[x + (y * width) + i % 4 + ycalc] = gameplay::Pieces[piece][rot][i];
        }

    }

}
