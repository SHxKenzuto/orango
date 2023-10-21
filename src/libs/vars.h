#ifndef __VARS_
#define __VARS_

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

void allocateVar(char*);


#endif

