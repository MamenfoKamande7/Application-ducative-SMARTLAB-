CC = gcc

# =========================
# FLAGS
# =========================
CFLAGS = -Wall -Wextra -std=c11 -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf -lm

# =========================
# DOSSIERS
# =========================
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# =========================
# SOURCES
# =========================
SRC =  $(SRC_DIR)/main.c \
       $(SRC_DIR)/ui/draw.c \
       $(SRC_DIR)/modules/physique/pendule.c \
       $(SRC_DIR)/modules/physique/ressort.c \
       $(SRC_DIR)/modules/physique/physics_menu.c \
       $(SRC_DIR)/modules/physique/physics_controller.c \
       $(SRC_DIR)/modules/maths/graphe.c \
       $(SRC_DIR)/modules/maths/maths_menu.c \
       $(SRC_DIR)/modules/maths/maths_controller.c

# =========================
# OBJETS (dans build/)
# =========================
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# =========================
# EXECUTABLE UNIQUE
# =========================
NAME = $(BIN_DIR)/smartlab.exe

# =========================
# REGLE PRINCIPALE
# =========================
all: $(NAME)

# création exécutable
$(NAME): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

# compilation des .c → .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# =========================
# NETTOYAGE
# =========================
clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

re: fclean all
