#include "parser.h"
Node *factor(TokenReturn *res)
{
    if (res->esito && res->token != NULL)
    {
        Node *n = NULL;
        Token *current_token = res->token;
        switch (current_token->type)
        {
        case TOKEN_NUMBER:
            char *num = strdup(current_token->value);
            res = eat(res, TOKEN_NUMBER);
            if (res->esito)
            {
                n = create_ast_node(TOKEN_NUMBER, strdup(num), NULL, NULL);
                free(num);
            }
            break;
        case TOKEN_LPAREN:
            res = eat(res, TOKEN_LPAREN);
            if (res->esito)
                n = expr(res);
            res = eat(res, TOKEN_RPAREN);
            break;
        case TOKEN_IDENTIFIER:
            Var* searchedVar = cerca_var(current_token->value);
            if (searchedVar!=NULL){
                res = eat(res, TOKEN_IDENTIFIER);
                if (res->esito)
                {
                    char str[12]; //Da gestire dinamicamente rispetto alla piattaforma     
                    int numero = *(int*) searchedVar->value;
                    sprintf(str, "%d", numero);
                    char *num = strdup(str);                
                    n = create_ast_node(TOKEN_NUMBER, strdup(num), NULL,NULL);
                    free(num);
                }
            }
            break;
        default:
            res = eat(res, TOKEN_FAILURE);
            break;
        }
        printf(">DEBUG FACTOR: %02hhx\n", res->messaggio);
        return n;
    }
    printf(">DEBUG FACTOR: %02hhx\n", res->messaggio);
    res = eat(res, TOKEN_FAILURE);
    return NULL;
}

Node *term(TokenReturn *res)
{
	Node *n = factor(res);
    TokenType op;
    char* tokenVal;
    Node *tmp;
	while(res->esito && res->token != NULL && (res->token->type == TOKEN_MULT || res->token->type == TOKEN_DIV))
	{
		op = res->token->type;
        tokenVal = strdup(res->token->value);
		if(op == TOKEN_MULT) 
			res = eat(res, TOKEN_MULT);
        else if(op == TOKEN_DIV)   
			res = eat(res, TOKEN_DIV);
        tmp = factor(res);
        if(res->esito)
        {
            n = create_ast_node(op, strdup(tokenVal), n, tmp);
            free(tokenVal);
        }	
	}
    printf(">DEBUG TERM: %02hhx\n", res->messaggio);
	return n;
}

Node *expr(TokenReturn *res)
{
    Node *n = term(res);
    TokenType op;
    char* tokenVal;
    Node *tmp;
    while(res->esito && res->token != NULL && (res->token->type == TOKEN_PLUS || res->token->type == TOKEN_MINUS)) 
    {
        op = res->token->type;
        tokenVal = strdup(res->token->value); 
        if(op == TOKEN_PLUS) 
			res = eat(res, TOKEN_PLUS);
        else if(op == TOKEN_MINUS)   
			res = eat(res, TOKEN_MINUS);
        tmp = term(res);
        if(res->esito)
        {
            n = create_ast_node(op, strdup(tokenVal), n, tmp);
            free(tokenVal);
        }
    }
    printf(">DEBUG EXPR: %02hhx\n", res->messaggio);
    return n;
}


// Funzione per costruire l'AST
Node *parse(TokenReturn *res)
{
    if (res->esito && res->token != NULL)
    {
        Token *current_token = res->token;
        Node *n = NULL;
        switch (current_token->type)
        {
            case TOKEN_IDENTIFIER:
                n = id_parse(res);
                break;
            case TOKEN_NUMBER:
            case TOKEN_LPAREN:
                n = expr(res);
                break;
            default:
                break;
        } 
        printf(">DEBUG PARSE: %02hhx\n", res->messaggio);
        return n;
    }
    printf(">DEBUG EXPR: %02hhx\n", res->messaggio);
    return NULL;
}

//Parser per espressioni che coinvolgono TOKEN_IDENTIFIER
Node* var_expr(TokenReturn* res){
    Node* n = NULL;
    if(res->token->type == TOKEN_NUMBER)
    {
        n = expr(res);
    }
    else
    {
        char *id = strdup(res->token->value);
        Var* searchedVar = cerca_var(id);
        if(searchedVar != NULL)
        {
            if(res->esito && res->token != NULL)
            {
                n = expr(res);
            }
        } 
    }
    return n;
}

//Parser per istruzioni che iniziano con TOKEN_IDENTIFIER
Node* id_parse(TokenReturn* res){
    Node* n = NULL;
    char *id = strdup(res->token->value);
    Var* searchedVar = cerca_var(id);
    if(searchedVar == NULL)
    {
        res = eat(res, TOKEN_IDENTIFIER);
        if (res->esito && res->token != NULL)
        {
            n = create_ast_node(TOKEN_IDENTIFIER, id, NULL, NULL);
            free(id);
            if (res->token->type==TOKEN_ASSIGN)
            {
                res = eat(res, TOKEN_ASSIGN);
                if (res->esito && res->token != NULL)
                {
                    n = create_ast_node(TOKEN_ASSIGN, "=", n, var_expr(res));
                }
            }
        }
    }else
    {
        n = var_expr(res);
    }
    return n;
}

//Entrypoint parser
Node* main_parse(TokenReturn* res){
    Node* parse_tree = parse(res);
    eat(res,TOKEN_TERM);
    return parse_tree;
}