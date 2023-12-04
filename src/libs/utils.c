#include "utils.h"
#include <stdio.h>

Boolean strcmpDecente(const char *expected, const char *actual)
{
    while (*expected != '\0' && *actual != '\0')
    {
        if (*expected != *actual)
        {
            return FALSE; // I caratteri non corrispondono
        }
        expected++;
        actual++;
    }
    // Se entrambe le stringhe hanno raggiunto il terminatore nullo '\0', sono uguali.
    return (*expected == '\0' && *actual == '\0');
}

Boolean isNumber(const char *num)
{
    while (*num != '\0')
    {
        if (!isdigit((unsigned char)*num))
        {
            return FALSE;
        }
        num++;
    }
    return TRUE;
}


Boolean is_op(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c=='=' || c==';';
}

char* strtok_decente(char *str) {
    static char *current;
    char static buffer[100];
    char *token = buffer;
    if (str != NULL) 
    {
        current = str;
    }
    if (*current == '\0') 
    {
        return NULL;
    }
    while (*current == ' ') 
    {
        current++;
    }
    if (is_op(*current)) 
    {
        *token++ = *current++;
    } else 
    {
        while (*current != '\0' && *current != ' ' && !is_op(*current)) 
        {
            *token++ = *current++;
        }
    }
    *token = '\0';
    return buffer;
}