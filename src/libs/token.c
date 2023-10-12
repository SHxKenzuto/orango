#include "token.h"

// Funzione per creare un nuovo nodo
Token *create_token_node(char *value, TokenType type)
{
    Token *nuovo_nodo = (Token *)malloc(sizeof(Token));
    nuovo_nodo->value = strdup(value);
    nuovo_nodo->type = type;
    nuovo_nodo->next = NULL;
    return nuovo_nodo;
}

// Funzione per inserire un nodo alla fine della lista
Token *append(Token *testa, char *valore, TokenType type)
{
    Token *nuovo_nodo = create_token_node(valore, type);
    if (testa == NULL)
    {
        return nuovo_nodo;
    }
    else
    {
        Token *temp = testa;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = nuovo_nodo;
        return testa;
    }
}

// Funzione per liberare la memoria occupata dalla lista
void free_all(Token *testa)
{
    Token *temp;
    while (testa != NULL)
    {
        temp = testa;
        testa = testa->next;
        free(temp);
    }
}

// Funzione per liberare la memoria occupata dalla lista
Token *pop(Token *testa)
{
    Token *temp;
    temp = testa;
    testa = testa->next;
    return temp;
}

// Funzione per tokenizzare l'input
Token *tokenize(char *input)
{
    Token *tokens = NULL;
    char *tokenValue = strtok(input, " ");
    while (tokenValue != NULL)
    {
        TokenType type;
        if (strcmpDecente(tokenValue, "="))
            type = TOKEN_ASSIGN;
        else if (strcmpDecente(tokenValue, "+"))
            type = TOKEN_PLUS;
        else if (strcmpDecente(tokenValue, "*"))
            type = TOKEN_MULT;
        else if (isdigit(tokenValue[0]))
            type = TOKEN_NUMBER; // gestire i char
        else
            type = TOKEN_IDENTIFIER;
        tokens = append(tokens, tokenValue, type);
        tokenValue = strtok(NULL, " ");
    }
    return tokens;
}

TokenReturn *eat(TokenReturn *res, TokenType expected_type)
{
    // TokenReturn* res = malloc(sizeof(TokenReturn));
    res->esito = FALSE;
    Token *token = res->token;
    if (token != NULL && token->type == expected_type)
    {
        res->token = token->next;
        res->esito = TRUE,
        free(token);
        res->messaggio = 0x0000;
        return res;
    }
    res->messaggio = 0x0001;
    return res;
}