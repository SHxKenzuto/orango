#include "parser.h"
#include "interpreter.h"

int main()
{
    char buffer[256];
    printf(">");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    Token *tokens = tokenize(buffer);
    TokenReturn *res = (TokenReturn *)malloc(sizeof(TokenReturn));
    res->token = tokens;
    res->esito = TRUE;
    Node *ast = parse(res);

    if (ast != NULL && res->esito)
    {
        printf("result: %d\n", interpret(ast));
    }
    else
    {
        printf("Istruzione non valida!\n");
    }
    // Liberare la memoria qui se necessario
    return 0;
}
