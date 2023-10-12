#include "utils.h"

Boolean strcmpDecente(char *expected, char *actual)
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
