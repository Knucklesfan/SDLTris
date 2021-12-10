OBJS	= SDLTetris.o tetrimino.o ghostblock.o titlescreen.o game.o background.o ingamemessagebox.o knuxfanscreen.o server.o highscore.o replay.o results.o font.o options.o cube.o credits.o sine.o
SOURCE	= source/SDLTetris.cpp source/tetrimino.cpp source/ghostblock.cpp source/titlescreen.cpp source/game.cpp source/background.cpp source/ingamemessagebox.cpp source/knuxfanscreen.cpp source/server.cpp source/highscore.cpp source/replay.cpp source/results.cpp source/font.cpp source/options.cpp source/cube.cpp source/credits.cpp source/sine.cpp
HEADER	= 
OUT	= SDLTetris
CC	 = g++
FLAGS	 = $(INC) -g -c -Wall -fsanitize=address -O0 -D _LINUX
LFLAGS	 = -lSDL2 -lSDL2_ttf -lSDL2_mixer -fsanitize=address
INC=-I include/

all: $(OBJS)
	$(CC) -g $(OBJS)  -o $(OUT) $(LFLAGS)

SDLTetris.o: source/SDLTetris.cpp
	$(CC) $(FLAGS) source/SDLTetris.cpp -std=c++17

tetrimino.o: source/tetrimino.cpp
	$(CC) $(FLAGS) source/tetrimino.cpp -std=c++17

ghostblock.o: source/ghostblock.cpp
	$(CC) $(FLAGS) source/ghostblock.cpp -std=c++17

titlescreen.o: source/titlescreen.cpp
	$(CC) $(FLAGS) source/titlescreen.cpp -std=c++17

game.o: source/game.cpp
	$(CC) $(FLAGS) source/game.cpp -std=c++17

background.o: source/background.cpp
	$(CC) $(FLAGS) source/background.cpp -std=c++17

ingamemessagebox.o: source/ingamemessagebox.cpp
	$(CC) $(FLAGS) source/ingamemessagebox.cpp -std=c++17

knuxfanscreen.o: source/knuxfanscreen.cpp
	$(CC) $(FLAGS) source/knuxfanscreen.cpp -std=c++17

server.o: source/server.cpp
	$(CC) $(FLAGS) source/server.cpp -std=c++17

highscore.o: source/highscore.cpp
	$(CC) $(FLAGS) source/highscore.cpp -std=c++17

replay.o: source/replay.cpp
	$(CC) $(FLAGS) source/replay.cpp -std=c++17

results.o: source/results.cpp
	$(CC) $(FLAGS) source/results.cpp -std=c++17

font.o: source/font.cpp
	$(CC) $(FLAGS) source/font.cpp -std=c++17

options.o: source/options.cpp
	$(CC) $(FLAGS) source/options.cpp -std=c++17

cube.o: source/cube.cpp
	$(CC) $(FLAGS) source/cube.cpp -std=c++17
	
credits.o: source/credits.cpp
	$(CC) $(FLAGS) source/credits.cpp -std=c++17

sine.o: source/sine.cpp
	$(CC) $(FLAGS) source/sine.cpp -std=c++17

clean:
	rm -f $(OBJS) $(OUT)

