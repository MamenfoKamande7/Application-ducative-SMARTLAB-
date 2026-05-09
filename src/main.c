#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "auth.h"
#include "modules.h"
#include "ui.h"

#define WIDTH  800
#define HEIGHT 600

// ===================== ÉTATS =====================
typedef enum {
    STATE_SIGNUP,
    STATE_LOGIN,
    STATE_MENU,
    STATE_EXIT
} AppState;

// Session globale
AuthSession current_session;

// ===================== PROTOTYPES =====================
AppState signup_screen(SDL_Renderer *renderer, TTF_Font *font);
AppState login_screen(SDL_Renderer *renderer, TTF_Font *font,
                      AuthSession *session);

// ===================== UTILITAIRES =====================
static bool isClicked(int mx, int my, SDL_Rect r) {
    return (mx > r.x && mx < r.x + r.w &&
            my > r.y && my < r.y + r.h);
}

static void renderText(SDL_Renderer *renderer, TTF_Font *font,
                       const char *text, int x, int y,
                       SDL_Color color) {
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

// ===================== SIGNUP =====================
AppState signup_screen(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event e;
    char fields[5][50] = {"", "", "", "", ""};
    char *labels[5]    = {"Nom", "Prenom", "Email",
                          "Mot de passe", "Confirmation"};
    int  active       = 0;
    char message[100] = "";
    int  msg_timer    = 0;

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color noir  = {0,   0,   0,   255};
    SDL_Color rouge = {231, 76,  60,  255};
    SDL_Color bleu  = {52,  152, 219, 255};

    SDL_StartTextInput();

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return STATE_EXIT;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;

                for (int i = 0; i < 5; i++) {
                    SDL_Rect r = {200, 80 + (i * 85), 400, 40};
                    if (isClicked(mx, my, r)) active = i;
                }

                SDL_Rect btnVal = {300, 520, 200, 40};
                if (isClicked(mx, my, btnVal)) {
                    bool vide = false;
                    for (int i = 0; i < 5; i++)
                        if (strlen(fields[i]) == 0) {
                            vide = true; break;
                        }

                    if (vide) {
                        strcpy(message,
                               "Remplissez tous les champs !");
                        msg_timer = 180;
                    } else if (strchr(fields[2], '@') == NULL) {
                        strcpy(message, "Email invalide !");
                        msg_timer = 180;
                    } else if (strcmp(fields[3], fields[4]) != 0) {
                        strcpy(message,
                               "Mots de passe differents !");
                        msg_timer = 180;
                    } else {
                        AuthStatus res = auth_signin(
                            "database/users.csv",
                            fields[0], fields[1],
                            fields[2], fields[3], fields[4]
                        );
                        if (res == AUTH_OK) {
                            return STATE_LOGIN;
                        } else if (res == AUTH_ERR_EXISTS) {
                            strcpy(message, "Email deja utilise !");
                            msg_timer = 180;
                        } else {
                            strcpy(message,
                                   "Erreur enregistrement !");
                            msg_timer = 180;
                        }
                    }
                }

                SDL_Rect btnLogin = {280, 570, 240, 25};
                if (isClicked(mx, my, btnLogin))
                    return STATE_LOGIN;
            }

            if (e.type == SDL_TEXTINPUT) {
                if (strlen(fields[active]) < 49)
                    strcat(fields[active], e.text.text);
            }

            if (e.type == SDL_KEYDOWN &&
                e.key.keysym.sym == SDLK_BACKSPACE) {
                size_t len = strlen(fields[active]);
                if (len > 0) fields[active][len - 1] = '\0';
            }
        }

        if (msg_timer > 0) msg_timer--;

        SDL_SetRenderDrawColor(renderer, 30, 30, 45, 255);
        SDL_RenderClear(renderer);

        renderText(renderer, font,
                   "CREER UN COMPTE SMARTLAB", 220, 20, blanc);

        SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
        SDL_RenderDrawLine(renderer, 200, 55, 600, 55);

        for (int i = 0; i < 5; i++) {
            int y = 80 + (i * 85);
            renderText(renderer, font, labels[i],
                       200, y - 22, blanc);

            SDL_Rect r = {200, y, 400, 40};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &r);

            if (i == active) {
                SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
                SDL_Rect border = {198, y - 2, 404, 44};
                SDL_RenderDrawRect(renderer, &border);
            }

            if (i < 3) {
                renderText(renderer, font, fields[i],
                           210, y + 10, noir);
            } else {
                char stars[50] = "";
                for (size_t j = 0; j < strlen(fields[i]); j++)
                    strcat(stars, "*");
                renderText(renderer, font, stars,
                           210, y + 10, noir);
            }
        }

        SDL_Rect btnVal = {300, 520, 200, 40};
        SDL_SetRenderDrawColor(renderer, 46, 204, 113, 255);
        SDL_RenderFillRect(renderer, &btnVal);
        renderText(renderer, font, "VALIDER", 368, 530, blanc);

        if (msg_timer > 0)
            renderText(renderer, font, message, 200, 480, rouge);

        renderText(renderer, font, "J'ai deja un compte",
                   290, 573, bleu);

        SDL_RenderPresent(renderer);
    }
}

// ===================== LOGIN =====================
AppState login_screen(SDL_Renderer *renderer, TTF_Font *font,
                      AuthSession *session) {
    SDL_Event e;
    char fields[2][50] = {"", ""};
    int  active        = 0;
    char message[100]  = "";
    int  msg_timer     = 0;

    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color noir  = {0,   0,   0,   255};
    SDL_Color rouge = {231, 76,  60,  255};
    SDL_Color bleu  = {52,  152, 219, 255};

    SDL_StartTextInput();

    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return STATE_EXIT;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;

                for (int i = 0; i < 2; i++) {
                    SDL_Rect r = {200, 200 + (i * 110), 400, 45};
                    if (isClicked(mx, my, r)) active = i;
                }

                SDL_Rect btnLogin = {300, 430, 200, 45};
                if (isClicked(mx, my, btnLogin)) {
                    if (strlen(fields[0]) == 0 ||
                        strlen(fields[1]) == 0) {
                        strcpy(message,
                               "Remplissez tous les champs !");
                        msg_timer = 180;
                    } else if (strchr(fields[0], '@') == NULL) {
                        strcpy(message, "Email invalide !");
                        msg_timer = 180;
                    } else {
                        AuthStatus res = auth_login_with_session(
                            "database/users.csv",
                            fields[0], fields[1], session
                        );
                        if (res == AUTH_OK) return STATE_MENU;
                        strcpy(message,
                               "Email ou mot de passe incorrect !");
                        msg_timer = 180;
                    }
                }

                SDL_Rect btnSignup = {270, 490, 260, 25};
                if (isClicked(mx, my, btnSignup))
                    return STATE_SIGNUP;
            }

            if (e.type == SDL_TEXTINPUT) {
                if (strlen(fields[active]) < 49)
                    strcat(fields[active], e.text.text);
            }

            if (e.type == SDL_KEYDOWN &&
                e.key.keysym.sym == SDLK_BACKSPACE) {
                size_t len = strlen(fields[active]);
                if (len > 0) fields[active][len - 1] = '\0';
            }
        }

        if (msg_timer > 0) msg_timer--;

        SDL_SetRenderDrawColor(renderer, 30, 30, 45, 255);
        SDL_RenderClear(renderer);

        renderText(renderer, font,
                   "CONNEXION SMARTLAB", 270, 50, blanc);

        SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
        SDL_RenderDrawLine(renderer, 200, 85, 600, 85);

        char *labels[2] = {"Email", "Mot de passe"};
        for (int i = 0; i < 2; i++) {
            int y = 200 + (i * 110);
            renderText(renderer, font, labels[i],
                       200, y - 25, blanc);

            SDL_Rect r = {200, y, 400, 45};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &r);

            if (i == active) {
                SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
                SDL_Rect border = {198, y - 2, 404, 49};
                SDL_RenderDrawRect(renderer, &border);
            }

            if (i == 0) {
                renderText(renderer, font, fields[i],
                           210, y + 12, noir);
            } else {
                char stars[50] = "";
                for (size_t j = 0; j < strlen(fields[i]); j++)
                    strcat(stars, "*");
                renderText(renderer, font, stars,
                           210, y + 12, noir);
            }
        }

        SDL_Rect btnLogin = {300, 430, 200, 45};
        SDL_SetRenderDrawColor(renderer, 52, 152, 219, 255);
        SDL_RenderFillRect(renderer, &btnLogin);
        renderText(renderer, font, "SE CONNECTER",
                   322, 442, blanc);

        if (msg_timer > 0)
            renderText(renderer, font, message, 200, 390, rouge);

        renderText(renderer, font, "Pas de compte ? S'inscrire",
                   250, 493, bleu);

        SDL_RenderPresent(renderer);
    }
}
int menu_screen(SDL_Renderer *renderer, TTF_Font *font, AuthSession *session);

// ===================== MAIN =====================
int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("Erreur TTF: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "SmartLab",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        printf("Erreur fenetre: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );
    if (!renderer) {
        printf("Erreur renderer: %s\n", SDL_GetError());
        return 1;
    }

    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

    TTF_Font *font = TTF_OpenFont("assets/arial.ttf", 18);
    if (!font) {
        printf("Erreur police: %s\n", TTF_GetError());
        return 1;
    }

    // Créer les fichiers database si absents
    auth_init("database/users.csv");

    AppState state = STATE_SIGNUP;

    while (state != STATE_EXIT) {
        switch (state) {
            case STATE_SIGNUP:
                state = signup_screen(renderer, font);
                break;
            case STATE_LOGIN:
                state = login_screen(renderer, font,
                                     &current_session);
                break;
            case STATE_MENU: {
                int ret = menu_screen(renderer, font,
                                      &current_session);
                state = (ret == 1) ? STATE_LOGIN : STATE_EXIT;
                break;
            }
            default:
                state = STATE_EXIT;
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}