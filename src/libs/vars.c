#include "vars.h"
#include <stdlib.h>
#include <stdio.h>

extern Var* globalVar;
extern Var*(*allocateVar)(char*);

Var* allocateVarInit(char* id){
    globalVar->id = id;
    allocateVar = &allocateVarNext;
    return globalVar;

}

Var* allocateVarNext(char* id){
    Var* tempVar = cerca_var(id);
    if (tempVar==NULL)
    {
        tempVar = malloc(sizeof(Var));
        tempVar->next = globalVar;
        globalVar =  tempVar;
        globalVar->id = id;  
    }
    return tempVar;   
}

Var* cerca_var(char* id){
    Var* tempVar = globalVar;
    while (tempVar!=NULL && !strcmpDecente(id, tempVar->id))
    {
        tempVar = tempVar->next;
    }
    return tempVar;
    
}



void free_vars(Var* testa){
    
    Var* temp;
    while (testa != NULL)
    {
        
        temp = testa;
        
        testa = testa->next;

        if (temp->type != INIT)
        {
            free(temp->value);
            free(temp->id); 
            printf(">Liberando %s = %d\n", temp->id, *(int*)temp->value);//commento 2
        }
            
        free(temp);
        
    }
    printf(">Variabili liberate\n");

}

