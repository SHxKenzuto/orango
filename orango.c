#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>

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
    TOKEN_END
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

typedef struct TokenReturn{
    Boolean esito;
    Byte messaggio;
    Token* token;
};

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
        while (temp->next != NULL) {
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

// Funzione per tokenizzare l'input
Token* tokenize(char* input) {
    Token* tokens = NULL;
    char* tokenValue = strtok(input, " ");
    
    while (tokenValue != NULL) {
        TokenType type;
        
        if (strcmp(tokenValue, "=") == 0) type = TOKEN_ASSIGN;
        else if(strcmp(tokenValue,"+" == 0)) type = TOKEN_PLUS;
        else if (isdigit(tokenValue[0])) type = TOKEN_NUMBER; //gestire i char
        else type = TOKEN_IDENTIFIER;
        
        append(tokens,tokenValue,type);
        
        tokenValue = strtok(NULL, " ");
    }
    
    append(tokens,NULL,TOKEN_END);
    
    return tokens;
}


TokenReturn* eat(TokenReturn* res, TokenType expected_type){
    //TokenReturn* res = malloc(sizeof(TokenReturn));
    res->token = NULL;
    res->esito = false;
    Token* token = res->token;
    if(token->type == expected_type){
        Token* next = token->next;
        res->token = next;
        res->esito = true,
        free(token);
        return res; 
    }
    res->messaggio = 0x0000;  
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

Node* factor(TokenReturn* res){
    if (res->esito)
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
            else{
                printf(">DEBUG FACTOR: %x", res->messaggio);
                n = NULL;
            }
        default:
            break;
        }
        if (!(res->esito))
        {
            printf(">DEBUG FACTOR: %x", res->messaggio);
        }
        
        return n;   
    }
    printf(">DEBUG FACTOR: %x", res->messaggio);

    return NULL;
    
    
}

Node* term(TokenReturn* res){
    if (res->esito)
    {
        Node* n = factor(current_token);
        TokenType op = res->token->type;
        if(op == TOKEN_MULT){
            res = eat(res, TOKEN_MULT);
        }else if(op  == TOKEN_DIV){
            res = eat(res, TOKEN_DIV);
        }
        if(res->esito){
            return create_ast_node(op, "*/", term(res), n);
        }
        printf(">DEBUG TERM: %x", res->messaggio);
        return n;
        
    }
    printf(">DEBUG TERM: %x", res->messaggio);
    return NULL;
    
    

}

Node* expr(TokenReturn* res){
    if (res->esito)
    {
        Node* n = term(res);
        TokenType op = res->token->type;
        if(op == TOKEN_PLUS){
            res = eat(res, TOKEN_PLUS);
        }else if(op  == TOKEN_MINUS){
            res = eat(res, TOKEN_MINUS);
        }
        if(res->esito){
            return create_ast_node(op, "+-", expr(res), n);
        }
        printf(">DEBUG EXPR: %x", res->messaggio);
        return n;
        
    }
    printf(">DEBUG EXPR: %x", res->messaggio);
    return NULL;
    
    

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
Node* parse(Token* tokens) {
    Token* current_token = tokens;
    Node* n = NULL;
    switch (current_token->type){
    case TOKEN_IDENTIFIER:
        char* id = strdup(current_token->value);
        current_token = eat(current_token,TOKEN_IDENTIFIER);
        if(current_token != NULL){
            current_token = eat(current_token, TOKEN_ASSIGN);
            if(current_token != NULL){
                n = create_ast_node(TOKEN_ASSIGN, "=", id, expr(current_token));
            }
        }
        break;
    case TOKEN_NUMBER:
    case TOKEN_LPAREN:
        n = expr(current_token);  
    default:
        break;
    }
    return n;
}

// Funzione per interpretare l'AST
void interpret(Node* ast) {

}



Token* tokens;
Token* current_token;


int main() {
    char buffer[256];
    
    printf(">");
    fgets(buffer, sizeof(buffer), stdin);
    
    Token* tokens = tokenize(buffer);
    Node* ast = parse(tokens);
    
    if (ast != NULL) {
        interpret(ast);
    } else {
        printf("Istruzione non valida!\n");
    }
    
    // Liberare la memoria qui se necessario
    
    return 0;
}
