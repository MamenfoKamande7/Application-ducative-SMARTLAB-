#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>

#include "ui/draw.h"

#define CX 300
#define CY 150
#define TRAIL 400
#define GSIZE 400

// ================= PHYSIQUE =================
float x, y;
float vx = 0, vy = 0;

float g = 9.81f;
float L = 200;
float damping = 0.999f;

int holding = 0;
int paused = 0;

// ================= TRAJECTOIRE =================
int tx[TRAIL], ty[TRAIL], ti = 0;

// ================= GRAPHE =================
float Ec[GSIZE], Ep[GSIZE], Et[GSIZE];
int gi = 0;

// ================= RESET =================
void reset(){
    x = CX + L * sinf(0.8f);
    y = CY + L * cosf(0.8f);
    vx = vy = 0;
    ti = gi = 0;
}

// ================= ÉNERGIES =================
float kinetic(){ return 0.5f * (vx*vx + vy*vy); }
float potential(){ return g * (y - CY); }

// ================= SLIDER =================
typedef struct{
    SDL_Rect r;
    float min, max, val;
    int drag;
} Slider;

void sliderUpdate(Slider *s, int mx){
    if(s->drag){
        float t = (mx - s->r.x) / (float)s->r.w;

        if(t < 0) t = 0;
        else if(t > 1) t = 1;

        s->val = s->min + t * (s->max - s->min);
    }
}

int sliderHit(Slider *s, int mx, int my){
    return mx >= s->r.x && mx <= s->r.x + s->r.w &&
           my >= s->r.y && my <= s->r.y + s->r.h;
}

void drawSlider(SDL_Renderer *r, TTF_Font *f, Slider *s, char *name){
    SDL_SetRenderDrawColor(r,100,100,120,255);
    SDL_RenderFillRect(r,&s->r);

    float t = (s->val - s->min) / (s->max - s->min);
    int k = s->r.x + t * s->r.w;

    SDL_Rect knob = {k-5, s->r.y-5, 10, s->r.h+10};
    SDL_SetRenderDrawColor(r,200,200,255,255);
    SDL_RenderFillRect(r,&knob);

    char buf[64];
    sprintf(buf,"%s: %.2f",name,s->val);
    drawText(r,f,buf,s->r.x,s->r.y-18);
}

// ================= ARROW =================
void drawArrow(SDL_Renderer *r,int x1,int y1,int x2,int y2){
    SDL_RenderDrawLine(r,x1,y1,x2,y2);

    float a = atan2f(y2-y1, x2-x1);
    float len = 10;

    int x3 = x2 + len*cosf(a + M_PI - 0.5f);
    int y3 = y2 + len*sinf(a + M_PI - 0.5f);

    int x4 = x2 + len*cosf(a + M_PI + 0.5f);
    int y4 = y2 + len*sinf(a + M_PI + 0.5f);

    SDL_RenderDrawLine(r,x2,y2,x3,y3);
    SDL_RenderDrawLine(r,x2,y2,x4,y4);
}

// ================= PHYSIQUE =================
void update(float dt){

    if(dt < 0.001f) dt = 0.001f;
    if(dt > 0.03f) dt = 0.03f;

    vy += g * dt;

    x += vx * dt;
    y += vy * dt;

    float dx = x - CX;
    float dy = y - CY;

    float dist = sqrtf(dx*dx + dy*dy);
    if(dist == 0) dist = 1;

    float diff = (dist - L) / dist;

    x -= dx * diff;
    y -= dy * diff;

    vx -= dx * diff;
    vy -= dy * diff;

    vx *= damping;
    vy *= damping;
}

// ================= MAIN =================
void simulation_pendule(SDL_Renderer *ren, SDL_Window *win){

    TTF_Font *font = TTF_OpenFont("assets/fonts/ariblk.ttf",16);

    Slider sG = {{720,120,200,8},1,20,9.81f,0};
    Slider sL = {{720,160,200,8},100,400,200,0};
    Slider sD = {{720,200,200,8},0.95f,1,0.999f,0};

    SDL_Rect btnPause = {720,260,100,40};
    SDL_Rect btnReset = {840,260,100,40};

    Uint32 last = SDL_GetTicks();
    SDL_Event e;

    reset();

    while(1){

        Uint32 frameStart = SDL_GetTicks();

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) return;

            int mx,my;
            SDL_GetMouseState(&mx,&my);

            if(e.type == SDL_MOUSEBUTTONDOWN){
                holding = 1;

                if(sliderHit(&sG,mx,my)) sG.drag=1;
                if(sliderHit(&sL,mx,my)) sL.drag=1;
                if(sliderHit(&sD,mx,my)) sD.drag=1;

                if(mx>btnPause.x && mx<btnPause.x+100 &&
                   my>btnPause.y && my<btnPause.y+40)
                    paused = !paused;

                if(mx>btnReset.x && mx<btnReset.x+100 &&
                   my>btnReset.y && my<btnReset.y+40)
                    reset();
            }

            if(e.type == SDL_MOUSEBUTTONUP){
                holding = 0;
                sG.drag = sL.drag = sD.drag = 0;
            }
        }

        int mx,my;
        SDL_GetMouseState(&mx,&my);

        sliderUpdate(&sG,mx);
        sliderUpdate(&sL,mx);
        sliderUpdate(&sD,mx);

        g = sG.val;
        L = sL.val;
        damping = sD.val;

        Uint32 now = SDL_GetTicks();
        float dt = (now - last) / 1000.0f;
        last = now;

        if(holding){
            vx += (mx - x) * 5.0f * dt;
            vy += (my - y) * 5.0f * dt;
        }

        if(!paused) update(dt);

        // ================= GEOMETRIE =================
        tx[ti] = (int)x;
        ty[ti] = (int)y;
        ti = (ti + 1) % TRAIL;

        Ec[gi] = kinetic();
        Ep[gi] = potential();
        Et[gi] = Ec[gi] + Ep[gi];
        gi = (gi + 1) % GSIZE;

        float dx = x - CX;
        float dy = y - CY;

        float dist = sqrtf(dx*dx + dy*dy);
        if(dist == 0) dist = 1;

        float erx = dx / dist;
        float ery = dy / dist;

        float etx = -ery;
        float ety = erx;

        // ================= VITESSE CORRIGEE =================
        float v_theta = vx * etx + vy * ety;

        float vfx = v_theta * etx;
        float vfy = v_theta * ety;

        // ================= TENSION =================
        float cosTheta = (y - CY) / L;
        float v2 = vx*vx + vy*vy;

        float T = v2 / L + g * cosTheta;

        float trx = -erx;
        float try_ = -ery;

        int scale = 60;

        // ================= RENDER =================
        SDL_SetRenderDrawColor(ren,15,15,25,255);
        SDL_RenderClear(ren);

        // pendule
        SDL_SetRenderDrawColor(ren,255,255,255,255);
        SDL_RenderDrawLine(ren,CX,CY,x,y);

        SDL_SetRenderDrawColor(ren,255,220,0,255);
        drawCircle(ren,CX,CY,6);

        SDL_SetRenderDrawColor(ren,100,180,255,255);
        drawCircle(ren,x,y,12);

        // trajectoire
        SDL_SetRenderDrawColor(ren,80,120,200,255);
        for(int i=0;i<TRAIL;i++)
            SDL_RenderDrawPoint(ren,tx[i],ty[i]);

        // er
        SDL_SetRenderDrawColor(ren,255,140,0,255);
        drawArrow(ren,x,y,x+erx*scale,y+ery*scale);
        drawText(ren,font,"er",x+erx*(scale+10),y+ery*(scale+10));

        // eθ
        SDL_SetRenderDrawColor(ren,0,200,255,255);
        drawArrow(ren,x,y,x+etx*scale,y+ety*scale);
        drawText(ren,font,"eθ",x+etx*(scale+10),y+ety*(scale+10));

        // vitesse (correcte)
        SDL_SetRenderDrawColor(ren,0,255,0,255);
        drawArrow(ren,x,y,x+vfx*10,y+vfy*10);
        drawText(ren,font,"v",x+vfx*10,y+vfy*10);

        // tension
        SDL_SetRenderDrawColor(ren,255,0,255,255);
        drawArrow(ren,x,y,x+trx*T*2,y+try_*T*2);

        // UI panel
        SDL_SetRenderDrawColor(ren,40,40,60,255);
        SDL_RenderFillRect(ren,&(SDL_Rect){700,0,300,700});

        SDL_SetRenderDrawColor(ren,70,70,120,255);
        SDL_RenderFillRect(ren,&btnPause);
        drawText(ren,font,paused?"Resume":"Pause",730,270);

        SDL_SetRenderDrawColor(ren,120,70,70,255);
        SDL_RenderFillRect(ren,&btnReset);
        drawText(ren,font,"Reset",850,270);

        char buf[64];
        sprintf(buf,"T=%.2f",T);
        drawText(ren,font,buf,720,400);

        sprintf(buf,"x=%.1f y=%.1f",x,y);
        drawText(ren,font,buf,720,320);

        sprintf(buf,"vx=%.2f vy=%.2f",vx,vy);
        drawText(ren,font,buf,720,340);

        // sliders
        drawSlider(ren,font,&sG,"g");
        drawSlider(ren,font,&sL,"L");
        drawSlider(ren,font,&sD,"damping");

        SDL_RenderPresent(ren);

        // FPS LIMIT
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if(frameTime < 16)
            SDL_Delay(16 - frameTime);
    }
}