#include "ast.h"

// Funzione per creare un nodo dell'AST
Node *create_ast_node(TokenType type, char *value, Node *left, Node *right)
{
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->value = strdup(value);
    node->left = left;
    node->right = right;
    return node;
}


