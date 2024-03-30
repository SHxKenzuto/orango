#include "parser.h"

TokenType tokensMD[] = {TOKEN_MULT, TOKEN_DIV, TOKEN_END};
TokenType *const MULT_DIV = tokensMD;
TokenType tokensPM[] = {TOKEN_PLUS, TOKEN_MINUS, TOKEN_END};
TokenType *const PLUS_MINUS = tokensPM;
TokenType tokensLA[] = {TOKEN_LAND, TOKEN_END};
TokenType *const LAND = tokensLA;
TokenType tokensLO[] = {TOKEN_LOR, TOKEN_END};
TokenType *const LOR = tokensLO;
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
                n = create_binary_ast_node(TOKEN_NUMBER, strdup(num), NULL, NULL);
                free(num);
            }
            break;
        case TOKEN_BOOL:
            char *bool = strdup(current_token->value);
            *resP = eat(*resP, TOKEN_BOOL);
            if ((*resP)->esito)
            {
                n = create_binary_ast_node(TOKEN_BOOL, strdup(bool), NULL, NULL);
                free(bool);
            }
            break;
        case TOKEN_LPAREN:
            *resP = eat(*resP, TOKEN_LPAREN);
            if ((*resP)->esito)
                n = expr(resP, PLUS_MINUS, TOKEN_NUMBER);
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
                    n = create_binary_ast_node(TOKEN_NUMBER, strdup(num), NULL,NULL);
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
        printf(">DEBUG FACTOR INTERNO: %02hhx\n", (*resP)->messaggio);
        return n;
    }
    printf(">DEBUG FACTOR ESTERNO: %02hhx\n", (*resP)->messaggio);
    *resP = eat(*resP, TOKEN_FAILURE);
    printf(">DEBUG FACTOR: EAT CONCLUSO");
    return NULL;
}

Node *term(TokenReturn** resP, TokenType* expectedTypes)
{
	Node *n = factor(resP);
    TokenType op;
    char* tokenVal;
    Node *tmp;
	while((*resP)->esito && (*resP)->token != NULL && compareTokenType(expectedTypes, (*resP)->token->type))
	{
		op = (*resP)->token->type;
        tokenVal = strdup((*resP)->token->value);
		/*if(op == TOKEN_MULT) 
			*resP = eat(*resP, TOKEN_MULT);
        else if(op == TOKEN_DIV)   
			*resP = eat(*resP, TOKEN_DIV);*/
        while (*expectedTypes!=TOKEN_END)
        {
            if (*expectedTypes==op)
            {
                *resP=eat(*resP, op);
                break;
            }
            expectedTypes++;
        }
        tmp = factor(resP);
        
        if((*resP)->esito)
        {
            n = create_binary_ast_node(op, strdup(tokenVal), n, tmp);
            free(tokenVal);
            
        }
        printf(">DEBUG TERM: PRIMA PARENTESI VIOLA;\n");	
	}
    printf(">DEBUG TERM: PRIMA printf;\n");
    //printf(">DEBUG TERM: %02hhx\n", (*resP)->messaggio);
    printf(">DEBUG TERM: DOPO printf;\n");
	return n;
}

Node *expr(TokenReturn** resP, TokenType* expectedTypes, TokenType filterFlag)
{
    
    
    TokenType* innerExpecetedTypes;
    if (filterFlag == TOKEN_BOOL)
        innerExpecetedTypes = LAND;

    else if (filterFlag == TOKEN_NUMBER)
        innerExpecetedTypes = MULT_DIV;

    Node *n = term(resP, innerExpecetedTypes);
    
    TokenType op;
    char* tokenVal;
    Node *tmp;
    while((*resP)->esito && (*resP)->token != NULL && compareTokenType(expectedTypes, (*resP)->token->type)) 
    {
        op = (*resP)->token->type;
        tokenVal = strdup((*resP)->token->value); 
        /*if(op == TOKEN_PLUS) 
			*resP = eat(*resP, TOKEN_PLUS);
        else if(op == TOKEN_MINUS)   
			*resP = eat(*resP, TOKEN_MINUS);*/
        while (*expectedTypes!=TOKEN_END)
        {
            if (*expectedTypes==op)
            {
                *resP=eat(*resP, op);
                break;
            }
            expectedTypes++;
        }
        
        tmp = term(resP, innerExpecetedTypes);
        if((*resP)->esito)
        {
            n = create_binary_ast_node(op, strdup(tokenVal), n, tmp);
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
                n = expr(resP, PLUS_MINUS, TOKEN_NUMBER);
                break;
            case TOKEN_BOOL:
                n=expr(resP, LOR, TOKEN_BOOL);
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
        n = expr(resP, PLUS_MINUS, TOKEN_NUMBER);
    }
    else
    {
        char *id = strdup((*resP)->token->value);
        Var* searchedVar = cerca_var(id);
        if(searchedVar != NULL)
        {
            if((*resP)->esito && (*resP)->token != NULL)
            {
                n = expr(resP, PLUS_MINUS, TOKEN_NUMBER);
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
            n = create_binary_ast_node(TOKEN_IDENTIFIER, id, NULL, NULL);

            if ((*resP)->token->type==TOKEN_ASSIGN)
            {
                *resP = eat(*resP, TOKEN_ASSIGN);
                if ((*resP)->esito && (*resP)->token != NULL)
                {
                    n = create_binary_ast_node(TOKEN_ASSIGN, "=", n, var_expr(resP));
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
            
            n = create_binary_ast_node(TOKEN_IDENTIFIER, id, NULL, NULL);
            
            if ((*resP)->token->type==TOKEN_ASSIGN)
            {
                
                *resP = eat(*resP, TOKEN_ASSIGN);
                if ((*resP)->esito && (*resP)->token != NULL)
                {
                    n = create_binary_ast_node(TOKEN_ASSIGN, "=", n, var_expr(resP));
                }
            }
            else
            {
                printf(">ELSE NON ASSIGN\n");
                *resP = tempRes;
                n = var_expr(resP);
            }
            
            
            
        }
        free(tempRes->token);
        free(tempRes);

        
    }
    free(id);
    return n;
}

Boolean compareTokenType(TokenType* typeList, TokenType goalType){
    printf(">DEBUG TERM: dentro comparett\n currentItem %d: \ngoalType: %d \n", *typeList, goalType);
    while (*typeList!=TOKEN_END)
    {

        printf(">DEBUG TERM: dentro while (*typeList!=TOKEN_END);\ncurrentItem %d: \n", *typeList);
        if (*typeList == goalType)
        {
            return TRUE;
        }
        typeList++;
        printf(">DEBUG TERM: dentro while (*typeList!=TOKEN_END);\nnextItem %d: \n", *typeList);
        
    }
    return FALSE;
    
}

//Entrypoint parser
Node* main_parse(TokenReturn** resP){
    Node* parse_tree = parse(resP);
    if((*resP)->esito == TRUE){
        eat(*resP,TOKEN_TERM);
    }
    return parse_tree;
}