OBJS	= SDLTetris.o tetrimino.o ghostblock.o titlescreen.o game.o background.o ingamemessagebox.o knuxfanscreen.o server.o highscore.o replay.o results.o options.o cube.o credits.o sine.o SDL_FontCache.o globalgamemode.o defs.o pixfont.o ttffont.o
SOURCE	= source/SDLTetris.cpp source/tetrimino.cpp source/ghostblock.cpp source/titlescreen.cpp source/game.cpp source/background.cpp source/ingamemessagebox.cpp source/knuxfanscreen.cpp source/server.cpp source/highscore.cpp source/replay.cpp source/results.cpp source/font.cpp source/options.cpp source/cube.cpp source/credits.cpp source/sine.cpp
HEADER	= 
OUT	= SDLTetris
CC	 = g++
FLAGS	 = $(INC) -g -c -Wall -fsanitize=address -O0 -D _LINUX
LFLAGS	 = -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image -fsanitize=address
INC=-I include/

all: $(OBJS)
	$(CC) -g $(OBJS)  -o $(OUT) $(LFLAGS)

SDLTetris.o: source/SDLTetris.cpp
	$(CC) $(FLAGS) source/SDLTetris.cpp -std=c++17

tetrimino.o: source/tetrimino.cpp
	$(CC) $(FLAGS) source/tetrimino.cpp -std=c++17

ghostblock.o: source/ghostblock.cpp
	$(CC) $(FLAGS) source/ghostblock.cpp -std=c++17

titlescreen.o: source/scenes/titlescreen.cpp
	$(CC) $(FLAGS) source/scenes/titlescreen.cpp -std=c++17

game.o: source/scenes/game.cpp
	$(CC) $(FLAGS) source/scenes/game.cpp -std=c++17

background.o: source/background.cpp
	$(CC) $(FLAGS) source/background.cpp -std=c++17

ingamemessagebox.o: source/ingamemessagebox.cpp
	$(CC) $(FLAGS) source/ingamemessagebox.cpp -std=c++17

knuxfanscreen.o: source/scenes/knuxfanscreen.cpp
	$(CC) $(FLAGS) source/scenes/knuxfanscreen.cpp -std=c++17

server.o: source/server.cpp
	$(CC) $(FLAGS) source/server.cpp -std=c++17

highscore.o: source/highscore.cpp
	$(CC) $(FLAGS) source/highscore.cpp -std=c++17

replay.o: source/replay.cpp
	$(CC) $(FLAGS) source/replay.cpp -std=c++17

results.o: source/scenes/results.cpp
	$(CC) $(FLAGS) source/scenes/results.cpp -std=c++17

pixfont.o: source/pixfont.cpp
	$(CC) $(FLAGS) source/pixfont.cpp 
ttffont.o: source/ttffont.cpp
	$(CC) $(FLAGS) source/ttffont.cpp 

options.o: source/scenes/options.cpp
	$(CC) $(FLAGS) source/scenes/options.cpp -std=c++17

cube.o: source/cube.cpp
	$(CC) $(FLAGS) source/cube.cpp -std=c++17
	
credits.o: source/scenes/credits.cpp
	$(CC) $(FLAGS) source/scenes/credits.cpp -std=c++17

sine.o: source/sine.cpp
	$(CC) $(FLAGS) source/sine.cpp -std=c++17
SDL_FontCache.o: source/SDL_FontCache/SDL_FontCache.c
	$(CC) $(FLAGS) source/SDL_FontCache/SDL_FontCache.c 
globalgamemode.o: source/globalgamemode.cpp
	$(CC) $(FLAGS) source/globalgamemode.cpp

defs.o: source/utils/defs.cpp
	$(CC) $(FLAGS) source/utils/defs.cpp 
clean:
	rm -f $(OBJS) $(OUT)

