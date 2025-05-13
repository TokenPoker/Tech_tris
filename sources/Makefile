# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# SDL2 and related libraries
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Source files
SRC = main.c game.c pieces.c score.c render.c key.c
OBJ = $(SRC:.c=.o)

# Executable name
EXEC = techtris

# Build rules
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(SDL2_LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(SDL2_CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC)

run: all
	./$(EXEC)