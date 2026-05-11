#include "ui.h"
#include "modules.h"
#include "auth.h"
#include <string.h>
#include <stdio.h>

// ===================== ÉTATS DU MENU =====================
typedef enum {
    MENU_ACCUEIL,
    MENU_MATHS,
    MENU_PHYSIQUE,
    MENU_CHIMIE,
    MENU_PROFIL,
    MENU_QUITTER
} MenuEtat;

// ===================== SIDEBAR =====================
static void draw_sidebar(SDL_Renderer *renderer, TTF_Font *font,
                         MenuEtat etat_actuel, AuthSession *session) {
    SDL_Rect sidebar = {0, 0, 200, 600};
    ui_draw_rect(renderer, sidebar, (SDL_Color){20, 20, 35, 255});

    SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
    SDL_RenderDrawLine(renderer, 200, 0, 200, 600);

    SDL_Rect titre_zone = {0, 10, 200, 50};
    ui_render_text_centered(renderer, font, "SMARTLAB", titre_zone,
                            (SDL_Color){52, 152, 219, 255});

    SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
    SDL_RenderDrawLine(renderer, 10, 65, 190, 65);

    char bienvenue[120];
    snprintf(bienvenue, sizeof(bienvenue), "%s %s",
             session->prenom, session->nom);
    ui_render_text(renderer, font, bienvenue, 10, 80,
                   (SDL_Color){200, 200, 200, 255});
    ui_render_text(renderer, font, session->role, 10, 105,
                   (SDL_Color){149, 165, 166, 255});

    SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
    SDL_RenderDrawLine(renderer, 10, 130, 190, 130);

    const char *nav_labels[] = {"Accueil", "Maths",
                                "Physique", "Chimie", "Profil"};
    MenuEtat nav_etats[] = {MENU_ACCUEIL, MENU_MATHS,
                            MENU_PHYSIQUE, MENU_CHIMIE, MENU_PROFIL};

    for (int i = 0; i < 5; i++) {
        SDL_Rect r = {0, 145 + (i * 55), 200, 45};
        SDL_Color couleur = (nav_etats[i] == etat_actuel)
            ? (SDL_Color){52, 152, 219, 255}
            : (SDL_Color){40, 40, 60, 255};
        ui_draw_rect(renderer, r, couleur);
        ui_render_text_centered(renderer, font, nav_labels[i], r,
                                (SDL_Color){255, 255, 255, 255});
    }

    SDL_Rect btn_deco = {10, 545, 180, 40};
    ui_draw_rect(renderer, btn_deco, (SDL_Color){231, 76, 60, 255});
    ui_render_text_centered(renderer, font, "Deconnexion", btn_deco,
                            (SDL_Color){255, 255, 255, 255});
}

// ===================== ACCUEIL =====================
static void draw_accueil(SDL_Renderer *renderer, TTF_Font *font,
                         AuthSession *session) {
    char titre[150];
    snprintf(titre, sizeof(titre), "Bonjour, %s !", session->prenom);
    ui_render_text(renderer, font, titre, 230, 30,
                   (SDL_Color){255, 255, 255, 255});

    ui_render_text(renderer, font,
                   "Choisissez une matiere pour commencer",
                   230, 65, (SDL_Color){149, 165, 166, 255});

    SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
    SDL_RenderDrawLine(renderer, 220, 95, 790, 95);

    typedef struct {
        const char *nom;
        const char *desc;
        SDL_Color   col;
    } Info;

    Info infos[3] = {
        {"MATHS",    "Algebre, Analyse",   {155, 89,  182, 255}},
        {"PHYSIQUE", "Mecanique, Optique", {52,  152, 219, 255}},
        {"CHIMIE",   "Molecules, Chimie",  {230, 126, 34,  255}}
    };

    for (int i = 0; i < 3; i++) {
        SDL_Rect carte = {220 + (i * 195), 120, 175, 200};
        ui_draw_rect(renderer, carte, infos[i].col);

        SDL_Rect icone_zone = {220 + (i * 195), 120, 175, 80};
        ui_render_text_centered(renderer, font, infos[i].nom,
                                icone_zone,
                                (SDL_Color){255, 255, 255, 255});

        ui_render_text(renderer, font, infos[i].desc,
                       230 + (i * 195), 210,
                       (SDL_Color){255, 255, 255, 255});

        SDL_Rect btn = {230 + (i * 195), 290, 155, 35};
        ui_draw_rect(renderer, btn, (SDL_Color){0, 0, 0, 80});
        ui_render_text_centered(renderer, font, "Commencer", btn,
                                (SDL_Color){255, 255, 255, 255});
    }

    SDL_Rect stats_zone = {220, 360, 570, 220};
    ui_draw_rect(renderer, stats_zone, (SDL_Color){40, 40, 60, 255});

    ui_render_text(renderer, font, "Vos statistiques", 240, 375,
                   (SDL_Color){255, 255, 255, 255});

    SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
    SDL_RenderDrawLine(renderer, 230, 400, 780, 400);

    const char *matieres[] = {"Maths", "Physique", "Chimie"};
    SDL_Color cols[] = {
        {155, 89,  182, 255},
        {52,  152, 219, 255},
        {230, 126, 34,  255}
    };

    for (int i = 0; i < 3; i++) {
        int x = 240 + (i * 190);
        ui_render_text(renderer, font, matieres[i], x, 415, cols[i]);

        int best = score_best("database/scores.csv",
                              session->email, (ModuleID)i);

        SDL_Rect barre_fond = {x, 445, 150, 20};
        ui_draw_rect(renderer, barre_fond,
                     (SDL_Color){60, 60, 80, 255});

        if (best >= 0) {
            int largeur = (150 * best) / 10;
            SDL_Rect barre_val = {x, 445, largeur, 20};
            ui_draw_rect(renderer, barre_val, cols[i]);

            char pct[20];
            snprintf(pct, sizeof(pct), "%d/10", best);
            ui_render_text(renderer, font, pct, x + 155, 445,
                           (SDL_Color){200, 200, 200, 255});
        } else {
            ui_render_text(renderer, font, "Pas joue", x, 470,
                           (SDL_Color){149, 165, 166, 255});
        }
    }
}

// ===================== PROFIL =====================
static void draw_profil(SDL_Renderer *renderer, TTF_Font *font,
                        AuthSession *session) {
    ui_render_text(renderer, font, "MON PROFIL", 230, 30,
                   (SDL_Color){255, 255, 255, 255});

    SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
    SDL_RenderDrawLine(renderer, 220, 60, 790, 60);

    SDL_Rect carte = {230, 80, 540, 300};
    ui_draw_rect(renderer, carte, (SDL_Color){40, 40, 60, 255});

    SDL_Rect avatar = {260, 100, 80, 80};
    ui_draw_rect(renderer, avatar, (SDL_Color){52, 152, 219, 255});
    ui_render_text_centered(renderer, font,
                            (char[]){session->prenom[0], '\0'},
                            avatar, (SDL_Color){255, 255, 255, 255});

    char ligne[150];

    snprintf(ligne, sizeof(ligne), "Nom    : %s", session->nom);
    ui_render_text(renderer, font, ligne, 360, 110,
                   (SDL_Color){255, 255, 255, 255});

    snprintf(ligne, sizeof(ligne), "Prenom : %s", session->prenom);
    ui_render_text(renderer, font, ligne, 360, 140,
                   (SDL_Color){255, 255, 255, 255});

    snprintf(ligne, sizeof(ligne), "Email  : %s", session->email);
    ui_render_text(renderer, font, ligne, 360, 170,
                   (SDL_Color){255, 255, 255, 255});

    snprintf(ligne, sizeof(ligne), "Role   : %s", session->role);
    ui_render_text(renderer, font, ligne, 360, 200,
                   (SDL_Color){149, 165, 166, 255});

    SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
    SDL_RenderDrawLine(renderer, 245, 240, 755, 240);

    ui_render_text(renderer, font, "Token de session :", 260, 255,
                   (SDL_Color){149, 165, 166, 255});
    ui_render_text(renderer, font, session->token, 260, 280,
                   (SDL_Color){52, 152, 219, 255});
}

// ===================== MENU PRINCIPAL =====================
int menu_screen(SDL_Renderer *renderer, TTF_Font *font,
                AuthSession *session) {
    SDL_Event e;
    MenuEtat  etat = MENU_ACCUEIL;

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return 0;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x;
                int my = e.button.y;

                const MenuEtat nav_etats[] = {
                    MENU_ACCUEIL, MENU_MATHS, MENU_PHYSIQUE,
                    MENU_CHIMIE,  MENU_PROFIL
                };
                for (int i = 0; i < 5; i++) {
                    SDL_Rect rect = {0, 145 + (i * 55), 200, 45};
                    if (ui_clicked(mx, my, rect))
                        etat = nav_etats[i];
                }

                SDL_Rect btn_deco = {10, 545, 180, 40};
                if (ui_clicked(mx, my, btn_deco)) return 1;

                if (etat == MENU_ACCUEIL) {
                    for (int i = 0; i < 3; i++) {
                        SDL_Rect btn = {230 + (i * 195), 290,
                                        155, 35};
                        if (ui_clicked(mx, my, btn)) {
                            if (i == 0) etat = MENU_MATHS;
                            if (i == 1) etat = MENU_PHYSIQUE;
                            if (i == 2) etat = MENU_CHIMIE;
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 45, 255);
        SDL_RenderClear(renderer);

        draw_sidebar(renderer, font, etat, session);

        switch (etat) {
            case MENU_ACCUEIL:
                draw_accueil(renderer, font, session);
                break;

            case MENU_MATHS: {
                int res_m = maths_screen(renderer, font);
                if (res_m == -1) return 0;
                etat = MENU_ACCUEIL;
                break;
            }

            case MENU_PHYSIQUE: {
                int res_p = physique_screen(renderer, font);
                if (res_p == -1) return 0;
                etat = MENU_ACCUEIL;
                break;
            }

            case MENU_CHIMIE: {
                int res_c = chimie_screen(renderer, font);
                if (res_c == -1) return 0;
                etat = MENU_ACCUEIL;
                break;
            }

            case MENU_PROFIL:
                draw_profil(renderer, font, session);
                break;

            default:
                break;
        }

        SDL_RenderPresent(renderer);
    }
}