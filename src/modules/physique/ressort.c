#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

#include "ui/draw.h"

#define CX 200
#define CY 300
#define GSIZE 400

// ================= PHYSIQUE =================
static float x = 350;
static float v = 0;

static float k = 0.5f;
static float m = 1.0f;
static float damping = 0.999f;

static int paused = 0;
static int holding = 0;

// ================= GRAPHE =================
static float Ec[GSIZE], Ep[GSIZE], Et[GSIZE];
static int gi = 0;

// ================= RESET =================
static void reset()
{
    x = 350;
    v = 0;
    gi = 0;
}

// ================= ENERGIES =================
static float kinetic()
{
    return 0.5f * m * v * v;
}
static float potential()
{
    return 0.5f * k * (x - CX)*(x - CX);
}

// ================= UPDATE =================
static void update(float dt)
{
    float a = -k/m * (x - CX);
    v += a * dt;
    x += v * dt;
    v *= damping;
}

// ================= RESSORT ZIGZAG =================
static void drawSpring(SDL_Renderer *r, int x1, int y1, int x2, int y2)
{

    (void)y2;
    int coils = 12;
    float dx = (x2 - x1) / (float)coils;

    for(int i = 0; i < coils; i++)
    {
        int xa = x1 + i * dx;
        int xb = x1 + (i + 1) * dx;

        int ya = y1 + (i % 2 == 0 ? -10 : 10);
        int yb = y1 + ((i + 1) % 2 == 0 ? -10 : 10);

        SDL_RenderDrawLine(r, xa, ya, xb, yb);
    }
}

// ================= GRAPHE =================
static void drawGraph(SDL_Renderer *r, TTF_Font *font)
{

    int gx = 20, gy = 400;
    int gw = 650, gh = 200;

    SDL_SetRenderDrawColor(r,30,30,50,255);
    SDL_RenderFillRect(r,&(SDL_Rect)
    {
        gx,gy,gw,gh
    });

    SDL_SetRenderDrawColor(r,200,200,200,255);
    SDL_RenderDrawLine(r,gx,gy+gh,gx+gw,gy+gh);
    SDL_RenderDrawLine(r,gx,gy,gx,gy+gh);

    drawText(r,font,"Temps",gx+gw-60,gy+gh+5);
    drawText(r,font,"Energie",gx,gy-20);

    float maxE = 2000.0f;

    for(int i=1; i<GSIZE; i++)
    {
        int a=(gi+i-1)%GSIZE;
        int b=(gi+i)%GSIZE;

        int x1=gx+(i-1)*(gw/GSIZE);
        int x2=gx+i*(gw/GSIZE);

        SDL_SetRenderDrawColor(r,255,80,80,255);
        SDL_RenderDrawLine(r,x1,gy+gh-(Ec[a]/maxE)*gh,x2,gy+gh-(Ec[b]/maxE)*gh);

        SDL_SetRenderDrawColor(r,80,255,80,255);
        SDL_RenderDrawLine(r,x1,gy+gh-(Ep[a]/maxE)*gh,x2,gy+gh-(Ep[b]/maxE)*gh);

        SDL_SetRenderDrawColor(r,80,150,255,255);
        SDL_RenderDrawLine(r,x1,gy+gh-(Et[a]/maxE)*gh,x2,gy+gh-(Et[b]/maxE)*gh);
    }
}

// ================= MAIN =================
void simulation_ressort(SDL_Renderer *ren, SDL_Window *win)
{

    (void)win;

    TTF_Font *font = TTF_OpenFont("assets/fonts/ariblk.ttf",16);

    SDL_Rect btnPause= {720,260,100,40};
    SDL_Rect btnReset= {840,260,100,40};
    SDL_Rect btnBack = {720,320,120,40};

    Uint32 last=SDL_GetTicks();
    SDL_Event e;

    reset();

    while(1)
    {

        while(SDL_PollEvent(&e))
        {

            if(e.type==SDL_QUIT) return;

            if(e.type==SDL_MOUSEBUTTONDOWN)
            {

                int mx=e.button.x;
                int my=e.button.y;

                // UI
                if(mx>btnPause.x && mx<btnPause.x+100 &&
                        my>btnPause.y && my<btnPause.y+40)
                    paused=!paused;

                else if(mx>btnReset.x && mx<btnReset.x+100 &&
                        my>btnReset.y && my<btnReset.y+40)
                    reset();

                else if(mx>btnBack.x && mx<btnBack.x+120 &&
                        my>btnBack.y && my<btnBack.y+40)
                    return;

                // interaction masse
                else
                {
                    float dx = mx - x;
                    float dy = my - CY;

                    if(dx*dx + dy*dy < 20*20)
                    {
                        holding = 1;
                        v = 0;
                    }
                }
            }

            if(e.type==SDL_MOUSEBUTTONUP)
            {
                holding=0;
            }
        }

        int mx,my;
        SDL_GetMouseState(&mx,&my);

        // déplacement souris
        if(holding)
        {
            x = mx;
            v = 0;
        }

        Uint32 now=SDL_GetTicks();
        float dt=(now-last)/1000.0f;
        last=now;

        if(!paused && !holding)
            update(dt);

        // ================= ENERGIES =================
        Ec[gi]=kinetic();
        Ep[gi]=potential();
        Et[gi]=Ec[gi]+Ep[gi];
        gi=(gi+1)%GSIZE;

        // ================= RENDER =================
        SDL_SetRenderDrawColor(ren,15,15,25,255);
        SDL_RenderClear(ren);

        // axe
        SDL_SetRenderDrawColor(ren,80,80,80,255);
        SDL_RenderDrawLine(ren,50,CY,650,CY);

        // mur
        SDL_SetRenderDrawColor(ren,200,200,200,255);
        SDL_RenderDrawLine(ren,CX,CY-40,CX,CY+40);

        // ressort
        SDL_SetRenderDrawColor(ren,255,255,255,255);
        drawSpring(ren,CX,CY,x,CY);

        // masse
        SDL_SetRenderDrawColor(ren,100,200,255,255);
        drawCircle(ren,x,CY,15);

        // force
        float F = -k*(x - CX);
        SDL_SetRenderDrawColor(ren,255,50,50,255);
        drawArrow(ren,x,CY,x+F*0.2f,CY);
        drawText(ren,font,"F",x+F*0.2f,CY-15);

        // UI
        SDL_SetRenderDrawColor(ren,40,40,60,255);
        SDL_RenderFillRect(ren,&(SDL_Rect)
        {
            700,0,300,700
        });

        SDL_SetRenderDrawColor(ren,70,70,120,255);
        SDL_RenderFillRect(ren,&btnPause);
        drawText(ren,font,paused?"Resume":"Pause",730,270);

        SDL_SetRenderDrawColor(ren,120,70,70,255);
        SDL_RenderFillRect(ren,&btnReset);
        drawText(ren,font,"Reset",850,270);

        SDL_SetRenderDrawColor(ren,90,60,120,255);
        SDL_RenderFillRect(ren,&btnBack);
        drawText(ren,font,"Retour",740,330);

        char buf[64];
        sprintf(buf,"x=%.1f v=%.2f",x,v);
        drawText(ren,font,buf,720,380);

        drawGraph(ren,font);

        SDL_RenderPresent(ren);
    }
}
