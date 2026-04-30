#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "ui/draw.h"
#include "modules/physique/pendule.h"
#include "modules/physique/cylindrique.h"




void module_physique_menu(){

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *win = SDL_CreateWindow("SmartLab Physique",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1000, 700, 0);

    SDL_Renderer *ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    TTF_Font *font = TTF_OpenFont("assets/fonts/ariblk.ttf",24);

    SDL_Rect btnPendule = {350,200,300,60};
    SDL_Rect btnCyl = {350,300,300,60};

    int running = 1;
    SDL_Event e;

    while(running){

        while(SDL_PollEvent(&e)){

            if(e.type == SDL_QUIT) running = 0;

            if(e.type == SDL_MOUSEBUTTONDOWN){

                int mx = e.button.x;
                int my = e.button.y;

                if(mx>=btnPendule.x && mx<=btnPendule.x+btnPendule.w &&
                   my>=btnPendule.y && my<=btnPendule.y+btnPendule.h){

                    simulation_pendule(ren, win);
                }

                if(mx>=btnCyl.x && mx<=btnCyl.x+btnCyl.w &&
                   my>=btnCyl.y && my<=btnCyl.y+btnCyl.h){

                    simulation_cylindrique(ren, win);
                }
            }
        }

        SDL_SetRenderDrawColor(ren,20,20,30,255);
        SDL_RenderClear(ren);

        drawText(ren,font,"MODULES PHYSIQUE",320,100);

        SDL_SetRenderDrawColor(ren,70,70,120,255);
        SDL_RenderFillRect(ren,&btnPendule);
        drawText(ren,font,"Pendule",430,215);

        SDL_RenderFillRect(ren,&btnCyl);
        drawText(ren,font,"Cylindrique",400,315);

        SDL_RenderPresent(ren);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
}
