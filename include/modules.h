#ifndef MODULES_H
#define MODULES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// ===================== IDENTIFIANTS MATIÈRES =====================
typedef enum {
    MODULE_MATHS    = 0,
    MODULE_PHYSIQUE = 1,
    MODULE_CHIMIE   = 2,
    MODULE_COUNT    = 3
} ModuleID;

// ===================== NIVEAU =====================
typedef enum {
    NIVEAU_DEBUTANT      = 0,
    NIVEAU_INTERMEDIAIRE = 1,
    NIVEAU_AVANCE        = 2
} Niveau;

// ===================== QUESTION QCM =====================
typedef struct {
    char enonce[256];
    char choix[4][100];
    int  bonne_reponse;
    char explication[256];
} Question;

// ===================== SCORE =====================
typedef struct {
    char     email[100];
    ModuleID module;
    int      score;
    int      total;
} Score;

// ===================== PROTOTYPES MATHS =====================
int maths_screen(SDL_Renderer *renderer, TTF_Font *font);
int maths_get_questions(Question *out, int max);

// ===================== PROTOTYPES PHYSIQUE =====================
int physique_screen(SDL_Renderer *renderer, TTF_Font *font);
int physique_get_questions(Question *out, int max);

// ===================== PROTOTYPES CHIMIE =====================
int chimie_screen(SDL_Renderer *renderer, TTF_Font *font);
int chimie_get_questions(Question *out, int max);

// ===================== PROTOTYPES SCORES =====================
int score_save(const char *csv_path, Score *s);
int score_load(const char *csv_path, const char *email,
               Score *out, int max);
int score_best(const char *csv_path, const char *email,
               ModuleID module);

#endif