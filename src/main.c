#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "modules/maths/maths_controller.h"
#include "ui/draw.h"
#include "modules/physique/physics_controller.h"

// ================= INIT =================
int init(SDL_Window **win, SDL_Renderer **ren)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
        return 0;

    if(TTF_Init() != 0)
        return 0;

    *win = SDL_CreateWindow("SMARTLAB",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1000, 700, SDL_WINDOW_SHOWN);

    if(!*win) return 0;

    *ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);
    if(!*ren) return 0;

    return 1;
}

// ================= MENU =================
void main_menu(SDL_Renderer *ren, SDL_Window *win)
{
    TTF_Font *font = TTF_OpenFont("assets/fonts/ariblk.ttf", 24);

    if(!font)
        printf("Erreur chargement police\n");

    SDL_Rect btnPhysics = {350, 250, 300, 80};
    SDL_Rect btnMaths   = {350, 370, 300, 80};

    SDL_Event e;
    int running = 1;

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                running = 0;

            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x;
                int my = e.button.y;

                if(mx > btnPhysics.x && mx < btnPhysics.x + btnPhysics.w &&
                   my > btnPhysics.y && my < btnPhysics.y + btnPhysics.h)
                {
                    physics_controller(ren, win);
                }

                if(mx > btnMaths.x && mx < btnMaths.x + btnMaths.w &&
                   my > btnMaths.y && my < btnMaths.y + btnMaths.h)
                {
                    maths_controller(ren,win);
                }
            }
        }

        SDL_SetRenderDrawColor(ren,20,20,30,255);
        SDL_RenderClear(ren);

        // boutons
        SDL_SetRenderDrawColor(ren,70,70,150,255);
        SDL_RenderFillRect(ren,&btnPhysics);

        SDL_SetRenderDrawColor(ren,150,70,70,255);
        SDL_RenderFillRect(ren,&btnMaths);

        // TEXTE sécurisé
        if(font)
        {
            drawText(ren,font,"PHYSIQUE",btnPhysics.x+80,btnPhysics.y+25);
            drawText(ren,font,"MATHS",btnMaths.x+100,btnMaths.y+25);
        }

        SDL_RenderPresent(ren);
    }

    if(font) TTF_CloseFont(font);
}

// ================= MAIN =================
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    SDL_Window *win = NULL;
    SDL_Renderer *ren = NULL;

    if(!init(&win,&ren)) return 1;

    main_menu(ren, win);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
