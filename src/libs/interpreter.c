#include "interpreter.h"

// Funzione per interpretare l'AST
int interpret(Node *ast, TokenType fatherType)
{
    extern Var* globalVar;
    int res = 0;
    switch (ast->type)
    {
    case TOKEN_MULT:
        res = interpret(ast->left, ast->type) * interpret(ast->right, ast->type);
        break;
    case TOKEN_DIV:
        res = (int) (interpret(ast->left, ast->type) / interpret(ast->right, ast->type));
        break;
    case TOKEN_PLUS:
        res = interpret(ast->left, ast->type) + interpret(ast->right, ast->type);
        break;
    case TOKEN_MINUS:
        res = interpret(ast->left, ast->type) - interpret(ast->right, ast->type);
        break;
    case TOKEN_NUMBER:
        res = atoi(ast->value);
        break;
    case TOKEN_ASSIGN:
        interpret(ast->left, ast->type);
        break;
    case TOKEN_IDENTIFIER:
        if (fatherType == TOKEN_ASSIGN)
        {
            //allocazione var
            printf(">%d\n",  *(int*)globalVar->value);
        }
        else
        {
            //recupero var
        }
        
        
    default:
        break;
    }
    return res;
}

//5 + 2 * 6 * 8 / 4 test
//5 / 5 * 2