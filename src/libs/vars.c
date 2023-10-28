#include "vars.h"
#include <stdlib.h>
#include <stdio.h>

extern Var* globalVar;
void allocateVar(char* id){

    if (!(globalVar->type==INIT))
    {

        Var* tempVar = malloc(sizeof(Var));
        tempVar->next = globalVar;
        globalVar =  tempVar;
    }
    globalVar->id = id;

    
}

void free_vars(Var* testa){
    
    Var* temp;
    while (testa != NULL)
    {
        
        temp = testa;
        printf(">Liberando %s = %d\n", temp->id, *(int*)temp->value);//commento 2
        testa = testa->next;
        free(temp->id);
        free(temp->value);
        free(temp);
    }
    printf(">Variabili liberate\n");

}

