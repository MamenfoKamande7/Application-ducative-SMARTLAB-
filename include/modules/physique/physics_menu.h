#ifndef PHYSICS_MENU_H
#define PHYSICS_MENU_H

#include <SDL2/SDL.h>

#define MENU_NONE    0
#define MENU_PENDULE 1
#define MENU_RESSORT 2

int physics_menu(SDL_Renderer *ren, SDL_Window *win);

#endif