#include "parser.h"
Node *factor(TokenReturn** resP)
{
    if ((*resP)->esito && (*resP)->token != NULL)
    {
        Node *n = NULL;
        Token *current_token = (*resP)->token;
        switch (current_token->type)
        {
        case TOKEN_NUMBER:
            char *num = strdup(current_token->value);
            *resP = eat(*resP, TOKEN_NUMBER);
            if ((*resP)->esito)
            {
                n = create_ast_node(TOKEN_NUMBER, strdup(num), NULL, NULL);
                free(num);
            }
            break;
        case TOKEN_LPAREN:
            *resP = eat(*resP, TOKEN_LPAREN);
            if ((*resP)->esito)
                n = expr(resP);
            *resP = eat(*resP, TOKEN_RPAREN);
            break;
        case TOKEN_IDENTIFIER:
            Var* searchedVar = cerca_var(current_token->value);
            if (searchedVar!=NULL){
                *resP = eat(*resP, TOKEN_IDENTIFIER);
                if ((*resP)->esito)
                {
                    char str[12]; //Da gestire dinamicamente rispetto alla piattaforma     
                    int numero = *(int*) searchedVar->value;
                    sprintf(str, "%d", numero);
                    char *num = strdup(str);                
                    n = create_ast_node(TOKEN_NUMBER, strdup(num), NULL,NULL);
                    free(num);
                }
            }else{
                *resP = eat(*resP, TOKEN_FAILURE);
            }
            break;
        default:
            *resP = eat(*resP, TOKEN_FAILURE);
            break;
        }
        printf(">DEBUG FACTOR: %02hhx\n", (*resP)->messaggio);
        return n;
    }
    printf(">DEBUG FACTOR: %02hhx\n", (*resP)->messaggio);
    *resP = eat(*resP, TOKEN_FAILURE);
    return NULL;
}

Node *term(TokenReturn** resP)
{
	Node *n = factor(resP);
    TokenType op;
    char* tokenVal;
    Node *tmp;
	while((*resP)->esito && (*resP)->token != NULL && ((*resP)->token->type == TOKEN_MULT || (*resP)->token->type == TOKEN_DIV))
	{
		op = (*resP)->token->type;
        tokenVal = strdup((*resP)->token->value);
		if(op == TOKEN_MULT) 
			*resP = eat(*resP, TOKEN_MULT);
        else if(op == TOKEN_DIV)   
			*resP = eat(*resP, TOKEN_DIV);
        tmp = factor(resP);
        if((*resP)->esito)
        {
            n = create_ast_node(op, strdup(tokenVal), n, tmp);
            free(tokenVal);
        }	
	}
    printf(">DEBUG TERM: %02hhx\n", (*resP)->messaggio);
	return n;
}

Node *expr(TokenReturn** resP)
{
    Node *n = term(resP);
    TokenType op;
    char* tokenVal;
    Node *tmp;
    while((*resP)->esito && (*resP)->token != NULL && ((*resP)->token->type == TOKEN_PLUS || (*resP)->token->type == TOKEN_MINUS)) 
    {
        op = (*resP)->token->type;
        tokenVal = strdup((*resP)->token->value); 
        if(op == TOKEN_PLUS) 
			*resP = eat(*resP, TOKEN_PLUS);
        else if(op == TOKEN_MINUS)   
			*resP = eat(*resP, TOKEN_MINUS);
        tmp = term(resP);
        if((*resP)->esito)
        {
            n = create_ast_node(op, strdup(tokenVal), n, tmp);
            free(tokenVal);
        }
    }
    printf(">DEBUG EXPR: %02hhx\n", (*resP)->messaggio);
    return n;
}


// Funzione per costruire l'AST
Node *parse(TokenReturn** resP)
{
    if ((*resP)->esito && (*resP)->token != NULL)
    {
        Token *current_token = (*resP)->token;
        Node *n = NULL;
        switch (current_token->type)
        {
            case TOKEN_IDENTIFIER:
                n = id_parse(resP);
                break;
            case TOKEN_NUMBER:
            case TOKEN_LPAREN:
                n = expr(resP);
                break;
            default:
                break;
        } 
        printf(">DEBUG PARSE: %02hhx\n", (*resP)->messaggio);
        return n;
    }
    printf(">DEBUG EXPR: %02hhx\n", (*resP)->messaggio);
    return NULL;
}

//Parser per espressioni che coinvolgono TOKEN_IDENTIFIER
Node* var_expr(TokenReturn** resP){
    Node* n = NULL;
    if((*resP)->token->type == TOKEN_NUMBER)
    {
        n = expr(resP);
    }
    else
    {
        char *id = strdup((*resP)->token->value);
        Var* searchedVar = cerca_var(id);
        if(searchedVar != NULL)
        {
            if((*resP)->esito && (*resP)->token != NULL)
            {
                n = expr(resP);
            }
        } 
    }
    return n;
}

//Parser per istruzioni che iniziano con TOKEN_IDENTIFIER
Node* id_parse(TokenReturn** resP){

    Node* n = NULL;
    char *id = strdup((*resP)->token->value);
    Var* searchedVar = cerca_var(id);
    if(searchedVar == NULL)
    {
        *resP = eat(*resP, TOKEN_IDENTIFIER);
        if ((*resP)->esito && (*resP)->token != NULL)
        {
            n = create_ast_node(TOKEN_IDENTIFIER, id, NULL, NULL);

            if ((*resP)->token->type==TOKEN_ASSIGN)
            {
                *resP = eat(*resP, TOKEN_ASSIGN);
                if ((*resP)->esito && (*resP)->token != NULL)
                {
                    n = create_ast_node(TOKEN_ASSIGN, "=", n, var_expr(resP));
                }
            }else{
               *resP = eat(*resP, TOKEN_FAILURE);
            }
        }
    }else
    {
        TokenReturn* tempRes = malloc(sizeof(TokenReturn));
        memcpy(tempRes,*resP, sizeof(TokenReturn));
        tempRes->token = malloc(sizeof(Token));
        memcpy(tempRes->token, (*resP)->token, sizeof(Token));
        *resP = eat(*resP, TOKEN_IDENTIFIER);
        if ((*resP)->esito && (*resP)->token != NULL)
        {
            
            n = create_ast_node(TOKEN_IDENTIFIER, id, NULL, NULL);
            
            if ((*resP)->token->type==TOKEN_ASSIGN)
            {
                free(tempRes->token);
                free(tempRes);
                *resP = eat(*resP, TOKEN_ASSIGN);
                if ((*resP)->esito && (*resP)->token != NULL)
                {
                    n = create_ast_node(TOKEN_ASSIGN, "=", n, var_expr(resP));
                }
            }
            else
            {
                printf(">ELSE NON ASSIGN\n");
                *resP = tempRes;
                n = var_expr(resP);
            }
            
            
            
        }

        
    }
    free(id);
    return n;
}

//Entrypoint parser
Node* main_parse(TokenReturn** resP){
    Node* parse_tree = parse(resP);
    if((*resP)->esito == TRUE){
        eat(*resP,TOKEN_TERM);
    }
    return parse_tree;
}