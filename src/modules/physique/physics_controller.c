#include "modules/physique/physics_controller.h"
#include "modules/physique/physics_menu.h"
#include "modules/physique/pendule.h"
#include "modules/physique/ressort.h"

void physics_controller(SDL_Renderer *ren, SDL_Window *win)
{
    int choice = physics_menu(ren, win);

    switch(choice)
    {
        case MENU_PENDULE:
            simulation_pendule(ren, win);
            break;

        case MENU_RESSORT:
            simulation_ressort(ren, win);
            break;

        default:
            return;
    }
}