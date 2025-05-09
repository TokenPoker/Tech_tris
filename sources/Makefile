# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g `sdl2-config --cflags` -I.
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf

# Source files
SRC = main.c game.c pieces.c score.c
OBJ = $(SRC:.c=.o)
EXEC = tetris

# Default rule
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean object files and executable
clean:
	rm -f $(OBJ) $(EXEC)

# Run the game
run: $(EXEC)
	./$(EXEC)
