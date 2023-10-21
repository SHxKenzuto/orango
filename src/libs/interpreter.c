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
    case TOKEN_DIV:
        res = (int) (interpret(ast->left) / interpret(ast->right));
        break;
    case TOKEN_PLUS:
        res = interpret(ast->left) + interpret(ast->right);
        break;
    case TOKEN_MINUS:
        res = interpret(ast->left) - interpret(ast->right);
        break;
    case TOKEN_NUMBER:
        res = atoi(ast->value);
        break;
    default:
        break;
    }
    return res;
}

//5 + 2 * 6 * 8 / 4 test
//5 / 5 * 2