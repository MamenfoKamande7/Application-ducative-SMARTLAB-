#include "auth.h"
#include "auth_validation.h"   // ← utilise les fonctions d'ici
#include "auth_session.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CSV_LINE_MAX 512

// ===================== AUTH INIT =====================
AuthStatus auth_init(const char *csv_path) {
    FILE *f = fopen(csv_path, "a");
    if (!f) return AUTH_ERR_IO;
    fclose(f);
    return AUTH_OK;
}

// ===================== INSCRIPTION =====================
AuthStatus auth_signin(const char *csv_path,
                       const char *nom,
                       const char *prenom,
                       const char *email,
                       const char *password,
                       const char *confirm_password) {
    if (!nom    || strlen(nom)    == 0) return AUTH_ERR_INVALID;
    if (!prenom || strlen(prenom) == 0) return AUTH_ERR_INVALID;
    if (!auth_is_valid_email(email))    return AUTH_ERR_INVALID;
    if (!password || strlen(password)  == 0) return AUTH_ERR_INVALID;
    if (strcmp(password, confirm_password) != 0) return AUTH_ERR_INVALID;

    // Vérifier email existant
    FILE *f = fopen(csv_path, "r");
    if (f) {
        char line[CSV_LINE_MAX];
        while (fgets(line, sizeof(line), f)) {
            line[strcspn(line, "\n")] = '\0';
            char copy[CSV_LINE_MAX];
            strncpy(copy, line, sizeof(copy));
            strtok(copy, ";");
            strtok(NULL, ";");
            char *t_email = strtok(NULL, ";");
            if (t_email && strcmp(t_email, email) == 0) {
                fclose(f);
                return AUTH_ERR_EXISTS;
            }
        }
        fclose(f);
    }

    // Écrire l'utilisateur
    f = fopen(csv_path, "a");
    if (!f) return AUTH_ERR_IO;

    char hash[AUTH_MAX_PASSWORD];
    auth_hash_password(password, hash);

    fprintf(f, "%s;%s;%s;%s;user;1\n",
            nom, prenom, email, hash);
    fclose(f);
    return AUTH_OK;
}

// ===================== CONNEXION =====================
AuthStatus auth_login_with_session(const char *csv_path,
                                   const char *email,
                                   const char *password,
                                   AuthSession *out_session) {
    if (!email || !password || !out_session) return AUTH_ERR_INVALID;

    FILE *f = fopen(csv_path, "r");
    if (!f) return AUTH_ERR_IO;

    char input_hash[AUTH_MAX_PASSWORD];
    auth_hash_password(password, input_hash);

    char line[CSV_LINE_MAX];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';

        char copy[CSV_LINE_MAX];
        strncpy(copy, line, sizeof(copy));

        char *t_nom    = strtok(copy, ";");
        char *t_prenom = strtok(NULL,  ";");
        char *t_email  = strtok(NULL,  ";");
        char *t_hash   = strtok(NULL,  ";");
        char *t_role   = strtok(NULL,  ";");
        char *t_actif  = strtok(NULL,  ";");

        if (!t_nom || !t_prenom || !t_email ||
            !t_hash || !t_role  || !t_actif) continue;

        if (strcmp(t_email, email)      == 0 &&
            strcmp(t_hash,  input_hash) == 0) {

            if (strcmp(t_actif, "1") != 0) {
                fclose(f);
                return AUTH_ERR_INVALID;
            }

            strncpy(out_session->nom,    t_nom,    AUTH_MAX_NOM);
            strncpy(out_session->prenom, t_prenom, AUTH_MAX_PRENOM);
            strncpy(out_session->email,  t_email,  AUTH_MAX_EMAIL);
            strncpy(out_session->role,   t_role,   AUTH_MAX_ROLE);
            auth_generate_token(out_session->token, AUTH_MAX_TOKEN);

            fclose(f);
            return AUTH_OK;
        }
    }

    fclose(f);
    return AUTH_ERR_NOTFOUND;
}