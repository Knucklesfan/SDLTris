#pragma once

class tetrimino {

    public:
        int x, y, lastx, lasty, rot, lastrot, width, height, piece;
        int* array;
        bool alive;

        //bool alive;
	    //int x,y,lastx,lasty,rot,lastrot,width,height;
        //int* array;
        tetrimino();
        tetrimino(int, int, int (barray)[], int bwidth, int bheight, int block);
        bool rebirth(int, int, int block);
        int forcedrop();
	    void movedown();
        void moveleft();
	    void moveright();
        void hold();
        void rotate();
        void draw();
        bool collides(int x, int y, int rot);
        int* getPieces();
        void removeolddraw();
    private:
        void redraw();
};
