#include "libs/parser.h"
#include "libs/interpreter.h"
#include "libs/vars.h"



Var* globalVar;
Var* currentMemLoc;
Var*(*allocateVar)(char*);

int main()
{
    allocateVar = &allocateVarInit;
    globalVar = malloc(sizeof(Var));
    globalVar->type = INIT;
    globalVar->next = NULL;
    globalVar->id = "\0";

    //globalVar->value=malloc(sizeof(int));
    //*(int*)globalVar->value=2;
    Boolean goOn = TRUE;
    while (goOn)
    {
        char buffer[256];
        printf(">");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        if (strcmpDecente(buffer, "UGHA"))
        {
            printf(">Arrivederci da Catanzaro\n");
            goOn=FALSE;
        }
        

        else{
            Token *tokens = tokenize(buffer);
            TokenReturn *res = (TokenReturn *)malloc(sizeof(TokenReturn));
            res->token = tokens;
            res->esito = TRUE;
            Node *ast = main_parse(res);

            if (ast != NULL && res->esito)
            {
            print_tree(ast,0);
            InterpretReturn* res = (interpret(ast, (TokenType)NULL));
            if (res != NULL)
            {
                printf("\nresult: %d\n", *(int*)(res->value));//commento 1
            }
            
            
            
            }
            else
            {
                printf("Istruzione non valida!\n");
            }
            
        }

    }
        
    
    printf(">fuori dal while principale\n");
    // Liberare la memoria qui se necessario
    free_vars(globalVar);
    return 0;
}
