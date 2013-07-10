CC = gcc
CFLAGS = -Wall -Wextra -I$(INC)
SRC = src
OBJ = obj
BIN = bin
INC = include
LIBS = -lGL -lSDLmain -lSDL -lSDL_image -lGLU -lopenal -lalut
OBJECTS =  $(OBJ)/main.o $(OBJ)/physics.o $(OBJ)/draw.o $(OBJ)/camera.o $(OBJ)/build.o $(OBJ)/frameControl.o $(OBJ)/init.o $(OBJ)/movement.o $(OBJ)/loader.o $(OBJ)/gameGrid.o $(OBJ)/random.o $(OBJ)/color.o $(OBJ)/play.o $(OBJ)/player.o $(OBJ)/bomb.o $(OBJ)/powerup.o $(OBJ)/ia.o $(OBJ)/configLoader.o $(OBJ)/list.o $(OBJ)/abp.o $(OBJ)/enemies.o
NAME = bombot

all: release

release: CFLAGS += -s -O3
release: main

debug: CFLAGS += -g
debug: main

main: $(OBJ) $(BIN) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(BIN)/$(NAME)

$(OBJ):
	mkdir $(OBJ)
$(BIN):
	mkdir $(BIN)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) $< -c -o $@
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -f $(OBJ)/*.o
	rm -f $(BIN)/$(NAME)
