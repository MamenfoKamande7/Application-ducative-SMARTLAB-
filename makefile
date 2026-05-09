CC = gcc

CFLAGS = -Iinclude -Wall -Wextra

LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lm

SRC = \
src/main.c \
src/ui/draw.c \
src/modules/physique/pendule.c \
src/modules/physique/ressort.c \
src/modules/physique/physics_menu.c \
src/modules/physique/physics_controller.c \
src/modules/maths/graphe.c \
src/modules/maths/maths_menu.c \
src/modules/maths/maths_controller.c

OBJ = $(SRC:src/%.c=build/%.o)

TARGET = bin/smartlab.exe

# =====================================================
# BUILD
# =====================================================

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# =====================================================
# OBJECTS
# =====================================================

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# =====================================================
# RUN
# =====================================================

run: all
	./$(TARGET)

# =====================================================
# CLEAN
# =====================================================

clean:
	rm -rf build bin

# =====================================================
# PHONY
# =====================================================

.PHONY: all run clean