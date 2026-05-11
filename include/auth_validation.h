#ifndef AUTH_VALIDATION_H
#define AUTH_VALIDATION_H

#include "auth.h"

/* Validation email / phone */
int auth_is_valid_email(const char *email);
int auth_is_valid_phone(const char *phone);

/* Hash simple */
void auth_hash_password(const char *password, char *out_hash);

#endif