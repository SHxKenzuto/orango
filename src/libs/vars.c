#include "vars.h"
#include <stdlib.h>

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

