#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "ui/draw.h"
#include "modules/maths/graphe.h"

// ================= TYPES =================
typedef enum {
    FUNC_NONE,
    FUNC_X,
    FUNC_X2,
    FUNC_SIN,
    FUNC_COS,
    FUNC_INV
} FunctionType;

typedef struct {
    FunctionType type;
} Function;

// ================= GLOBAL =================
static float zoom = 100;
static float offsetX = 0, offsetY = 0;
static int dragging = 0, lastX, lastY;

static char input[100] = "x^2";
static Function current;

// ================= PARSE =================
Function parse(char *s)
{
    Function f;
    if(strcmp(s,"x")==0) f.type=FUNC_X;
    else if(strcmp(s,"x^2")==0) f.type=FUNC_X2;
    else if(strcmp(s,"sin(x)")==0) f.type=FUNC_SIN;
    else if(strcmp(s,"cos(x)")==0) f.type=FUNC_COS;
    else if(strcmp(s,"1/x")==0) f.type=FUNC_INV;
    else f.type=FUNC_NONE;
    return f;
}

// ================= CALCUL =================
float compute(Function f, float x)
{
    switch(f.type)
    {
        case FUNC_X: return x;
        case FUNC_X2: return x*x;
        case FUNC_SIN: return sinf(x);
        case FUNC_COS: return cosf(x);
        case FUNC_INV: return (x!=0)?1/x:0;
        default: return 0;
    }
}

// dérivée numérique
float derivative(Function f, float x)
{
    float h = 0.001f;
    return (compute(f,x+h)-compute(f,x-h))/(2*h);
}

// ================= INFOS =================
const char* domain(Function f)
{
    if(f.type==FUNC_INV) return "R - {0}";
    return "R";
}

const char* asymptote(Function f)
{
    if(f.type==FUNC_INV) return "x=0 , y=0";
    return "Aucune";
}

float limit_left(Function f)
{
    if(f.type==FUNC_INV) return -INFINITY;
    return compute(f,-1000);
}

float limit_right(Function f)
{
    if(f.type==FUNC_INV) return INFINITY;
    return compute(f,1000);
}

// ================= INTERSECTIONS =================
void draw_intersections(SDL_Renderer *ren, Function f, int cx, int cy)
{
    SDL_SetRenderDrawColor(ren,255,0,0,255);

    for(float x=-10; x<10; x+=0.1f)
    {
        float y = compute(f,x);
        if(fabs(y) < 0.05)
        {
            int px = cx + x*zoom;
            int py = cy;
            SDL_RenderDrawPoint(ren,px,py);
        }
    }
}

// ================= VARIATION =================
void draw_variation(SDL_Renderer *ren, TTF_Font *font, Function f)
{
    int x0 = 20, y0 = 500;

    drawText(ren,font,"Variation :",x0,y0);

    for(int i=0;i<10;i++)
    {
        float x = -5 + i;
        float d = derivative(f,x);

        char buf[50];
        sprintf(buf,"x=%.1f : %s",x,(d>0)?"croissant":"decroissant");

        drawText(ren,font,buf,x0,y0+20+20*i);
    }
}

// ================= MAIN =================
void simulation_graphe(SDL_Renderer *ren, SDL_Window *win)
{
    (void)win;

    TTF_Font *font = TTF_OpenFont("assets/fonts/ariblk.ttf",16);
    SDL_StartTextInput();

    SDL_Event e;
    int running = 1;

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT) return;

            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_ESCAPE) return;

                if(e.key.keysym.sym == SDLK_BACKSPACE && strlen(input)>0)
                    input[strlen(input)-1]=0;

                if(e.key.keysym.sym == SDLK_RETURN)
                    current = parse(input);
            }

            if(e.type == SDL_TEXTINPUT)
                strcat(input,e.text.text);

            // zoom
            if(e.type == SDL_MOUSEWHEEL)
            {
                if(e.wheel.y>0) zoom*=1.1f;
                if(e.wheel.y<0) zoom*=0.9f;
            }

            // drag
            if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                dragging=1;
                lastX=e.button.x;
                lastY=e.button.y;
            }
            if(e.type == SDL_MOUSEBUTTONUP) dragging=0;

            if(e.type == SDL_MOUSEMOTION && dragging)
            {
                offsetX += e.motion.x-lastX;
                offsetY += e.motion.y-lastY;
                lastX=e.motion.x;
                lastY=e.motion.y;
            }
        }

        int cx = 500 + offsetX;
        int cy = 300 + offsetY;

        SDL_SetRenderDrawColor(ren,10,10,20,255);
        SDL_RenderClear(ren);

        // axes
        SDL_SetRenderDrawColor(ren,200,200,200,255);
        SDL_RenderDrawLine(ren,0,cy,1000,cy);
        SDL_RenderDrawLine(ren,cx,0,cx,700);

        // courbe
        SDL_SetRenderDrawColor(ren,0,255,0,255);

        for(int x=0;x<1000;x++)
        {
            float t = (x-cx)/zoom;
            float y = compute(current,t);
            int py = cy - y*zoom;
            SDL_RenderDrawPoint(ren,x,py);
        }

        // tangente au centre
        float a = 0;
        SDL_SetRenderDrawColor(ren,255,255,0,255);
        for(int x=0;x<1000;x++)
        {
            float t = (x-cx)/zoom;
            float y = derivative(current,a)*(t-a)+compute(current,a);
            int py = cy - y*zoom;
            SDL_RenderDrawPoint(ren,x,py);
        }

        // intersections
        draw_intersections(ren,current,cx,cy);

        // infos
        drawText(ren,font,input,20,20);
        drawText(ren,font,domain(current),20,50);
        drawText(ren,font,asymptote(current),20,80);

        // limites
        char buf[100];
        sprintf(buf,"lim gauche: %.2f",limit_left(current));
        drawText(ren,font,buf,20,110);

        sprintf(buf,"lim droite: %.2f",limit_right(current));
        drawText(ren,font,buf,20,140);

        // variation
        draw_variation(ren,font,current);

        SDL_RenderPresent(ren);
    }
}