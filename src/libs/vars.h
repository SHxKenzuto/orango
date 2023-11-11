#ifndef __VARS_
#define __VARS_

#include "utils.h"

typedef enum{
    INIT,
    INTEGER,
    STRING,
    BOOL
    
} VarType;

typedef struct Var
{
    VarType type;
    char* id;
    void* value;
    struct Var* next;
}Var;

//void allocateVar(char*);
Var* allocateVarInit(char*);
Var* allocateVarNext(char*);
Var* cerca_var(char*);
void free_vars(Var*);


#endif

