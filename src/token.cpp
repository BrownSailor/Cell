#include "include/token.h"
#include <iostream>

/* initToken
 *    Purpose: create and initialize a new token
 * Parameters: string data, unsigned int token type
 *    Returns: token
 */
Token *initToken(std::string data, unsigned int type)
{
    Token *token = new Token();
    token->data = data;
    token->type = type;

    return token;
}

/* tokenTypeToString
 *    Purpose: create a string representation of a token type
 * Parameters: unsigned int token type
 *    Returns: string
 */
std::string tokenTypeToString(unsigned int type)
{
    std::string typeName;
    switch (type)
    {
    case TOKEN_ID:
        return "TOKEN_ID";

    case TOKEN_INT:
        return "TOKEN_INT";

    case TOKEN_COLON:
        return "TOKEN_COLON";

    case TOKEN_DOUBLE_COLON:
        return "TOKEN_DOUBLE_COLON";

    case TOKEN_COMMA:
        return "TOKEN_COMMA";

    case TOKEN_LBRACE:
        return "TOKEN_LBRACE";

    case TOKEN_RBRACE:
        return "TOKEN_RBRACE";

    case TOKEN_LPAREN:
        return "TOKEN_LPAREN";

    case TOKEN_RPAREN:
        return "TOKEN_RPAREN";

    case TOKEN_LBRACKET:
        return "TOKEN_LBRACKET";

    case TOKEN_RBRACKET:
        return "TOKEN_RBRACKET";

    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";

    case TOKEN_SEMI:
        return "TOKEN_SEMI";

    case TOKEN_QUESTION:
        return "TOKEN_QUESTION";

    case TOKEN_QUOTE:
        return "TOKEN_QUOTE";

    case TOKEN_AT:
        return "TOKEN_AT";

    case TOKEN_LT:
        return "TOKEN_LT";

    case TOKEN_GT:
        return "TOKEN_GT";

    case TOKEN_LTE:
        return "TOKEN_LTE";

    case TOKEN_GTE:
        return "TOKEN_GTE";

    case TOKEN_LINE:
        return "TOKEN_LINE";

    case TOKEN_DOUBLE_LINE:
        return "TOKEN_DOUBLE_LINE";

    case TOKEN_AND:
        return "TOKEN_AND";

    case TOKEN_DOUBLE_AND:
        return "TOKEN_DOUBLE_AND";

    case TOKEN_DOT:
        return "TOKEN_DOT";

    case TOKEN_DASH:
        return "TOKEN_DASH";

    case TOKEN_EOF:
        return "TOKEN_EOF";

    default:
        break;
    }

    return "";
}

/* tokenToString
 *    Purpose: create a string representation of a token
 * Parameters: token
 *    Returns: string
 */
std::string tokenToString(Token *token)
{
    std::string typeName = tokenTypeToString(token->type);
    return "TOKEN (" + token->data + ") " + typeName;
}