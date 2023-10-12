#ifndef __PARSER_
#define __PARSER_

#include <stdio.h>
#include "ast.h"

Node *expr(TokenReturn *);
Node *factor(TokenReturn *res);
Node *term(TokenReturn *res);
Node *parse(TokenReturn *res);

#endif