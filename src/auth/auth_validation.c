#include "auth.h"
#include <string.h>
#include <ctype.h>

/* Vérifie email simple */
int auth_is_valid_email(const char *email)
{
    if (!email || strlen(email) < 5) return 0;
    return strchr(email, '@') != NULL;
}

/* Vérifie téléphone simple */
int auth_is_valid_phone(const char *phone)
{
    if (!phone || strlen(phone) < 8) return 0;
    for (size_t i = 0; i < strlen(phone); i++)
        if (!isdigit(phone[i])) return 0;
    return 1;
}

/* Hash simplifié du mot de passe */
void auth_hash_password(const char *password, char *out_hash)
{
    size_t len = strlen(password);
    for (size_t i = 0; i < len && i < AUTH_MAX_PASSWORD; i++)
        out_hash[i] = password[i] ^ 0xAA;  /* XOR simple pour demo */
    out_hash[len] = '\0';
}