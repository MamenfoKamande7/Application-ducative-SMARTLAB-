#include "modules.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSV_LINE_MAX 256

// ===================== SAUVEGARDE =====================
int score_save(const char *csv_path, Score *s) {
    if (!csv_path || !s) return -1;

    FILE *f = fopen(csv_path, "a");
    if (!f) return -1;

    const char *module_str[] = {"maths", "physique", "chimie"};

    fprintf(f, "%s;%s;%d;%d\n",
            s->email,
            module_str[s->module],
            s->score,
            s->total);

    fclose(f);
    return 0;
}

// ===================== CHARGEMENT =====================
int score_load(const char *csv_path, const char *email,
               Score *out, int max) {
    if (!csv_path || !email || !out) return 0;

    FILE *f = fopen(csv_path, "r");
    if (!f) return 0;

    const char *module_str[] = {"maths", "physique", "chimie"};

    char line[CSV_LINE_MAX];
    int  count = 0;

    while (fgets(line, sizeof(line), f) && count < max) {
        line[strcspn(line, "\n")] = '\0';

        char copy[CSV_LINE_MAX];
        strncpy(copy, line, sizeof(copy));

        char *t_email  = strtok(copy, ";");
        char *t_module = strtok(NULL, ";");
        char *t_score  = strtok(NULL, ";");
        char *t_total  = strtok(NULL, ";");

        if (!t_email || !t_module ||
            !t_score || !t_total) continue;

        if (strcmp(t_email, email) != 0) continue;

        ModuleID mid = MODULE_MATHS;
        for (int i = 0; i < MODULE_COUNT; i++) {
            if (strcmp(t_module, module_str[i]) == 0) {
                mid = (ModuleID)i;
                break;
            }
        }

        strncpy(out[count].email, t_email, 99);
        out[count].module = mid;
        out[count].score  = atoi(t_score);
        out[count].total  = atoi(t_total);
        count++;
    }

    fclose(f);
    return count;
}

// ===================== MEILLEUR SCORE =====================
int score_best(const char *csv_path, const char *email,
               ModuleID module) {
    Score scores[100];
    int   n    = score_load(csv_path, email, scores, 100);
    int   best = -1;

    for (int i = 0; i < n; i++) {
        if (scores[i].module == module &&
            scores[i].score  > best)
            best = scores[i].score;
    }

    return best;
}