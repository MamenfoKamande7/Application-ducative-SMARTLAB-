#include "modules.h"
#include "ui.h"
#include <string.h>
#include <stdio.h>

// ===================== QUESTIONS =====================
static Question questions_maths[] = {
    {
        "Quelle est la derivee de f(x) = x^2 ?",
        {"f'(x) = x", "f'(x) = 2x", "f'(x) = 2", "f'(x) = x^2"},
        1,
        "La derivee de x^2 est 2x par la regle de puissance."
    },
    {
        "Que vaut sin(pi/2) ?",
        {"0", "1", "-1", "1/2"},
        1,
        "sin(pi/2) = 1, c'est une valeur remarquable."
    },
    {
        "Limite de sin(x)/x quand x tend vers 0 ?",
        {"0", "infini", "1", "indeterminee"},
        2,
        "Limite fondamentale : lim sin(x)/x = 1 en 0."
    },
    {
        "Determinant de la matrice identite 2x2 ?",
        {"0", "2", "1", "-1"},
        2,
        "det(I) = 1 pour toute matrice identite."
    },
    {
        "Somme des angles d'un triangle ?",
        {"90", "180", "270", "360"},
        1,
        "La somme des angles d'un triangle est 180 degres."
    },
    {
        "Que vaut log(1) en base 10 ?",
        {"1", "-1", "0", "indefini"},
        2,
        "log(1) = 0 car 10^0 = 1."
    },
    {
        "Formule de l'aire d'un cercle ?",
        {"2*pi*r", "pi*r^2", "pi*d", "2*pi*r^2"},
        1,
        "L'aire d'un cercle de rayon r est A = pi*r^2."
    },
    {
        "Que vaut i^2 (nombre complexe) ?",
        {"1", "i", "-1", "-i"},
        2,
        "Par definition, i^2 = -1."
    },
    {
        "Quelle est la derivee de ln(x) ?",
        {"ln(x)", "1/x", "x", "e^x"},
        1,
        "La derivee de ln(x) est 1/x pour x > 0."
    },
    {
        "Combien y a-t-il de permutations de 3 elements ?",
        {"3", "6", "9", "12"},
        1,
        "3! = 3 x 2 x 1 = 6 permutations."
    }
};

#define NB_QUESTIONS_MATHS 10

// ===================== GETTER =====================
int maths_get_questions(Question *out, int max) {
    int n = (max < NB_QUESTIONS_MATHS) ? max : NB_QUESTIONS_MATHS;
    for (int i = 0; i < n; i++) out[i] = questions_maths[i];
    return n;
}

// ===================== ECRAN QCM =====================
int maths_screen(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color blanc  = {255, 255, 255, 255};
    SDL_Color vert   = {46,  204, 113, 255};
    SDL_Color rouge  = {231, 76,  60,  255};
    SDL_Color violet = {155, 89,  182, 255};
    SDL_Color gris   = {149, 165, 166, 255};

    int  q_index     = 0;
    int  score       = 0;
    int  reponse_sel = -1;
    int  reponse_ok  = -1;
    bool termine     = false;
    char message[256] = "";
    int  msg_timer    = 0;

    SDL_Event e;

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return -1;

            if (e.type == SDL_MOUSEBUTTONDOWN && !termine) {
                int mx = e.button.x, my = e.button.y;

                // Bouton Abandonner
                SDL_Rect btn_abandon = {220, 530, 160, 40};
                if (ui_clicked(mx, my, btn_abandon)) return 0;

                if (reponse_ok == -1) {
                    for (int i = 0; i < 4; i++) {
                        SDL_Rect r = {220, 220 + (i * 70), 560, 55};
                        if (ui_clicked(mx, my, r)) {
                            reponse_sel = i;
                            if (i == questions_maths[q_index].bonne_reponse) {
                                reponse_ok = 1;
                                score++;
                                strcpy(message, "Bonne reponse !");
                            } else {
                                reponse_ok = 0;
                                strcpy(message,
                                    questions_maths[q_index].explication);
                            }
                            msg_timer = 180;
                        }
                    }
                }

                SDL_Rect btn_suivant = {560, 530, 180, 45};
                if (ui_clicked(mx, my, btn_suivant) && reponse_ok != -1) {
                    q_index++;
                    if (q_index >= NB_QUESTIONS_MATHS) {
                        termine = true;
                        Score s;
                        strncpy(s.email, "user", 99);
                        s.module = MODULE_MATHS;
                        s.score  = score;
                        s.total  = NB_QUESTIONS_MATHS;
                        score_save("database/scores.csv", &s);
                    } else {
                        reponse_sel = -1;
                        reponse_ok  = -1;
                        memset(message, 0, sizeof(message));
                        msg_timer   = 0;
                    }
                }
            }

            if (termine && e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;
                if (ui_clicked(mx, my,
                    (SDL_Rect){310, 430, 180, 45})) {
                    q_index = 0; score = 0;
                    reponse_sel = -1; reponse_ok = -1;
                    termine = false;
                    memset(message, 0, sizeof(message));
                }
                if (ui_clicked(mx, my,
                    (SDL_Rect){510, 430, 180, 45})) return 0;
            }
        }

        if (msg_timer > 0) msg_timer--;

        // ===== RENDU =====
        ui_draw_rect(renderer, (SDL_Rect){210, 0, 590, 600},
                     (SDL_Color){30, 30, 45, 255});

        SDL_Rect header = {210, 0, 590, 60};
        ui_draw_rect(renderer, header, violet);
        ui_render_text_centered(renderer, font,
                                "MATHEMATIQUES", header, blanc);

        if (termine) {
            char resultat[100];
            snprintf(resultat, sizeof(resultat),
                     "Score : %d / %d", score, NB_QUESTIONS_MATHS);
            ui_render_text_centered(renderer, font, resultat,
                (SDL_Rect){210, 200, 590, 60}, blanc);

            const char *appr; SDL_Color col_a;
            if      (score >= 8) { appr = "Excellent !";        col_a = vert;   }
            else if (score >= 5) { appr = "Bien ! Continuez !"; col_a = violet; }
            else                 { appr = "A revoir !";          col_a = rouge;  }

            ui_render_text_centered(renderer, font, appr,
                (SDL_Rect){210, 270, 590, 40}, col_a);

            ui_draw_bouton(renderer, font,
                (Bouton){{310, 430, 180, 45}, "Recommencer",
                          vert, blanc});
            ui_draw_bouton(renderer, font,
                (Bouton){{510, 430, 180, 45}, "Menu",
                          rouge, blanc});

        } else {
            char prog[40];
            snprintf(prog, sizeof(prog), "Question %d / %d",
                     q_index + 1, NB_QUESTIONS_MATHS);
            ui_render_text(renderer, font, prog, 220, 70, gris);

            ui_draw_rect(renderer, (SDL_Rect){220, 95, 560, 12},
                         (SDL_Color){60, 60, 80, 255});
            ui_draw_rect(renderer,
                (SDL_Rect){220, 95,
                           (560 * q_index) / NB_QUESTIONS_MATHS, 12},
                violet);

            char sc[20];
            snprintf(sc, sizeof(sc), "Score : %d", score);
            ui_render_text(renderer, font, sc, 650, 70, blanc);

            ui_draw_rect(renderer, (SDL_Rect){220, 115, 560, 90},
                         (SDL_Color){40, 40, 60, 255});
            ui_render_text_centered(renderer, font,
                questions_maths[q_index].enonce,
                (SDL_Rect){220, 115, 560, 90}, blanc);

            for (int i = 0; i < 4; i++) {
                SDL_Rect r = {220, 220 + (i * 70), 560, 55};
                SDL_Color col;
                if (reponse_ok == -1)
                    col = (SDL_Color){50, 50, 75, 255};
                else if (i == questions_maths[q_index].bonne_reponse)
                    col = vert;
                else if (i == reponse_sel)
                    col = rouge;
                else
                    col = (SDL_Color){50, 50, 75, 255};

                ui_draw_rect(renderer, r, col);
                ui_draw_border(renderer, r,
                               (SDL_Color){80, 80, 110, 255}, 1);
                ui_render_text_centered(renderer, font,
                    questions_maths[q_index].choix[i], r, blanc);
            }

            if (msg_timer > 0) {
                SDL_Color cm = (reponse_ok == 1) ? vert : rouge;
                ui_draw_message(renderer, font, message,
                                220, 505, cm, &msg_timer);
            }

            ui_draw_bouton(renderer, font,
                (Bouton){{220, 530, 160, 40}, "Abandonner",
                          rouge, blanc});

            if (reponse_ok != -1) {
                ui_draw_bouton(renderer, font,
                    (Bouton){{560, 530, 180, 45},
                    (q_index == NB_QUESTIONS_MATHS - 1)
                        ? "Terminer" : "Suivant ->",
                    vert, blanc});
            } else {
                ui_draw_rect(renderer,
                             (SDL_Rect){560, 530, 180, 45},
                             (SDL_Color){80, 80, 80, 255});
                ui_render_text_centered(renderer, font,
                    "Suivant ->",
                    (SDL_Rect){560, 530, 180, 45}, gris);
            }
        }

        SDL_RenderPresent(renderer);
    }
}