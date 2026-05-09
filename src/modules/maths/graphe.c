/*

===========================================
SMARTLAB - graphe.c
Version professionnelle V3
===========================================

Fonctionnalités :

✔ saisie de fonctions
✔ tracé interactif
✔ zoom
✔ déplacement
✔ tangente dynamique
✔ limites
✔ dérivée numérique
✔ extrema
✔ tableau de variation

Fonctions supportées :
- x
- x^2
- x^3
- sin(x)
- cos(x)
- 1/x

===========================================

*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "ui/draw.h"
#include "modules/maths/graphe.h"

// =====================================================
// CONFIG
// =====================================================

#define WIDTH 1000
#define HEIGHT 700

#define GRAPH_WIDTH 700
#define PANEL_X 700

#define MAX_CRITICAL 20

// =====================================================
// SCREENS
// =====================================================

typedef enum
{
    SCREEN_INPUT,
    SCREEN_GRAPH
} Screen;

// =====================================================
// PANELS
// =====================================================

typedef enum
{
    PANEL_NONE,
    PANEL_LIMITS,
    PANEL_VARIATIONS
} PanelMode;

// =====================================================
// FUNCTION TYPES
// =====================================================

typedef enum
{
    FUNC_NONE,
    FUNC_X,
    FUNC_X2,
    FUNC_X3,
    FUNC_SIN,
    FUNC_COS,
    FUNC_INV
} FunctionType;

typedef struct
{
    FunctionType type;
} Function;

// =====================================================
// GLOBALS
// =====================================================

static Screen currentScreen = SCREEN_INPUT;

static PanelMode currentPanel = PANEL_NONE;

static Function currentFunction;

static char input[100] = "";

static float zoom = 80.0f;

static float offsetX = 0;
static float offsetY = 0;

static int dragging = 0;

static int lastX = 0;
static int lastY = 0;

// =====================================================
// CRITICAL POINTS
// =====================================================

static float criticalPoints[MAX_CRITICAL];

static int criticalCount = 0;

// =====================================================
// BUTTONS
// =====================================================

static SDL_Rect btnLimits =
{
    740,150,220,60
};

static SDL_Rect btnVariations =
{
    740,250,220,60
};

static SDL_Rect btnReturn =
{
    740,350,220,60
};

// =====================================================
// PARSE
// =====================================================

Function parse(char *s)
{
    Function f;

    f.type = FUNC_NONE;

    if(strcmp(s,"x")==0)
        f.type = FUNC_X;

    else if(strcmp(s,"x^2")==0)
        f.type = FUNC_X2;

    else if(strcmp(s,"x^3")==0)
        f.type = FUNC_X3;

    else if(strcmp(s,"sin(x)")==0)
        f.type = FUNC_SIN;

    else if(strcmp(s,"cos(x)")==0)
        f.type = FUNC_COS;

    else if(strcmp(s,"1/x")==0)
        f.type = FUNC_INV;

    return f;
}

// =====================================================
// COMPUTE
// =====================================================

float compute(Function f,float x)
{
    switch(f.type)
    {
        case FUNC_X:
            return x;

        case FUNC_X2:
            return x*x;

        case FUNC_X3:
            return x*x*x;

        case FUNC_SIN:
            return sinf(x);

        case FUNC_COS:
            return cosf(x);

        case FUNC_INV:

            if(fabs(x)<0.0001f)
                return NAN;

            return 1.0f/x;

        default:
            return 0;
    }
}

// =====================================================
// DERIVATIVE
// =====================================================

float derivative(Function f,float x)
{
    float h = 0.0001f;

    return (compute(f,x+h)-compute(f,x-h))/(2*h);
}

// =====================================================
// FIND CRITICAL POINTS
// =====================================================

void find_critical_points(Function f)
{
    criticalCount = 0;

    float step = 0.05f;

    float prev = derivative(f,-10);

    for(float x=-10;x<=10;x+=step)
    {
        float d = derivative(f,x);

        // changement de signe

        if((prev < 0 && d > 0) ||
           (prev > 0 && d < 0))
        {
            if(criticalCount < MAX_CRITICAL)
            {
                criticalPoints[criticalCount++] = x;
            }
        }

        prev = d;
    }
}

// =====================================================
// GRID
// =====================================================

void draw_grid(SDL_Renderer *ren)
{
    SDL_SetRenderDrawColor(ren,35,35,45,255);

    for(int x=0;x<GRAPH_WIDTH;x+=50)
    {
        SDL_RenderDrawLine(ren,x,0,x,HEIGHT);
    }

    for(int y=0;y<HEIGHT;y+=50)
    {
        SDL_RenderDrawLine(ren,0,y,GRAPH_WIDTH,y);
    }
}

// =====================================================
// AXES
// =====================================================

void draw_axes(SDL_Renderer *ren,int cx,int cy)
{
    SDL_SetRenderDrawColor(ren,220,220,220,255);

    SDL_RenderDrawLine(ren,0,cy,GRAPH_WIDTH,cy);

    SDL_RenderDrawLine(ren,cx,0,cx,HEIGHT);
}

// =====================================================
// CURVE
// =====================================================

void draw_curve(SDL_Renderer *ren,
                Function f,
                int cx,
                int cy)
{
    SDL_SetRenderDrawColor(ren,0,255,150,255);

    int first = 1;

    int prevX = 0;
    int prevY = 0;

    for(int px=0;px<GRAPH_WIDTH;px++)
    {
        float x = (px-cx)/zoom;

        float y = compute(f,x);

        if(isnan(y) || isinf(y))
        {
            first = 1;
            continue;
        }

        int py = cy-y*zoom;

        if(!first)
        {
            SDL_RenderDrawLine(ren,
                               prevX,
                               prevY,
                               px,
                               py);
        }

        first = 0;

        prevX = px;
        prevY = py;
    }
}

// =====================================================
// TANGENT
// =====================================================

void draw_tangent(SDL_Renderer *ren,
                  Function f,
                  int cx,
                  int cy)
{
    int mx,my;

    SDL_GetMouseState(&mx,&my);

    if(mx > GRAPH_WIDTH)
        return;

    float a = (mx-cx)/zoom;

    float slope = derivative(f,a);

    float fa = compute(f,a);

    if(isnan(fa) || isinf(fa))
        return;

    SDL_SetRenderDrawColor(ren,255,220,0,255);

    int first = 1;

    int prevX = 0;
    int prevY = 0;

    for(int px=0;px<GRAPH_WIDTH;px++)
    {
        float x = (px-cx)/zoom;

        float y = slope*(x-a)+fa;

        if(isnan(y) || isinf(y))
        {
            first = 1;
            continue;
        }

        int py = cy-y*zoom;

        if(!first)
        {
            SDL_RenderDrawLine(ren,
                               prevX,
                               prevY,
                               px,
                               py);
        }

        first = 0;

        prevX = px;
        prevY = py;
    }

    // point de tangence

    int sx = cx+a*zoom;

    int sy = cy-fa*zoom;

    SDL_SetRenderDrawColor(ren,255,0,0,255);

    drawCircle(ren,sx,sy,7);
}

// =====================================================
// BUTTON
// =====================================================

void draw_button(SDL_Renderer *ren,
                 TTF_Font *font,
                 SDL_Rect rect,
                 const char *text,
                 SDL_Color color)
{
    SDL_SetRenderDrawColor(ren,
                           color.r,
                           color.g,
                           color.b,
                           color.a);

    SDL_RenderFillRect(ren,&rect);

    drawText(ren,
             font,
             text,
             rect.x+25,
             rect.y+18);
}

// =====================================================
// VARIATION TABLE
// =====================================================

void draw_variation_table(SDL_Renderer *ren,
                          TTF_Font *font)
{
    drawText(ren,font,
             "TABLEAU DE VARIATION",
             705,450);

    int y = 500;

    for(int i=0;i<criticalCount;i++)
    {
        char buf[128];

        float c = criticalPoints[i];

        float yc = compute(currentFunction,c);

        sprintf(buf,
                "f'(x)=0 vers %.2f",
                c);

        drawText(ren,font,
                 buf,
                 720,y);

        y += 35;

        float left =
            derivative(currentFunction,c-0.1f);

        float right =
            derivative(currentFunction,c+0.1f);

        if(left < 0 && right > 0)
        {
            sprintf(buf,
                    "Minimum : (%.2f ; %.2f)",
                    c,yc);
        }

        else if(left > 0 && right < 0)
        {
            sprintf(buf,
                    "Maximum : (%.2f ; %.2f)",
                    c,yc);
        }

        else
        {
            sprintf(buf,
                    "Point critique");
        }

        drawText(ren,font,
                 buf,
                 720,y);

        y += 50;
    }

    // tableau visuel

    SDL_SetRenderDrawColor(ren,220,220,220,255);

    int tx = 720;
    int ty = 640;

    SDL_RenderDrawLine(ren,tx,ty,960,ty);

    SDL_RenderDrawLine(ren,tx,ty-40,960,ty-40);

    SDL_RenderDrawLine(ren,tx+40,ty-40,tx+40,ty+40);

    drawText(ren,font,"x",730,605);

    drawText(ren,font,"f",730,645);

    int spacing = 150;

    for(int i=0;i<criticalCount;i++)
    {
        int px = tx+80+i*spacing;

        char buf[64];

        sprintf(buf,"%.1f",criticalPoints[i]);

        drawText(ren,font,buf,px,605);

        SDL_RenderDrawLine(ren,
                           px-40,
                           ty+20,
                           px,
                           ty-10);

        SDL_RenderDrawLine(ren,
                           px,
                           ty-10,
                           px+40,
                           ty+20);
    }
}

// =====================================================
// PANEL
// =====================================================

void draw_panel(SDL_Renderer *ren,
                TTF_Font *font)
{
    SDL_Rect panel =
    {
        PANEL_X,0,300,700
    };

    SDL_SetRenderDrawColor(ren,25,25,35,255);

    SDL_RenderFillRect(ren,&panel);

    drawText(ren,font,
             "SMARTLAB ETUDE",
             725,40);

    draw_button(
        ren,font,
        btnLimits,
        "1 - LIMITES",
        (SDL_Color){70,120,220,255}
    );

    draw_button(
        ren,font,
        btnVariations,
        "2 - VARIATIONS",
        (SDL_Color){70,180,120,255}
    );

    draw_button(
        ren,font,
        btnReturn,
        "3 - RETOUR",
        (SDL_Color){180,70,70,255}
    );

    // =====================================
    // LIMITES
    // =====================================

    if(currentPanel == PANEL_LIMITS)
    {
        drawText(ren,font,
                 "LIMITES",
                 760,470);

        switch(currentFunction.type)
        {
            case FUNC_INV:

                drawText(ren,font,
                         "x->0- : -inf",
                         730,520);

                drawText(ren,font,
                         "x->0+ : +inf",
                         730,560);

                break;

            case FUNC_X2:

                drawText(ren,font,
                         "x->+inf : +inf",
                         730,520);

                drawText(ren,font,
                         "x->-inf : +inf",
                         730,560);

                break;

            case FUNC_X3:

                drawText(ren,font,
                         "x->+inf : +inf",
                         730,520);

                drawText(ren,font,
                         "x->-inf : -inf",
                         730,560);

                break;

            default:

                drawText(ren,font,
                         "Non disponible",
                         730,520);
        }
    }

    // =====================================
    // VARIATIONS
    // =====================================

    else if(currentPanel == PANEL_VARIATIONS)
    {
        draw_variation_table(ren,font);
    }
}

// =====================================================
// INPUT SCREEN
// =====================================================

void draw_input_screen(SDL_Renderer *ren,
                       TTF_Font *font)
{
    SDL_SetRenderDrawColor(ren,15,15,25,255);

    SDL_RenderClear(ren);

    drawText(ren,font,
             "SMARTLAB MATHS",
             320,80);

    drawText(ren,font,
             "Entrez une fonction :",
             300,180);

    SDL_Rect inputBox =
    {
        250,250,500,60
    };

    SDL_SetRenderDrawColor(ren,40,40,60,255);

    SDL_RenderFillRect(ren,&inputBox);

    if(strlen(input)==0)
        drawText(ren,font," ",280,270);
    else
        drawText(ren,font,input,280,270);

    drawText(ren,font,
             "Fonctions disponibles :",
             300,380);

    drawText(ren,font,
             "x   x^2   x^3",
             340,430);

    drawText(ren,font,
             "sin(x)   cos(x)   1/x",
             270,470);

    drawText(ren,font,
             "Appuyez sur ENTER",
             300,560);
}

// =====================================================
// GRAPH SCREEN
// =====================================================

void draw_graph_screen(SDL_Renderer *ren,
                       TTF_Font *font)
{
    SDL_SetRenderDrawColor(ren,15,15,20,255);

    SDL_RenderClear(ren);

    int cx = GRAPH_WIDTH/2 + offsetX;

    int cy = HEIGHT/2 + offsetY;

    draw_grid(ren);

    draw_axes(ren,cx,cy);

    draw_curve(ren,currentFunction,cx,cy);

    draw_tangent(ren,currentFunction,cx,cy);

    draw_panel(ren,font);
}

// =====================================================
// INPUT EVENTS
// =====================================================

void handle_input_events(SDL_Event *e)
{
    if(e->type == SDL_TEXTINPUT)
    {
        if(strlen(input) < 90)
        {
            strcat(input,e->text.text);
        }
    }

    if(e->type == SDL_KEYDOWN)
    {
        if(e->key.keysym.sym == SDLK_BACKSPACE
           && strlen(input)>0)
        {
            input[strlen(input)-1] = '\0';
        }

        if(e->key.keysym.sym == SDLK_RETURN)
        {
            currentFunction = parse(input);

            if(currentFunction.type != FUNC_NONE)
            {
                find_critical_points(currentFunction);

                currentScreen = SCREEN_GRAPH;
            }
        }
    }
}

// =====================================================
// GRAPH EVENTS
// =====================================================

void handle_graph_events(SDL_Event *e)
{
    int mx,my;

    SDL_GetMouseState(&mx,&my);

    if(e->type == SDL_MOUSEBUTTONDOWN)
    {
        // limites

        if(mx > btnLimits.x &&
           mx < btnLimits.x+btnLimits.w &&
           my > btnLimits.y &&
           my < btnLimits.y+btnLimits.h)
        {
            currentPanel = PANEL_LIMITS;
        }

        // variations

        if(mx > btnVariations.x &&
           mx < btnVariations.x+btnVariations.w &&
           my > btnVariations.y &&
           my < btnVariations.y+btnVariations.h)
        {
            currentPanel = PANEL_VARIATIONS;
        }

        // retour

        if(mx > btnReturn.x &&
           mx < btnReturn.x+btnReturn.w &&
           my > btnReturn.y &&
           my < btnReturn.y+btnReturn.h)
        {
            currentScreen = SCREEN_INPUT;

            currentPanel = PANEL_NONE;

            strcpy(input,"");
        }

        // drag

        if(mx < GRAPH_WIDTH)
        {
            dragging = 1;

            lastX = mx;
            lastY = my;
        }
    }

    if(e->type == SDL_MOUSEBUTTONUP)
    {
        dragging = 0;
    }

    if(e->type == SDL_MOUSEMOTION && dragging)
    {
        offsetX += e->motion.x-lastX;

        offsetY += e->motion.y-lastY;

        lastX = e->motion.x;

        lastY = e->motion.y;
    }

    // zoom

    if(e->type == SDL_MOUSEWHEEL)
    {
        if(mx < GRAPH_WIDTH)
        {
            if(e->wheel.y > 0)
                zoom *= 1.1f;

            if(e->wheel.y < 0)
                zoom *= 0.9f;
        }
    }
}

// =====================================================
// MAIN
// =====================================================

void simulation_graphe(SDL_Renderer *ren,
                       SDL_Window *win)
{
    (void)win;

    TTF_Font *font =
        TTF_OpenFont(
            "assets/fonts/ariblk.ttf",
            20);

    if(font == NULL)
    {
        printf("Erreur police : %s\n",
               TTF_GetError());

        return;
    }

    SDL_StartTextInput();

    SDL_Event e;

    int running = 1;

    while(running)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
                return;

            if(currentScreen == SCREEN_INPUT)
            {
                handle_input_events(&e);
            }

            else if(currentScreen == SCREEN_GRAPH)
            {
                handle_graph_events(&e);
            }
        }

        // render

        if(currentScreen == SCREEN_INPUT)
        {
            draw_input_screen(ren,font);
        }

        else if(currentScreen == SCREEN_GRAPH)
        {
            draw_graph_screen(ren,font);
        }

        SDL_RenderPresent(ren);
    }

    SDL_StopTextInput();

    TTF_CloseFont(font);
}