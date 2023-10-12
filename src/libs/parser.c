#include "parser.h"
Node *factor(TokenReturn *res)
{
    if (res->esito && res->token != NULL)
    {
        Node *n = NULL;
        Token *current_token = res->token;
        switch (current_token->type)
        {
        case TOKEN_NUMBER:
            char *num = strdup(current_token->value);
            res = eat(res, TOKEN_NUMBER);
            if (res->esito)
            {
                n = create_ast_node(TOKEN_NUMBER, strdup(num), NULL, NULL);
            }
            break;
        case TOKEN_LPAREN:
            res = eat(res, TOKEN_LPAREN);
            if (res->esito)
            {
                n = expr(res);
            }
            res = eat(res, TOKEN_RPAREN);
            if (!(res->esito && res->token != NULL))
            {
                printf(">DEBUG FACTOR: %x\n", res->messaggio);
                n = NULL;
            }
        default:
            break;
        }
        printf(">DEBUG FACTOR: %x\n", res->messaggio);
        return n;
    }
    printf(">DEBUG FACTOR: %x\n", res->messaggio);
    return NULL;
}

Node *term(TokenReturn *res)
{
    Node *n = factor(res);
    if (n != NULL && res->token != NULL)
    {
        TokenType op = res->token->type;
        res = eat(res, TOKEN_MULT);
        if (!(res->esito))
        {
            res = eat(res, TOKEN_DIV);
        }
        if (res->token != NULL)
        {
            if (res->esito)
            {
                return create_ast_node(op, "*/", term(res), n);
            }
            else if (res->token->type == TOKEN_PLUS || res->token->type == TOKEN_MINUS)
            {
                res->esito = TRUE;
                res->messaggio = 0x0000;
            }
        }
    }
    printf(">DEBUG TERM: %x\n", res->messaggio);
    return n;
}

Node *expr(TokenReturn *res)
{
    Node *n = term(res);
    if (n != NULL && res->token != NULL)
    {
        TokenType op = res->token->type;
        res = eat(res, TOKEN_PLUS);
        if (!(res->esito))
        {
            res = eat(res, TOKEN_MINUS);
        }
        if (res->esito && res->token != NULL)
        {
            return create_ast_node(op, "+-", expr(res), n);
        }
        printf(">DEBUG EXPR: %x\n", res->messaggio);
        return n;
    }
    printf(">DEBUG EXPR: %x\n", res->messaggio);
    return n;
}

// Funzione per costruire l'AST
Node *parse(TokenReturn *res)
{
    if (res->esito && res->token != NULL)
    {
        Token *current_token = res->token;
        Node *n = NULL;
        switch (current_token->type)
        {
        case TOKEN_IDENTIFIER:
            char *id = strdup(current_token->value);
            res = eat(res, TOKEN_IDENTIFIER);
            if (res->esito && res->token != NULL)
            {
                res = eat(res, TOKEN_ASSIGN);
                if (res->esito && res->token != NULL)
                {
                    n = create_ast_node(TOKEN_ASSIGN, "=", create_ast_node(TOKEN_IDENTIFIER, id, NULL, NULL), expr(res));
                }
                else
                {
                    printf(">DEBUG EXPR: %02hhx\n", res->messaggio);
                }
            }
            else
            {
                printf(">DEBUG EXPR: %02hhx\n", res->messaggio);
            }
            break;
        case TOKEN_NUMBER:
        case TOKEN_LPAREN:
            n = expr(res);
        default:
            break;
        }
        return n;
    }
    printf(">DEBUG EXPR: %02hhx\n", res->messaggio);
    return NULL;
}