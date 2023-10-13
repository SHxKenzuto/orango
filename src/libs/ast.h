#ifndef __AST_
#define __AST_
#include <stdio.h>
#include "token.h"
typedef struct Node
{
    TokenType type;
    char *value;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_ast_node(TokenType type, char *value, Node *left, Node *right);
void print_tree(Node* root, int space);

#endif