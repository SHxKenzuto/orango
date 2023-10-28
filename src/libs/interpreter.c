#include "interpreter.h"

// Funzione per interpretare l'AST
InterpretReturn* interpret(Node *ast, TokenType fatherType)
{
    extern Var* globalVar;
    InterpretReturn* res = malloc(sizeof(InterpretReturn));
    int* lValue;
    int* rValue;
    switch (ast->type)
    {
    case TOKEN_MULT:
        
        res->value = malloc(sizeof(int));//commento 1 
        lValue = (int*)(interpret(ast->left, ast->type)->value);//commento 2
        rValue = (int*)(interpret(ast->right, ast->type)->value);//commento 2
        *(int*)res->value = (*lValue * *rValue);//commento 3
        free(rValue);//commento 2
        free(lValue);//commento 2
        res->type = INTEGER;
        break;
    case TOKEN_DIV:
        res->value = malloc(sizeof(int));//commento 1
        lValue = (int*)(interpret(ast->left, ast->type)->value);//commento 2
        rValue = (int*)(interpret(ast->right, ast->type)->value);//commento 2
        *(int*)res->value = ((int) (*lValue / *rValue));//commento 3
        free(rValue);//commento 2
        free(lValue);//commento 2
        res->type = INTEGER;
        break;
    case TOKEN_PLUS:
        printf(">start token plus\n");
        res->value = malloc(sizeof(int));//commento 1
        lValue = (int*)(interpret(ast->left, ast->type)->value);//commento 2
        rValue = (int*)(interpret(ast->right, ast->type)->value);//commento 2
        *(int*)res->value = (*lValue + *rValue);//commento 3
        free(rValue);//commento 2
        free(lValue);//commento 2
        res->type = INTEGER;
        printf(">end token plus\n");
        break;
    case TOKEN_MINUS:
        res->value = malloc(sizeof(int));//commento 1
        lValue = (int*)(interpret(ast->left, ast->type)->value);//commento 2
        rValue = (int*)(interpret(ast->right, ast->type)->value);//commento 2
        *(int*)res->value = (*lValue - *rValue);//commento 3
        free(rValue);//commento 2
        free(lValue);//commento 2
        res->type = INTEGER;
        break;
    case TOKEN_NUMBER:
        printf(">start token number\n");
        res->value = malloc(sizeof(int));//commento 1
        int atoiRes =atoi(ast->value);
        *(int*)(res->value) = atoiRes;//commento 3
        res->type = INTEGER;
        printf(">end token number\n");
        break;
    case TOKEN_ASSIGN:
        printf(">start token assign\n");
        interpret(ast->left, ast->type);
        InterpretReturn* resTemp = interpret(ast->right,ast->type);
        globalVar->value = resTemp->value;
        globalVar->type = resTemp->type;
        printf(">globalVar in interpret: Id: %s\t Val: %d\n", globalVar->id, *(int*)globalVar->value);
        res = NULL;
        break;
    case TOKEN_IDENTIFIER:
        printf(">start token identifer\n");
        if (fatherType == TOKEN_ASSIGN)
        {
            printf(">father token assign\n");
            //allocazione var
            allocateVar(ast->value);

        }
        else
        {
            printf(">father else\n");
            //recupero var
        }
        res = NULL;
        
        
    default:
        break;
    }
    return res;
}

//5 + 2 * 6 * 8 / 4 test
//5 / 5 * 2