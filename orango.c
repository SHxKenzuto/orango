#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_UNKNOWN,
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_FAILURE
} TokenType;

typedef enum{
    false,
    true
} Boolean;


// Definizione della struttura Token per la lista linkata
typedef struct Token {
    TokenType type;
    char* value;
    struct Token* next; // Puntatore al next nodo nella lista
} Token;

typedef struct TokenReturn {
    Boolean esito;
    unsigned char messaggio;
    Token* token;
} TokenReturn;

typedef struct Node {
    TokenType type;
    char* value;
    struct Node* left;
    struct Node* right;
} Node;

// Funzione per creare un nuovo nodo
Token* create_token_node(char* value, TokenType type) {
    Token* nuovo_nodo = (Token*)malloc(sizeof(Token));
    nuovo_nodo->value = strdup(value);
    nuovo_nodo->type = type;
    nuovo_nodo->next = NULL;
    return nuovo_nodo;
}

// Funzione per inserire un nodo alla fine della lista
Token* append(Token* testa, char* valore, TokenType type) {
    Token* nuovo_nodo = create_token_node(valore, type);
    if (testa == NULL) {
        return nuovo_nodo;
    } else {
        Token* temp = testa;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = nuovo_nodo;
        return testa;
    }
}

// Funzione per liberare la memoria occupata dalla lista
void free_all(Token* testa) {
    Token* temp;
    while (testa != NULL) {
        temp = testa;
        testa = testa->next;
        free(temp);
    }
}

// Funzione per liberare la memoria occupata dalla lista
Token* pop(Token* testa) {
    Token* temp;
    temp = testa;
    testa = testa->next;
    return temp;
}

Boolean strcmpDecente(char*, char*);
// Funzione per tokenizzare l'input
Token* tokenize(char* input) {
    Token* tokens = NULL;
    char* tokenValue = strtok(input, " ");
    while (tokenValue != NULL) {
        TokenType type;
        if (strcmpDecente(tokenValue, "=")) type = TOKEN_ASSIGN;
        else if(strcmpDecente(tokenValue,"+")) type = TOKEN_PLUS;
        else if(strcmpDecente(tokenValue,"*")) type = TOKEN_MULT;
        else if (isdigit(tokenValue[0])) type = TOKEN_NUMBER; //gestire i char
        else type = TOKEN_IDENTIFIER;
        tokens = append(tokens,tokenValue,type);
        tokenValue = strtok(NULL, " ");
    }
    
    return tokens;
}


TokenReturn* eat(TokenReturn* res, TokenType expected_type){
    //TokenReturn* res = malloc(sizeof(TokenReturn));
    res->esito = false;
    Token* token = res->token;
    if(token != NULL && token->type == expected_type){
        res->token = token->next;
        res->esito = true,
        free(token);
        res->messaggio = 0x0000;  
        return res; 
    }
    res->messaggio = 0x0001;  
    return res;
    
}

// Funzione per creare un nodo dell'AST
Node* create_ast_node(TokenType type, char* value, Node* left, Node* right) {
    Node* node = malloc(sizeof(Node));
    node->type = type;
    node->value = strdup(value);
    node->left = left;
    node->right = right;
    return node;
}
Node* expr(TokenReturn*);
Node* factor(TokenReturn* res){
    if (res->esito && res->token!=NULL)
    {
        Node* n = NULL;
        Token* current_token = res->token;
        switch (current_token->type)
        {
        case TOKEN_NUMBER:
            char* num = strdup(current_token->value);
            res = eat(res, TOKEN_NUMBER);
            if(res->esito){
                n = create_ast_node(TOKEN_NUMBER, strdup(num), NULL, NULL);
            }     
            break;
        case TOKEN_LPAREN:
            res = eat(res, TOKEN_LPAREN);
            if(res->esito){
                n = expr(res);
            }
            res = eat(res, TOKEN_RPAREN);
            if(!(res->esito && res->token!=NULL)){
                printf(">DEBUG FACTOR: %x\n", res->messaggio);
                n = NULL;
            }
        default:
            res = eat(res, TOKEN_FAILURE);
            break;
        }
        if (!(res->esito && res->token!=NULL))
        {
            printf(">DEBUG FACTOR: %x\n", res->messaggio);
        }
        
        return n;   
    }
    printf(">DEBUG FACTOR: %x\n", res->messaggio);

    return NULL;
    
    
}

Node* term(TokenReturn* res){
    Node* n = factor(res);
    if (n!= NULL && res->esito && res->token!=NULL)
    {
        TokenType op = res->token->type;
        res = eat(res, TOKEN_MULT);
       if(!(res->esito)){
            res = eat(res, TOKEN_DIV);
        }
        if(res->esito && res->token!=NULL){
            return create_ast_node(op, "*/", term(res), n);
        }      
    }
    printf(">DEBUG TERM: %x\n", res->messaggio);
    return n;
    
    

}

Node* expr(TokenReturn* res){
    Node* n = term(res);
    if (n != NULL && res->token!=NULL)
    {
        TokenType op = res->token->type;
        res = eat(res, TOKEN_PLUS);
        if(!(res->esito)){
            res = eat(res, TOKEN_MINUS);
        }
        if(res->esito && res->token!=NULL){
            return create_ast_node(op, "+-", expr(res), n);
        }
        printf(">DEBUG EXPR: %x\n", res->messaggio);
        return n;
        
    }
    printf(">DEBUG EXPR: %x\n", res->messaggio);
    return n;
    
    

}

Boolean strcmpDecente(char* expected, char* actual){
    while (*expected != '\0' && *actual != '\0') {
        if (*expected != *actual) {
            return false; // I caratteri non corrispondono
        }
        expected++;
        actual++;
    }

    // Se entrambe le stringhe hanno raggiunto il terminatore nullo '\0', sono uguali.
    return true;
    
}

//RIFATTORZZARE
// Funzione per costruire l'AST
Node* parse(TokenReturn* res) {
    if(res->esito && res->token!=NULL){
        Token* current_token = res->token;
        Node* n = NULL;
        switch (current_token->type){
        case TOKEN_IDENTIFIER:
            char* id = strdup(current_token->value);
            res = eat(res,TOKEN_IDENTIFIER);
            if(res->esito && res->token!=NULL){
                res = eat(res, TOKEN_ASSIGN);
                if(res->esito && res->token!=NULL){
                    n = create_ast_node(TOKEN_ASSIGN, "=", create_ast_node(TOKEN_IDENTIFIER,id,NULL,NULL), expr(res));
                } else {
                    printf(">DEBUG EXPR: %x\n", res->messaggio);
                }
            }else{
                printf(">DEBUG EXPR: %x\n", res->messaggio);
            }
            break;
        case TOKEN_NUMBER:
        case TOKEN_LPAREN:
            n = expr(res);  
        default:
            break;
        }
        return n;
    }
    printf(">DEBUG EXPR: %x\n", res->messaggio);
    return NULL;
}
int interpret(Node*);
// Funzione per interpretare l'AST
int interpret(Node* ast) {
    int res = 0;
    switch(ast->type){
        case TOKEN_MULT:
            res = interpret(ast->left) * interpret(ast->right);
            break;
        case TOKEN_PLUS:
            res =  interpret(ast->left) + interpret(ast->right);
            break;
        case TOKEN_NUMBER:
            res = atoi(ast->value);
        default:
            break;
    }
    return res;
}




Token* tokens;
Token* current_token;


int main() {
    char buffer[256];
    printf(">");
    fgets(buffer, sizeof(buffer), stdin);

    Token* tokens = tokenize(buffer);
    TokenReturn* res = (TokenReturn*) malloc(sizeof(TokenReturn));
    res->token = tokens;
    res->esito = true;
    Node* ast = parse(res);
    
    if (ast != NULL && res->esito) {
        printf("result: %d\n",interpret(ast));
    } else {
        printf("Istruzione non valida!\n");
    }
    
    // Liberare la memoria qui se necessario
    
    return 0;
}
