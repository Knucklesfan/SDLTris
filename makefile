#OBJS specifies which files to compile as part of the project
OBJS = source/SDLTetris.cpp
#CC specifies which compiler we're using
CC = g++
#Compiler flags supresses the annoying warnings
COMPILER_FLAGS = -w
#Linker Flags give the libraries
LINKER_FLAGS = -lSDL2
#Specify c++17 as well.
CPP_VER = -std=c++17
#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = SDLTetris

#This is the target that compiles our executable
all: $(OBJS)
	$(CC) $(CPP_VER) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	@$(MAKE) && ./$(OBJ_NAME)
