#include "ui/draw.h"

// ================= TEXT =================
void drawText(SDL_Renderer *r, TTF_Font *f, const char *t, int x, int y){

    SDL_Color c = {255,255,255};

    SDL_Surface *s = TTF_RenderText_Solid(f,t,c);
    SDL_Texture *tx = SDL_CreateTextureFromSurface(r,s);

    SDL_Rect d = {x,y,s->w,s->h};
    SDL_RenderCopy(r,tx,NULL,&d);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(tx);
}

// ================= CIRCLE =================
void drawCircle(SDL_Renderer *r,int cx,int cy,int rad){

    for(int x=-rad;x<=rad;x++){
        for(int y=-rad;y<=rad;y++){
            if(x*x + y*y <= rad*rad){
                SDL_RenderDrawPoint(r,cx+x,cy+y);
            }
        }
    }
}