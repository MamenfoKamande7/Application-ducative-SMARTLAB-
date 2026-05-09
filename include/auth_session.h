#ifndef AUTH_SESSION_H
#define AUTH_SESSION_H
#include <stddef.h> 
#include "auth.h"

/* Génération d’un token aléatoire pour session */
void auth_generate_token(char *out_token, size_t size);

#endif