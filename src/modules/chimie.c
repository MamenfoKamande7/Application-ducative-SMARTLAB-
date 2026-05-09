#include "modules.h"
#include "ui.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

// ================= QUESTIONS =================
static Question questions_chimie[] = {
    {"Quel est le symbole chimique de l'or ?", {"Or","Au","Ag","Go"},1,"Au vient du latin Aurum."},
    {"Quelle est la formule de l'eau ?", {"HO","H2O","H2O2","OH"},1,"2 H + 1 O"},
    {"Quel gaz represente ~78% de l'atmosphere ?", {"Oxygene","CO2","Argon","Azote"},3,"Azote = 78%"},
    {"Qu'est-ce que le pH mesure ?", {"Temperature","Pression","Acidite","Densite"},2,"Acidite/basicite"},
    {"Numero atomique du carbone ?", {"6","12","14","8"},0,"6 protons"},
    {"Liaison dans H2 ?", {"Ionique","Covalente","Metallique","Van der Waals"},1,"Covalente"},
    {"Combustion complete du carbone ?", {"CO","CO2","C+e-","2CO"},1,"Donne CO2"},
    {"Masse molaire eau ?", {"16","18","20","22"},1,"18 g/mol"},
    {"Symbole du fer ?", {"Fe","Fr","F","Fa"},0,"Ferrum"},
    {"Acide selon Bronsted ?", {"Accepteur H+","Donneur H+","Donneur e-","Accepteur e-"},1,"Donneur H+"}
};

#define NB_QUESTIONS_CHIMIE 10

// ================= SCREEN =================
int chimie_screen(SDL_Renderer *renderer, TTF_Font *font) {

    SDL_Color blanc = {255,255,255,255};
    SDL_Color vert  = {46,204,113,255};
    SDL_Color rouge = {231,76,60,255};
    SDL_Color orange= {230,126,34,255};
    SDL_Color gris  = {149,165,166,255};

    int q_index = 0, score = 0;
    int reponse_sel = -1, reponse_ok = -1;
    bool termine = false;

    char message[256] = "";
    int msg_timer = 0;

    SDL_Event e;

    while (1) {

        // ================= EVENTS =================
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT)
                return 0;

            if (e.type == SDL_MOUSEBUTTONDOWN) {

                int mx = e.button.x;
                int my = e.button.y;

                // abandon
                SDL_Rect btn_abandon = {220,530,160,40};
                if (ui_clicked(mx,my,btn_abandon))
                    return 1;

                if (!termine) {

                    // réponses
                    if (reponse_ok == -1) {
                        for (int i=0;i<4;i++) {
                            SDL_Rect r = {220,220+(i*70),560,55};

                            if (ui_clicked(mx,my,r)) {
                                reponse_sel = i;

                                if (i == questions_chimie[q_index].bonne_reponse) {
                                    reponse_ok = 1;
                                    score++;
                                    strcpy(message,"Bonne reponse !");
                                } else {
                                    reponse_ok = 0;
                                    strcpy(message,questions_chimie[q_index].explication);
                                }

                                msg_timer = 180;
                            }
                        }
                    }

                    // bouton suivant
                    SDL_Rect btn_suivant = {560,530,180,45};
                    if (ui_clicked(mx,my,btn_suivant) && reponse_ok != -1) {

                        q_index++;

                        if (q_index >= NB_QUESTIONS_CHIMIE)
                            termine = true;
                        else {
                            reponse_sel = -1;
                            reponse_ok = -1;
                            msg_timer = 0;
                            memset(message,0,sizeof(message));
                        }
                    }
                }
                else {
                    // recommencer
                    if (ui_clicked(mx,my,(SDL_Rect){310,430,180,45})) {
                        q_index = 0;
                        score = 0;
                        reponse_sel = -1;
                        reponse_ok = -1;
                        termine = false;
                        memset(message,0,sizeof(message));
                    }

                    // menu
                    if (ui_clicked(mx,my,(SDL_Rect){510,430,180,45}))
                        return 1;
                }
            }
        }

        if (msg_timer > 0) msg_timer--;

        // ================= RENDER =================
        ui_draw_rect(renderer,(SDL_Rect){210,0,590,600},
                     (SDL_Color){30,30,45,255});

        // header
        SDL_Rect header = {210,0,590,60};
        ui_draw_rect(renderer,header,orange);
        ui_render_text_centered(renderer,font,"CHIMIE",header,blanc);

        // ================= FIN =================
        if (termine) {

            char resultat[100];
            snprintf(resultat,sizeof(resultat),
                     "Score : %d / %d",score,NB_QUESTIONS_CHIMIE);

            ui_render_text_centered(renderer,font,resultat,
                                    (SDL_Rect){210,200,590,60},blanc);

            ui_draw_bouton(renderer,font,
                (Bouton){{310,430,180,45},"Recommencer",vert,blanc});

            ui_draw_bouton(renderer,font,
                (Bouton){{510,430,180,45},"Menu",rouge,blanc});
        }

        // ================= JEU =================
        else {

            char prog[30];
            snprintf(prog,sizeof(prog),"Question %d/%d",
                     q_index+1,NB_QUESTIONS_CHIMIE);

            ui_render_text(renderer,font,prog,220,70,gris);

            ui_render_text(renderer,font,
                questions_chimie[q_index].enonce,220,120,blanc);

            for (int i=0;i<4;i++) {

                SDL_Rect r = {220,220+(i*70),560,55};

                SDL_Color col = {50,50,75,255};

                if (reponse_ok != -1) {
                    if (i == questions_chimie[q_index].bonne_reponse)
                        col = vert;
                    else if (i == reponse_sel)
                        col = rouge;
                }

                ui_draw_rect(renderer,r,col);

                ui_render_text_centered(renderer,font,
                    questions_chimie[q_index].choix[i],r,blanc);
            }

            if (reponse_ok != -1) {
                ui_draw_bouton(renderer,font,
                    (Bouton){{560,530,180,45},"Suivant",vert,blanc});
            }
        }

        // ================= ABANDON (TOUJOURS) =================
        ui_draw_bouton(renderer,font,
            (Bouton){{220,530,160,40},
            "Abandonner",
            (SDL_Color){231,76,60,255},
            blanc});

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return 0;
}