#ifndef __INTERPRETER_
#define __INTERPRETER_

#include "ast.h"
#include "vars.h"
#include "vars.h"

typedef struct InterpretReturn
{
    VarType type;
    void *value;
} InterpretReturn;

InterpretReturn* interpret(Node *, TokenType);

#endif