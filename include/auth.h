#ifndef AUTH_H
#define AUTH_H

#include <stddef.h>

#ifdef _WIN32
#define AUTH_API __declspec(dllexport)
#else
#define AUTH_API
#endif

// --- Constantes de taille ---
#define AUTH_MAX_NOM      50
#define AUTH_MAX_PRENOM   50
#define AUTH_MAX_EMAIL    100
#define AUTH_MAX_PASSWORD 128
#define AUTH_MAX_ROLE     50
#define AUTH_MAX_TOKEN    128

// --- Codes d'erreurs ---
typedef enum {
    AUTH_OK          = 0,
    AUTH_ERR_INVALID = 1,
    AUTH_ERR_EXISTS  = 2,
    AUTH_ERR_NOTFOUND = 3,
    AUTH_ERR_IO      = 4
} AuthStatus;

// --- Structure de Session ---
typedef struct {
    char nom[AUTH_MAX_NOM + 1];
    char prenom[AUTH_MAX_PRENOM + 1];
    char email[AUTH_MAX_EMAIL + 1];
    char role[AUTH_MAX_ROLE + 1];
    char token[AUTH_MAX_TOKEN + 1];
} AuthSession;

// --- Fonctions API ---
AUTH_API AuthStatus auth_init(const char *csv_path);

AUTH_API AuthStatus auth_signin(const char *csv_path,
                                const char *nom,
                                const char *prenom,
                                const char *email,
                                const char *password,
                                const char *confirm_password);

AUTH_API AuthStatus auth_login_with_session(const char *csv_path,
                                            const char *email,
                                            const char *password,
                                            AuthSession *out_session);

#endif