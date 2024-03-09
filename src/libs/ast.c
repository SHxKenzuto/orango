#include "ast.h"

// Funzione per creare un nodo dell'AST
Node *create_ast_node(TokenType type, char *value, NodeList *children)
{
    Node *node = malloc(sizeof(Node));
    node->type = type;
    node->value = strdup(value);
    node->children = children;
    return node;
}

Node *adoptChild(Node* parent, Node* child){
    if (parent->children == NULL)
    {
        NodeList *children = malloc(sizeof(NodeList));
        children->item = child;
        parent->children = children;
        return parent;
    }

    NodeList *tempNode =parent->children;
    while (tempNode->next)
    {
        tempNode = tempNode->next;   
    }
    NodeList* childToAdopt = malloc(sizeof(NodeList));
    childToAdopt->item = child;
    tempNode->next = childToAdopt; 
    
    
    return parent;
}

Node *create_binary_ast_node(TokenType type, char *value, Node *left, Node *right){
    Node* root = create_ast_node(type, value, NULL);
    root = adoptChild(root, left);
    root = adoptChild(root, right);
    return root;

}

Node *get_left_child(Node* root){
    return root->children->item;
}

Node *get_right_child(Node* root){
    return root->children->next->item;
}
// Funzione ricorsiva per stampare l'albero
void print_tree(Node* root, int space) {
    int i;
    const int COUNT = 5;
    if (root == NULL) 
        return;
    space += COUNT;
    NodeList* remainingChildren = root->children;
    while (remainingChildren)
    {
        print_tree(remainingChildren->item, space);
        printf("\n");
        for (i = COUNT; i < space; i++) {
            printf(" ");
        }
        remainingChildren = remainingChildren->next;
    }
    printf("%s\n", root->value);
}

