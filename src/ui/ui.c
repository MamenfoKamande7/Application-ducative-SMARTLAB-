#include "ui.h"
#include "auth.h"
#include <string.h>
#include <stdio.h>

// ===================== UTILITAIRES =====================

bool ui_clicked(int mx, int my, SDL_Rect r) {
    return (mx > r.x && mx < r.x + r.w &&
            my > r.y && my < r.y + r.h);
}

void ui_render_text(SDL_Renderer *renderer, TTF_Font *font,
                    const char *text, int x, int y, SDL_Color color) {
    if (!text || strlen(text) == 0) return;
    SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) { SDL_FreeSurface(surf); return; }
    SDL_Rect dst = {x, y, surf->w, surf->h};
    SDL_RenderCopy(renderer, tex, NULL, &dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void ui_render_text_centered(SDL_Renderer *renderer, TTF_Font *font,
                             const char *text, SDL_Rect zone,
                             SDL_Color color) {
    if (!text || strlen(text) == 0) return;
    SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) { SDL_FreeSurface(surf); return; }
    SDL_Rect dst = {
        zone.x + (zone.w - surf->w) / 2,
        zone.y + (zone.h - surf->h) / 2,
        surf->w,
        surf->h
    };
    SDL_RenderCopy(renderer, tex, NULL, &dst);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void ui_draw_rect(SDL_Renderer *renderer, SDL_Rect r,
                  SDL_Color couleur) {
    SDL_SetRenderDrawColor(renderer,
                           couleur.r, couleur.g,
                           couleur.b, couleur.a);
    SDL_RenderFillRect(renderer, &r);
}

void ui_draw_border(SDL_Renderer *renderer, SDL_Rect r,
                    SDL_Color couleur, int epaisseur) {
    SDL_SetRenderDrawColor(renderer,
                           couleur.r, couleur.g,
                           couleur.b, couleur.a);
    for (int i = 0; i < epaisseur; i++) {
        SDL_Rect border = {r.x - i, r.y - i,
                           r.w + i*2, r.h + i*2};
        SDL_RenderDrawRect(renderer, &border);
    }
}

// ===================== BOUTON =====================

void ui_draw_bouton(SDL_Renderer *renderer, TTF_Font *font,
                    Bouton b) {
    ui_draw_rect(renderer, b.rect, b.couleur);

    SDL_Color ombre = {
        (Uint8)(b.couleur.r * 0.7f),
        (Uint8)(b.couleur.g * 0.7f),
        (Uint8)(b.couleur.b * 0.7f),
        180
    };
    ui_draw_border(renderer, b.rect, ombre, 1);
    ui_render_text_centered(renderer, font,
                            b.label, b.rect, b.couleur_texte);
}

bool ui_bouton_clicked(Bouton b, int mx, int my) {
    return ui_clicked(mx, my, b.rect);
}

// ===================== CHAMP TEXTE ===================
// Ajoute à la fin de src/ui/ui.c

void ui_draw_message(SDL_Renderer *renderer, TTF_Font *font,
                     const char *message, int x, int y,
                     SDL_Color couleur, int *timer) {
    if (!timer || *timer <= 0) return;

    SDL_Surface *surf = TTF_RenderText_Blended(font, message, couleur);
    if (!surf) return;

    // Fond semi-transparent
    SDL_Rect fond = {x - 8, y - 5, surf->w + 16, surf->h + 10};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
    SDL_RenderFillRect(renderer, &fond);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_FreeSurface(surf);

    // Texte
    ui_render_text(renderer, font, message, x, y, couleur);

    (*timer)--;
}