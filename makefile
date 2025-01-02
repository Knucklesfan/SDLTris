OBJS	= SDLTetris.o tetrimino.o ghostblock.o titlescreen.o background.o ingamemessagebox.o knuxfanscreen.o cube.o sine.o globalgamemode.o defs.o pixfont.o ttffont.o texture.o shader.o plane.o rect.o buffermanager.o sprite.o stbi_image.o animation.o line.o game.o results.o credits.o wireframecube.o debugscene.o gameplaymenu.o skybox.o model.o classicmenu.o rpcimplement.o sota.o tpp_assert.o tpp_impl.o keyboard.o highscore.o
DISCORDOBJS = achievement_manager.o activity_manager.o application_manager.o core.o image_manager.o lobby_manager.o network_manager.o overlay_manager.o relationship_manager.o storage_manager.o store_manager.o types.o user_manager.o voice_manager.o $(OBJS)
HEADER	= 
OUT	= SDLTetris
CC	 = g++
FLAGS	 = $(INC) -g -c -Wall -O0 -D _LINUX -DCLIENT
LFLAGS	 = -Wl,-Bstatic -lSDL2_mixer_ext  -lopusfile -lopus -ltimidity_sdl2 -logg -lwavpack -lmodplug -lEDMIDI -lzlib -Wl,-Bdynamic -lGL -lpthread -lm -ldl -static-libgcc -lstdc++ -lGLEW  -lSDL2 -lSDL2_image -lfreetype
DISCORDFLAGS	= $(LFLAGS) ./libdiscord_game_sdk.so

INC=-I./include/ -I/usr/include/freetype2

LFLAGS += -L./lib/
all: $(DISCORDOBJS)
	$(CC) -g $(DISCORDOBJS)  -o $(OUT) $(DISCORDFLAGS)

nodiscord: $(OBJS)
	$(CC) -g $(OBJS)  -o $(OUT) $(LFLAGS)

SDLTetris.o: source/SDLTetris.cpp
	$(CC) $(FLAGS) source/SDLTetris.cpp -std=c++17

tetrimino.o: source/gameplay/tetrimino.cpp
	$(CC) $(FLAGS) source/gameplay/tetrimino.cpp -std=c++17

ghostblock.o: source/gameplay/ghostblock.cpp
	$(CC) $(FLAGS) source/gameplay/ghostblock.cpp -std=c++17

titlescreen.o: source/scenes/titlescreen.cpp
	$(CC) $(FLAGS) source/scenes/titlescreen.cpp -std=c++17

#removed for now!
game.o: source/scenes/game.cpp
	$(CC) $(FLAGS) source/scenes/game.cpp -std=c++17

background.o: source/opengl/background.cpp
	$(CC) $(FLAGS) source/opengl/background.cpp -std=c++17

ingamemessagebox.o: source/gameplay/ingamemessagebox.cpp
	$(CC) $(FLAGS) source/gameplay/ingamemessagebox.cpp -std=c++17

knuxfanscreen.o: source/scenes/knuxfanscreen.cpp
	$(CC) $(FLAGS) source/scenes/knuxfanscreen.cpp -std=c++17

gameplaymenu.o: source/scenes/gameplaymenu.cpp
	$(CC) $(FLAGS) source/scenes/gameplaymenu.cpp -std=c++17
classicmenu.o: source/scenes/classicmenu.cpp
	$(CC) $(FLAGS) source/scenes/classicmenu.cpp -std=c++17
highscore.o: source/scenes/highscore.cpp
	$(CC) $(FLAGS) source/scenes/highscore.cpp -std=c++17

# server.o: source/server.cpp
# 	$(CC) $(FLAGS) source/server.cpp -std=c++17

results.o: source/scenes/results.cpp
	$(CC) $(FLAGS) source/scenes/results.cpp -std=c++17

pixfont.o: source/opengl/pixfont.cpp
	$(CC) $(FLAGS) source/opengl/pixfont.cpp 
ttffont.o: source/opengl/ttffont.cpp
	$(CC) $(FLAGS) source/opengl/ttffont.cpp 

wireframecube.o: source/opengl/wireframecube.cpp
	$(CC) $(FLAGS) source/opengl/wireframecube.cpp -std=c++17

# options.o: source/scenes/options.cpp
# 	$(CC) $(FLAGS) source/scenes/options.cpp -std=c++17

cube.o: source/opengl/cube.cpp
	$(CC) $(FLAGS) source/opengl/cube.cpp -std=c++17
	
credits.o: source/scenes/credits.cpp
	$(CC) $(FLAGS) source/scenes/credits.cpp -std=c++17

debugscene.o: source/scenes/debugscene.cpp
	$(CC) $(FLAGS) source/scenes/debugscene.cpp -std=c++17

sine.o: source/opengl/sine.cpp
	$(CC) $(FLAGS) source/opengl/sine.cpp -std=c++17
globalgamemode.o: source/engine/globalgamemode.cpp
	$(CC) $(FLAGS) source/engine/globalgamemode.cpp
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
skybox.o: source/opengl/skybox.cpp
	$(CC) $(FLAGS) source/opengl/skybox.cpp
model.o: source/opengl/model.cpp
	$(CC) $(FLAGS) source/opengl/model.cpp -std=c++17

rpcimplement.o: source/engine/rpcimplement.cpp
	$(CC) $(FLAGS) source/engine/rpcimplement.cpp -std=c++17

achievement_manager.o: source/discord/achievement_manager.cpp
	$(CC) $(FLAGS) source/discord/achievement_manager.cpp
activity_manager.o: source/discord/activity_manager.cpp
	$(CC) $(FLAGS) source/discord/activity_manager.cpp
application_manager.o: source/discord/application_manager.cpp
	$(CC) $(FLAGS) source/discord/application_manager.cpp
core.o: source/discord/core.cpp
	$(CC) $(FLAGS) source/discord/core.cpp
image_manager.o: source/discord/image_manager.cpp
	$(CC) $(FLAGS) source/discord/image_manager.cpp
lobby_manager.o: source/discord/lobby_manager.cpp
	$(CC) $(FLAGS) source/discord/lobby_manager.cpp
network_manager.o: source/discord/network_manager.cpp
	$(CC) $(FLAGS) source/discord/network_manager.cpp
overlay_manager.o: source/discord/overlay_manager.cpp
	$(CC) $(FLAGS) source/discord/overlay_manager.cpp
relationship_manager.o: source/discord/relationship_manager.cpp
	$(CC) $(FLAGS) source/discord/relationship_manager.cpp
storage_manager.o: source/discord/storage_manager.cpp
	$(CC) $(FLAGS) source/discord/storage_manager.cpp
store_manager.o: source/discord/store_manager.cpp
	$(CC) $(FLAGS) source/discord/store_manager.cpp
types.o: source/discord/types.cpp
	$(CC) $(FLAGS) source/discord/types.cpp
user_manager.o: source/discord/user_manager.cpp
	$(CC) $(FLAGS) source/discord/user_manager.cpp
voice_manager.o: source/discord/voice_manager.cpp
	$(CC) $(FLAGS) source/discord/voice_manager.cpp

tpp_impl.o: source/opengl/triangle/tpp_impl.cpp
	$(CC) $(FLAGS) source/opengl/triangle/tpp_impl.cpp
	
tpp_assert.o: source/opengl/triangle/tpp_assert.cpp
	$(CC) $(FLAGS) source/opengl/triangle/tpp_assert.cpp
sota.o: source/opengl/sota.cpp
	$(CC) $(FLAGS) source/opengl/sota.cpp
keyboard.o: source/gameplay/keyboard.cpp
	$(CC) $(FLAGS) source/gameplay/keyboard.cpp

clean:
	rm -f $(OBJS) $(OUT)

