#ifndef __AST_
#define __AST_
#include <stdio.h>
#include "token.h"

// Dichiarazione anticipata di NodeList
typedef struct NodeList NodeList;

// Definizione di Node
typedef struct Node
{
    TokenType type;
    char *value;
    NodeList *children;
    //struct Node *right;
} Node;

// Definizione di NodeList
struct NodeList
{
    Node *item;
    NodeList *next;
};


Node *create_ast_node(TokenType type, char *value, NodeList *children);
Node *adoptChild(Node* parent, Node* child);
Node *create_binary_ast_node(TokenType type, char *value, Node *left, Node *right);
Node *get_left_child(Node* root);
Node *get_right_child(Node* root);
void print_tree(Node* root, int space);

#endif