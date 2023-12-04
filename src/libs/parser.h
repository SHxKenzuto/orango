#ifndef __PARSER_
#define __PARSER_

#include <stdio.h>
#include "ast.h"
#include "vars.h"

Node *expr(TokenReturn **);
Node *factor(TokenReturn **);
Node *term(TokenReturn **);
Node *parse(TokenReturn **);
Node *main_parse(TokenReturn **);
Node *id_parse(TokenReturn **);
Node *var_expr(TokenReturn**);


#endif