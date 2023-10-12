#include "interpreter.h"

// Funzione per interpretare l'AST
int interpret(Node *ast)
{
    int res = 0;
    switch (ast->type)
    {
    case TOKEN_MULT:
        res = interpret(ast->left) * interpret(ast->right);
        break;
    case TOKEN_PLUS:
        res = interpret(ast->left) + interpret(ast->right);
        break;
    case TOKEN_NUMBER:
        res = atoi(ast->value);
    default:
        break;
    }
    return res;
}