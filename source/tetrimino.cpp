#include "tetriminos.h"
#include <stdio.h>
#include <iostream>
#include <cstring>

	    int x,y,lastx,lasty,rot,lastrot,width,height,piece;
        int Pieces[7][4][16] = {
            //S PIECE 
            {
            {0,1,1,0,
             0,0,1,1,
             0,0,0,0,
             0,0,0,0
            },
            {0,0,1,0,
             0,1,1,0,
             0,1,0,0,
             0,0,0,0
            },
            {0,0,0,1,
             0,0,1,1,
             0,0,1,0,
             0,0,0,0
            },
            {0,0,0,0,
             0,1,1,0,
             0,0,1,1,
             0,0,0,0
            }
        },
            //T PIECE
            {
            {
             0,0,2,0,
             0,2,2,0,
             0,0,2,0,
             0,0,0,0
            },
            {
             0,0,2,0,
             0,2,2,2,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,2,0,
             0,0,2,2,
             0,0,2,0,
             0,0,0,0
            },
            {
             0,0,0,0,
             0,2,2,2,
             0,0,2,0,
             0,0,0,0
            },

        },
            //B PIECE
            {
            {
             0,0,3,0,
             0,0,3,3,
             0,0,0,3,
             0,0,0,0
            },
            {
             0,0,3,3,
             0,3,3,0,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,3,0,
             0,0,3,3,
             0,0,0,3,
             0,0,0,0
            },
            {
             0,0,0,0,
             0,0,3,3,
             0,3,3,0,
             0,0,0,0
            },

        },
            //C PIECE
            {
            {
             0,0,1,1,
             0,0,1,1,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,1,1,
             0,0,1,1,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,1,1,
             0,0,1,1,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,1,1,
             0,0,1,1,
             0,0,0,0,
             0,0,0,0
            },

        },
            //L PIECE
            {
            {
             0,0,0,0,
             0,2,2,2,
             0,2,0,0,
             0,0,0,0
            },
            {
             0,2,2,0,
             0,0,2,0,
             0,0,2,0,
             0,0,0,0
            },
            {
             0,0,0,2,
             0,2,2,2,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,2,0,
             0,0,2,0,
             0,0,2,2,
             0,0,0,0
            },

        },
            //P PIECE
            {
            {
             0,3,0,0,
             0,3,3,3,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,3,3,
             0,0,3,0,
             0,0,3,0,
             0,0,0,0
            },
            {
             0,3,3,3,
             0,0,0,3,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,3,0,
             0,0,3,0,
             0,3,3,0,
             0,0,0,0
            },

        },
            //R PIECE
            {
            {
             0,0,0,0,
             1,1,1,1,
             0,0,0,0,
             0,0,0,0
            },
            {
             0,0,1,0,
             0,0,1,0,
             0,0,1,0,
             0,0,1,0
            },
            {
             0,0,0,0,
             0,0,0,0,
             1,1,1,1,
             0,0,0,0
            },
            {
             0,1,0,0,
             0,1,0,0,
             0,1,0,0,
             0,1,0,0
            },

        }
        };
        int* array;
        bool alive;
tetrimino::tetrimino(int xspawn, int yspawn, int (barray)[200], int bwidth, int bheight) {
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
    piece = 0;
}
void tetrimino::rebirth(int xspawn, int yspawn) {
    x = xspawn;
    y = yspawn;
    lastx = xspawn;
    lasty = yspawn;
    rot = 0;
    lastrot = 0;
    alive = true;
    piece = rand() % 7;
}

void tetrimino::movedown() {
    if(alive) {
        removeolddraw();
        lastx = x;
        lasty = y;
        lastrot=rot;
        bool movable = collides(x, y+1, rot);
        if(y <= 20 && movable) {
            y+=1;
        }
        else {
            alive = false;
        }
        redraw();
    }
}
void tetrimino::moveleft() {
    if(alive) {
        removeolddraw();
        lastx = x; //always preserve value before moving
        lasty = y; //to ensure that undrawing works.
        lastrot=rot;

        if(collides(x-1, y, rot)) {
            x-=1;
        }
        redraw();
    }
}
void tetrimino::moveright() {
    if(alive) {
        removeolddraw();
        lastx = x;
        lasty = y;
        lastrot=rot;

        if(collides(x+1, y, rot)) {
            x+=1;
        }
        redraw();
    }
}
bool tetrimino::collides(int colx, int coly, int colrot) {
    lastx = x; //always preserve value before moving
    lasty = y; //to ensure that undrawing works.
    lastrot=rot;
    removeolddraw();
    if(coly > height) {
        return false;
    }
    for(int i = 0; i < 16; i++) {
        int ycalc = (i/4)*width;
        if((Pieces[piece][colrot][i] > 0)) {
            if((ycalc + (coly*width) >= height*width)) {
                return false;
            }
            if((colx+i%4 >= width) || colx+i%4 <= -1) {
                return false;
            }
            if(array[colx + (coly*width) + i%4 + ycalc] > 0) {
                return false;
            }
        }
    }
    redraw();
    return true;
}
void tetrimino::rotate() {
    if(alive) {
        removeolddraw();
        lastx = x; //always preserve value before moving
        lasty = y; //to ensure that undrawing works.
        lastrot=rot;
        if(collides(x,y,rot+1)) {
            rot = (rot+1)%4;
        }
        printf("current rot: %i last rot: %i\n",rot, lastrot);
        redraw();
    }
}
void tetrimino::draw() {
    //Pieces[piece][rot][actual data here]
    removeolddraw();
    redraw();
    lastx = x; //always preserve value before moving
    lasty = y; //to ensure that undrawing works.
    lastrot=rot;

}

void tetrimino::removeolddraw() {
    for(int i = 0; i < 16; i++) {
        int ycalc = (i/4)*width;
        if(Pieces[piece][lastrot][i] > 0) {
            array[lastx + (lasty*width) + i%4 + ycalc] = 0;
        }
    }

}

void tetrimino::redraw() {
    for(int i = 0; i < 16; i++) {
        int ycalc = (i/4)*width;
        if(Pieces[piece][rot][i] > 0) {
            array[x + (y*width) + i%4 + ycalc] = Pieces[piece][rot][i];
        }

    }

}