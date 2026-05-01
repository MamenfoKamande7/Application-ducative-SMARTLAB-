# ================= CONFIG =================
CC = gcc

CFLAGS = -Wall -Wextra -Iinclude `pkg-config --cflags sdl2 SDL2_ttf`
LDFLAGS = `pkg-config --libs sdl2 SDL2_ttf`

# ================= DOSSIERS =================
SRC = src
BUILD = build
BIN = bin

# ================= SOURCES =================
SOURCES = \
$(SRC)/main.c \
$(SRC)/menu.c \
$(SRC)/ui/draw.c \
$(SRC)/modules/physique/pendule.c \
$(SRC)/modules/physique/ressort.c

# ================= OBJETS =================
OBJECTS = $(SOURCES:$(SRC)/%.c=$(BUILD)/%.o)

# ================= EXEC =================
EXEC = $(BIN)/smartlab

# ================= RULES =================

all: $(EXEC)

$(EXEC): $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $^ -o $@ $(LDFLAGS)

# Compilation .c -> .o
$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(BUILD)

fclean: clean
	rm -rf $(BIN)

re: fclean all

run: all
	./$(EXEC)
