#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "auth.h"


// ===================== COULEURS =====================
#define COL_BG     (SDL_Color){30,  30,  45,  255}
#define COL_BLANC  (SDL_Color){255, 255, 255, 255}
#define COL_NOIR   (SDL_Color){0,   0,   0,   255}
#define COL_BLEU   (SDL_Color){52,  152, 219, 255}
#define COL_VERT   (SDL_Color){46,  204, 113, 255}
#define COL_ROUGE  (SDL_Color){231, 76,  60,  255}
#define COL_ORANGE (SDL_Color){230, 126, 34,  255}
#define COL_VIOLET (SDL_Color){155, 89,  182, 255}
#define COL_GRIS   (SDL_Color){149, 165, 166, 255}

// ===================== BOUTON =====================
typedef struct {
    SDL_Rect    rect;
    const char *label;
    SDL_Color   couleur;
    SDL_Color   couleur_texte;
} Bouton;

// ===================== CHAMP TEXTE =====================
typedef struct {
    SDL_Rect rect;
    char     valeur[100];
    char    *placeholder;
    bool     actif;
    bool     secret;
} ChampTexte;

// ===================== PROTOTYPES UTILITAIRES =====================
bool ui_clicked(int mx, int my, SDL_Rect r);

void ui_render_text(SDL_Renderer *renderer, TTF_Font *font,
                    const char *text, int x, int y, SDL_Color color);

void ui_render_text_centered(SDL_Renderer *renderer, TTF_Font *font,
                             const char *text, SDL_Rect zone,
                             SDL_Color color);

void ui_draw_rect(SDL_Renderer *renderer, SDL_Rect r,
                  SDL_Color couleur);

void ui_draw_border(SDL_Renderer *renderer, SDL_Rect r,
                    SDL_Color couleur, int epaisseur);

// ===================== PROTOTYPES BOUTON =====================
void ui_draw_bouton(SDL_Renderer *renderer, TTF_Font *font, Bouton b);
bool ui_bouton_clicked(Bouton b, int mx, int my);

// ===================== PROTOTYPES CHAMP TEXTE =====================
void ui_draw_champ(SDL_Renderer *renderer, TTF_Font *font,
                   ChampTexte *c);
void ui_champ_input(ChampTexte *c, SDL_Event *e);

// ===================== PROTOTYPES MESSAGE =====================
void ui_draw_message(SDL_Renderer *renderer, TTF_Font *font,
                     const char *message, int x, int y,
                     SDL_Color couleur, int *timer);

// ===================== PROTOTYPE MENU =====================
int menu_screen(SDL_Renderer *renderer, TTF_Font *font,
                AuthSession *session);

#endif