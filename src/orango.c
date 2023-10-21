#include "libs/parser.h"
#include "libs/interpreter.h"



Var* globalVar;

int main()
{
    globalVar = malloc(sizeof(Var));
    globalVar->type = INIT;
    globalVar->next = NULL;
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
            Node *ast = parse(res);

            if (ast != NULL && res->esito)
            {
            print_tree(ast,0);
            printf("\nresult: %d\n", interpret(ast, (TokenType)NULL));
            }
            else
            {
            printf("Istruzione non valida!\n");
            }
        }

        }
        
    
    
    // Liberare la memoria qui se necessario
    return 0;
}
