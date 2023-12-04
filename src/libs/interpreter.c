#include "interpreter.h"

// Funzione per interpretare l'AST
InterpretReturn* interpret(Node *ast, TokenType fatherType)
{
    extern Var* currentMemLoc;
    extern Var*(*allocateVar)(char*);
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
        res->value = malloc(sizeof(int));//commento 1
        lValue = (int*)(interpret(ast->left, ast->type)->value);//commento 2
        rValue = (int*)(interpret(ast->right, ast->type)->value);//commento 2
        *(int*)res->value = (*lValue + *rValue);//commento 3
        free(rValue);//commento 2
        free(lValue);//commento 2
        res->type = INTEGER;
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
        res->value = malloc(sizeof(int));//commento 1
        int atoiRes =atoi(ast->value);
        *(int*)(res->value) = atoiRes;//commento 3
        res->type = INTEGER;
        break;
    case TOKEN_ASSIGN:
        interpret(ast->left, ast->type);
        InterpretReturn* resTemp = interpret(ast->right,ast->type);
        currentMemLoc->value = resTemp->value;
        currentMemLoc->type = resTemp->type;
        printf(">currentMemLoc in interpret: Id: %s\t Val: %d\n", currentMemLoc->id, *(int*)currentMemLoc->value);
        res = NULL;
        break;
    case TOKEN_IDENTIFIER:
        if (fatherType == TOKEN_ASSIGN)
        {
            //allocazione var
            currentMemLoc = (*allocateVar)(ast->value);
            res = NULL;

        }
        else if (fatherType == (TokenType)NULL)
        {
            Var* searchedVar = cerca_var(ast->value);
            if (searchedVar!=NULL)
            {
                res->value = searchedVar->value;
                res->type = searchedVar->type;
            }
            
        }
        
        
            
            //recupero var
        
        
        
        
    default:
        break;
    }
    return res;
}

//5 + 2 * 6 * 8 / 4 test
//5 / 5 * 2