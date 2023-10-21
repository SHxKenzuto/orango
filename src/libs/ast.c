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

// Funzione ricorsiva per stampare l'albero
void print_tree(Node* root, int space) {
    int i;
    const int COUNT = 5;
    if (root == NULL) 
        return;
    space += COUNT;
    print_tree(root->right, space);
    printf("\n");
    for (i = COUNT; i < space; i++) {
        printf(" ");
    }
    printf("%s\n", root->value);
    print_tree(root->left, space);
}

