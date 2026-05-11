#include "modules.h"
#include "ui.h"
#include <string.h>
#include <stdio.h>

// ===================== QUESTIONS =====================
// DOIT être AVANT physique_screen
static Question questions_physique[] = {
    {
        "Quelle est l'unite de la force dans le SI ?",
        {"Joule", "Newton", "Pascal", "Watt"},
        1,
        "La force se mesure en Newton (N)."
    },
    {
        "Vitesse de la lumiere dans le vide ?",
        {"3x10^6 m/s", "3x10^8 m/s", "3x10^10 m/s", "3x10^4 m/s"},
        1,
        "c = 3x10^8 m/s est une constante fondamentale."
    },
    {
        "Que dit la 1ere loi de Newton ?",
        {"F = ma", "Principe d'inertie", "Action-reaction", "Conservation energie"},
        1,
        "La 1ere loi : un corps reste au repos sans force."
    },
    {
        "Quelle est l'unite de la pression ?",
        {"Newton", "Joule", "Pascal", "Watt"},
        2,
        "La pression se mesure en Pascal (Pa)."
    },
    {
        "Que vaut g sur Terre ?",
        {"9.81 m/s", "9.81 m/s^2", "9.81 N", "9.81 kg"},
        1,
        "g = 9.81 m/s^2 est l'acceleration de pesanteur."
    },
    {
        "Quelle loi relie tension, intensite et resistance ?",
        {"Loi de Newton", "Loi de Joule", "Loi d'Ohm", "Loi de Faraday"},
        2,
        "La loi d'Ohm : U = R x I."
    },
    {
        "Quelle est l'unite de l'energie ?",
        {"Newton", "Pascal", "Watt", "Joule"},
        3,
        "L'energie se mesure en Joule (J)."
    },
    {
        "Un photon a-t-il une masse au repos ?",
        {"Oui", "Non", "Selon sa frequence", "Selon sa longueur d'onde"},
        1,
        "Le photon a une masse au repos nulle."
    },
    {
        "Formule de l'energie cinetique ?",
        {"E = mc^2", "E = mgh", "E = 1/2 mv^2", "E = Pt"},
        2,
        "L'energie cinetique est Ec = 1/2 mv^2."
    },
    {
        "Interaction maintenant les electrons autour du noyau ?",
        {"Gravitationnelle", "Forte", "Faible", "Electromagnetique"},
        3,
        "C'est l'interaction electromagnetique."
    }
};

#define NB_QUESTIONS_PHYSIQUE 10

// ===================== GETTER =====================
int physique_get_questions(Question *out, int max) {
    int n = (max < NB_QUESTIONS_PHYSIQUE) ? max : NB_QUESTIONS_PHYSIQUE;
    for (int i = 0; i < n; i++) out[i] = questions_physique[i];
    return n;
}

// ===================== ECRAN QCM =====================
int physique_screen(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color vert  = {46,  204, 113, 255};
    SDL_Color rouge = {231, 76,  60,  255};
    SDL_Color bleu  = {52,  152, 219, 255};
    SDL_Color gris  = {149, 165, 166, 255};

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
            if (e.type == SDL_QUIT) return -1;  // propage SDL_QUIT

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
                            if (i == questions_physique[q_index].bonne_reponse) {
                                reponse_ok = 1;
                                score++;
                                strcpy(message, "Bonne reponse !");
                            } else {
                                reponse_ok = 0;
                                strcpy(message,
                                    questions_physique[q_index].explication);
                            }
                            msg_timer = 180;
                        }
                    }
                }

                SDL_Rect btn_suivant = {560, 530, 180, 45};
                if (ui_clicked(mx, my, btn_suivant) && reponse_ok != -1) {
                    q_index++;
                    if (q_index >= NB_QUESTIONS_PHYSIQUE) {
                        termine = true;
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
                if (ui_clicked(mx, my, (SDL_Rect){310, 430, 180, 45})) {
                    q_index = 0; score = 0;
                    reponse_sel = -1; reponse_ok = -1;
                    termine = false;
                    memset(message, 0, sizeof(message));
                }
                if (ui_clicked(mx, my, (SDL_Rect){510, 430, 180, 45}))
                    return 0;
            }
        }

        if (msg_timer > 0) msg_timer--;

        // ===== RENDU =====
        ui_draw_rect(renderer, (SDL_Rect){210, 0, 590, 600},
                     (SDL_Color){30, 30, 45, 255});

        SDL_Rect header = {210, 0, 590, 60};
        ui_draw_rect(renderer, header, bleu);
        ui_render_text_centered(renderer, font, "PHYSIQUE", header, blanc);

        if (termine) {
            char resultat[100];
            snprintf(resultat, sizeof(resultat), "Score : %d / %d",
                     score, NB_QUESTIONS_PHYSIQUE);
            ui_render_text_centered(renderer, font, resultat,
                                    (SDL_Rect){210, 200, 590, 60}, blanc);

            const char *appr; SDL_Color col_a;
            if      (score >= 8) { appr = "Excellent !";        col_a = vert;  }
            else if (score >= 5) { appr = "Bien ! Continuez !"; col_a = bleu;  }
            else                 { appr = "A revoir !";          col_a = rouge; }

            ui_render_text_centered(renderer, font, appr,
                                    (SDL_Rect){210, 270, 590, 40}, col_a);

            ui_draw_bouton(renderer, font,
                (Bouton){{310, 430, 180, 45}, "Recommencer", vert,  blanc});
            ui_draw_bouton(renderer, font,
                (Bouton){{510, 430, 180, 45}, "Menu",        rouge, blanc});

        } else {
            char prog[40];
            snprintf(prog, sizeof(prog), "Question %d / %d",
                     q_index + 1, NB_QUESTIONS_PHYSIQUE);
            ui_render_text(renderer, font, prog, 220, 70, gris);

            // Barre progression
            ui_draw_rect(renderer, (SDL_Rect){220, 95, 560, 12},
                         (SDL_Color){60, 60, 80, 255});
            ui_draw_rect(renderer,
                (SDL_Rect){220, 95,
                           (560 * q_index) / NB_QUESTIONS_PHYSIQUE, 12},
                bleu);

            char sc[20];
            snprintf(sc, sizeof(sc), "Score : %d", score);
            ui_render_text(renderer, font, sc, 650, 70, blanc);

            // Enonce
            ui_draw_rect(renderer, (SDL_Rect){220, 115, 560, 90},
                         (SDL_Color){40, 40, 60, 255});
            ui_render_text_centered(renderer, font,
                questions_physique[q_index].enonce,
                (SDL_Rect){220, 115, 560, 90}, blanc);

            // Choix
            for (int i = 0; i < 4; i++) {
                SDL_Rect r = {220, 220 + (i * 70), 560, 55};
                SDL_Color col;
                if      (reponse_ok == -1) col = (SDL_Color){50, 50, 75, 255};
                else if (i == questions_physique[q_index].bonne_reponse)
                    col = vert;
                else if (i == reponse_sel) col = rouge;
                else                       col = (SDL_Color){50, 50, 75, 255};

                ui_draw_rect(renderer, r, col);
                ui_draw_border(renderer, r, (SDL_Color){80, 80, 110, 255}, 1);
                ui_render_text_centered(renderer, font,
                    questions_physique[q_index].choix[i], r, blanc);
            }

            // Message
            if (msg_timer > 0) {
                SDL_Color cm = (reponse_ok == 1) ? vert : rouge;
                ui_draw_message(renderer, font, message,
                                220, 505, cm, &msg_timer);
            }

            // Bouton Abandonner
            ui_draw_bouton(renderer, font,
                (Bouton){{220, 530, 160, 40}, "Abandonner", rouge, blanc});

            // Bouton Suivant
            if (reponse_ok != -1) {
                ui_draw_bouton(renderer, font,
                    (Bouton){{560, 530, 180, 45},
                    (q_index == NB_QUESTIONS_PHYSIQUE-1) ? "Terminer"
                                                         : "Suivant ->",
                    vert, blanc});
            } else {
                ui_draw_rect(renderer, (SDL_Rect){560, 530, 180, 45},
                             (SDL_Color){80, 80, 80, 255});
                ui_render_text_centered(renderer, font, "Suivant ->",
                    (SDL_Rect){560, 530, 180, 45}, gris);
            }
        }

        SDL_RenderPresent(renderer);
    }
}