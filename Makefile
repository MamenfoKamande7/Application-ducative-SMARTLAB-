# ===================== COMPILATEUR & FLAGS =====================
CC     = gcc
CFLAGS = -Wall -Wextra -Iinclude \
         $(shell sdl2-config --cflags)
LIBS   = $(shell sdl2-config --libs) -lSDL2_ttf

# ===================== DOSSIERS =====================
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = bin/obj

# ===================== FICHIERS SOURCE =====================
SRCS = src/main.c                    \
       src/auth/auth.c               \
       src/auth/auth_session.c       \
       src/auth/auth_validation.c    \
       src/ui/ui.c                   \
       src/ui/menu.c                 \
       src/modules/maths.c           \
       src/modules/physique.c        \
       src/modules/chimie.c          \
       src/modules/scores.c

# ===================== FICHIERS OBJETS =====================
OBJS = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# ===================== EXÉCUTABLE =====================
TARGET = $(BIN_DIR)/smartlab

# ===================== RÈGLE PRINCIPALE =====================
all: dirs $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)
	@echo "✅ Compilation réussie : $(TARGET)"

# ===================== COMPILATION DES .o =====================
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ===================== CRÉATION DES DOSSIERS =====================
dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)/auth
	@mkdir -p $(OBJ_DIR)/ui
	@mkdir -p $(OBJ_DIR)/modules

# ===================== NETTOYAGE =====================
clean:
	rm -rf $(OBJ_DIR)
	@echo "🧹 Objets supprimés"

fclean: clean
	rm -f $(TARGET)
	@echo "🧹 Exécutable supprimé"

re: fclean all

# ===================== LANCEMENT =====================
run: all
	./$(TARGET)

.PHONY: all clean fclean re run dirs