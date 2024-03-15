OBJS	= SDLTetris.o tetrimino.o ghostblock.o titlescreen.o background.o ingamemessagebox.o knuxfanscreen.o highscore.o replay.o cube.o sine.o SDL_FontCache.o globalgamemode.o defs.o pixfont.o ttffont.o texture.o shader.o plane.o rect.o buffermanager.o sprite.o stbi_image.o animation.o line.o game.o results.o
SOURCE	= source/SDLTetris.cpp source/tetrimino.cpp source/ghostblock.cpp source/titlescreen.cpp source/game.cpp source/background.cpp source/ingamemessagebox.cpp source/knuxfanscreen.cpp source/server.cpp source/highscore.cpp source/replay.cpp source/results.cpp source/font.cpp source/options.cpp source/cube.cpp source/credits.cpp source/sine.cpp source/animation.cpp source/opengl/line.cpp source/scenes/game.cpp source/scenes/results.cpp
HEADER	= 
OUT	= SDLTetris
CC	 = g++
FLAGS	 = $(INC) -g -c -Wall -O0 -D _LINUX -DCLIENT
LFLAGS	 = -lSDL2 -lGLEW -lGL -lSDL2_ttf -lSDL2_mixer -lSDL2_image
INC=-I./include/

LFLAGS += -L/opt/homebrew/lib

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

#removed for now!
game.o: source/scenes/game.cpp
	$(CC) $(FLAGS) source/scenes/game.cpp -std=c++17

background.o: source/background.cpp
	$(CC) $(FLAGS) source/background.cpp -std=c++17

ingamemessagebox.o: source/ingamemessagebox.cpp
	$(CC) $(FLAGS) source/ingamemessagebox.cpp -std=c++17

knuxfanscreen.o: source/scenes/knuxfanscreen.cpp
	$(CC) $(FLAGS) source/scenes/knuxfanscreen.cpp -std=c++17

# server.o: source/server.cpp
# 	$(CC) $(FLAGS) source/server.cpp -std=c++17

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

# options.o: source/scenes/options.cpp
# 	$(CC) $(FLAGS) source/scenes/options.cpp -std=c++17

cube.o: source/opengl/cube.cpp
	$(CC) $(FLAGS) source/opengl/cube.cpp -std=c++17
	
# credits.o: source/scenes/credits.cpp
# 	$(CC) $(FLAGS) source/scenes/credits.cpp -std=c++17

sine.o: source/sine.cpp
	$(CC) $(FLAGS) source/sine.cpp -std=c++17
SDL_FontCache.o: source/SDL_FontCache/SDL_FontCache.c
	$(CC) $(FLAGS) source/SDL_FontCache/SDL_FontCache.c 
globalgamemode.o: source/globalgamemode.cpp
	$(CC) $(FLAGS) source/globalgamemode.cpp
shader.o: source/opengl/shader.cpp
	$(CC) $(FLAGS) source/opengl/shader.cpp
texture.o: source/opengl/texture.cpp
	$(CC) $(FLAGS) source/opengl/texture.cpp
plane.o: source/opengl/plane.cpp
	$(CC) $(FLAGS) source/opengl/plane.cpp
rect.o: source/opengl/rect.cpp
	$(CC) $(FLAGS) source/opengl/rect.cpp
buffermanager.o: source/opengl/buffermanager.cpp
	$(CC) $(FLAGS) source/opengl/buffermanager.cpp
sprite.o: source/opengl/sprite.cpp
	$(CC) $(FLAGS) source/opengl/sprite.cpp
stbi_image.o: source/opengl/stbi_image.c
	$(CC) $(FLAGS) source/opengl/stbi_image.c
animation.o: source/utils/animation.cpp
	$(CC) $(FLAGS) source/utils/animation.cpp
defs.o: source/utils/defs.cpp
	$(CC) $(FLAGS) source/utils/defs.cpp 
line.o: source/opengl/line.cpp
	$(CC) $(FLAGS) source/opengl/line.cpp

clean:
	rm -f $(OBJS) $(OUT)

