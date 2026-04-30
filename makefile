CC = gcc

CFLAGS = -Wall -Iinclude

LIBS = `pkg-config --cflags --libs sdl2 SDL2_ttf`

SRC = src/main.c \
      src/modules/physique/physique.c \
      src/modules/physique/pendule.c \
      src/modules/physique/cylindrique.c \
      src/ui/draw.c

OUT = bin/app.exe

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LIBS)

run: all
	./bin/app.exe

clean:
	rm -f $(OUT)
