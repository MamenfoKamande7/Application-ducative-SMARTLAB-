#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ui/draw.h"
#include "modules/maths/maths_menu.h"

int maths_menu(SDL_Renderer *ren, SDL_Window *win)
{
    (void)win;

    TTF_Font *font = TTF_OpenFont("assets/fonts/ariblk.ttf", 22);

    SDL_Rect btnGraph = {350, 250, 300, 70};

    SDL_Event e;

    while(1)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                return MATHS_NONE;

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx > btnGraph.x && mx < btnGraph.x + btnGraph.w &&
                   my > btnGraph.y && my < btnGraph.y + btnGraph.h)
                {
                    return MATHS_GRAPHE;
                }
            }
        }

        SDL_SetRenderDrawColor(ren,20,20,30,255);
        SDL_RenderClear(ren);

        drawText(ren,font,"SMARTLAB - MATHS",300,100);

        SDL_SetRenderDrawColor(ren,70,150,70,255);
        SDL_RenderFillRect(ren,&btnGraph);
        drawText(ren,font,"Graphe",btnGraph.x+100,btnGraph.y+20);

        SDL_RenderPresent(ren);
    }
}