#include "auth_session.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void auth_generate_token(char *out_token, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    if (size == 0) return;
    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < size-1; i++) {
        out_token[i] = charset[rand() % (sizeof(charset)-1)];
    }
    out_token[size-1] = '\0';
}