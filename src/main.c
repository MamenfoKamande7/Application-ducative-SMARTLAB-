#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void run_menu(SDL_Renderer *ren, SDL_Window *win);

int main(int argc, char* argv[]){

    (void)argc;
    (void)argv;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *win = SDL_CreateWindow(
        "SmartLab",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000, 700,
        0
    );

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    run_menu(ren, win);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
