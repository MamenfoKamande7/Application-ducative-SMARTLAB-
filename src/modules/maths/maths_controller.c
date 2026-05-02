#include "modules/maths/maths_controller.h"
#include "modules/maths/maths_menu.h"
#include "modules/maths/graphe.h"

void maths_controller(SDL_Renderer *ren, SDL_Window *win)
{
    int choice = maths_menu(ren, win);

    switch(choice)
    {
        case MATHS_GRAPHE:
            simulation_graphe(ren, win);
            break;

        default:
            return;
    }
}