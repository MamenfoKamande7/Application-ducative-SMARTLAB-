#include <SDL2/SDL.h>
#include <math.h>

#include "ui/draw.h"
#include "modules/physique/cylindrique.h"

void simulation_cylindrique(SDL_Renderer *ren, SDL_Window *win){

    float theta = 0;
    float r = 150;

    int run = 1;
    SDL_Event e;

    Uint32 last = SDL_GetTicks();

    while(run){

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) return;

            if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                run = 0;
        }

        Uint32 now = SDL_GetTicks();
        float dt = (now-last)/1000.0f;
        last = now;

        theta += 2*dt;

        int x = 500 + (int)(r*cos(theta));
        int y = 300 + (int)(r*sin(theta));

        SDL_SetRenderDrawColor(ren,10,10,20,255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren,255,200,100,255);
        SDL_RenderDrawLine(ren,500,300,x,y);

        SDL_RenderPresent(ren);
    }
}
