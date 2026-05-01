#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void drawText(SDL_Renderer *r, TTF_Font *font, const char *text, int x, int y);
void drawCircle(SDL_Renderer *r, int cx, int cy, int radius);
void drawArrow(SDL_Renderer *r, int x1, int y1, int x2, int y2);

#endif