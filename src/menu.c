#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ui/draw.h"
#include "modules/physique/pendule.h"
#include "modules/physique/ressort.h"

void run_menu(SDL_Renderer *ren, SDL_Window *win){

    TTF_Font *font = TTF_OpenFont("assets/fonts/ariblk.ttf", 22);

    SDL_Rect btnPendule = {350, 200, 300, 70};
    SDL_Rect btnRessort = {350, 300, 300, 70};

    SDL_Event e;

    while(1){

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT)
                return;

            if(e.type == SDL_MOUSEBUTTONDOWN){

                int mx = e.button.x;
                int my = e.button.y;

                if(mx > btnPendule.x && mx < btnPendule.x + btnPendule.w &&
                   my > btnPendule.y && my < btnPendule.y + btnPendule.h){

                    simulation_pendule(ren, win);
                }

                if(mx > btnRessort.x && mx < btnRessort.x + btnRessort.w &&
                   my > btnRessort.y && my < btnRessort.y + btnRessort.h){

                    simulation_ressort(ren, win);
                }
            }
        }

        // ================= RENDER =================
        SDL_SetRenderDrawColor(ren,20,20,30,255);
        SDL_RenderClear(ren);

        drawText(ren, font, "SMARTLAB - PHYSIQUE", 300, 100);

        // bouton pendule
        SDL_SetRenderDrawColor(ren,70,70,150,255);
        SDL_RenderFillRect(ren,&btnPendule);
        drawText(ren, font, "Pendule", btnPendule.x + 90, btnPendule.y + 20);

        // bouton ressort
        SDL_SetRenderDrawColor(ren,150,70,70,255);
        SDL_RenderFillRect(ren,&btnRessort);
        drawText(ren, font, "Ressort", btnRessort.x + 90, btnRessort.y + 20);

        SDL_RenderPresent(ren);
    }
}