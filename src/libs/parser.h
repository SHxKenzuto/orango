#ifndef __PARSER_
#define __PARSER_

#include <stdio.h>
#include "ast.h"
#include "vars.h"


Node *expr(TokenReturn **, TokenType*, TokenType);
Node *factor(TokenReturn **);
Node *term(TokenReturn **, TokenType*);
Node *parse(TokenReturn **);
Node *main_parse(TokenReturn **);
Node *id_parse(TokenReturn **);
Node *var_expr(TokenReturn**);
Boolean compareTokenType(TokenType*, TokenType);


#endif