#include "ui/draw.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI
#include <math.h>

void drawText(SDL_Renderer *r, TTF_Font *font, const char *text, int x, int y){
    SDL_Color color = {255,255,255,255};
    SDL_Surface *surf = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(r, surf);

    SDL_Rect dst = {x, y, surf->w, surf->h};
    SDL_RenderCopy(r, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void drawCircle(SDL_Renderer *r, int cx, int cy, int radius){
    for(int x=-radius; x<=radius; x++)
        for(int y=-radius; y<=radius; y++)
            if(x*x + y*y <= radius*radius)
                SDL_RenderDrawPoint(r, cx+x, cy+y);
}

void drawArrow(SDL_Renderer *r, int x1, int y1, int x2, int y2){

    SDL_RenderDrawLine(r,x1,y1,x2,y2);

    float a = atan2f(y2-y1, x2-x1);
    float len = 10;

    int x3 = x2 + len*cosf(a + M_PI - 0.5f);
    int y3 = y2 + len*sinf(a + M_PI - 0.5f);

    int x4 = x2 + len*cosf(a + M_PI + 0.5f);
    int y4 = y2 + len*sinf(a + M_PI + 0.5f);

    SDL_RenderDrawLine(r,x2,y2,x3,y3);
    SDL_RenderDrawLine(r,x2,y2,x4,y4);
}
