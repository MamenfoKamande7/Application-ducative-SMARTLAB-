#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void drawText(SDL_Renderer *r, TTF_Font *f, const char *t, int x, int y);
void drawCircle(SDL_Renderer *r, int cx, int cy, int rad);

#endif