#ifndef __TOKEN_
#define __TOKEN_

#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef enum
{
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_UNKNOWN,
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_FAILURE,
    TOKEN_TERM
} TokenType;

// Definizione della struttura Token per la lista linkata
typedef struct Token
{
    TokenType type;
    char *value;
    struct Token *next; // Puntatore al next nodo nella lista
} Token;

typedef struct TokenReturn
{
    Boolean esito;
    unsigned char messaggio;
    Token *token;
} TokenReturn;

Token *create_token_node(char *value, TokenType type);
Token *append(Token *testa, char *valore, TokenType type);
void free_all(Token *testa);
Token *pop(Token *testa);
Token *tokenize(char *input);
TokenReturn *eat(TokenReturn *res, TokenType expected_type);
int is_operator(TokenType);


#endif