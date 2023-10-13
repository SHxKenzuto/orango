#ifndef __PARSER_
#define __PARSER_

#include <stdio.h>
#include "ast.h"

Node *expr(TokenReturn *);
Node *factor(TokenReturn *);
Node *term(TokenReturn *);
Node *parse(TokenReturn *);

#endif