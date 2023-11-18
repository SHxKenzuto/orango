#ifndef __UTILS_
#define __UTILS__

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"

Boolean strcmpDecente(const char *, const char *);
Boolean isNumber(const char *);
Boolean is_op(char);
char *strtok_decente(char *);

#endif