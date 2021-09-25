#include "include/token.h"

Token *initToken(std::string data, unsigned int type)
{
    Token *token = new Token();
    token->data = data;
    token->type = type;

    return token;
}

std::string tokenTypeToString(int type)
{
    std::string typeName;
    switch (type)
    {
    case TOKEN_ID:
    {
        typeName = "TOKEN_ID";
    }

    case TOKEN_INT:
    {
        typeName = "TOKEN_INT";
    }

    case TOKEN_COLON:
    {
        typeName = "TOKEN_COLON";
    }

    case TOKEN_DOUBLE_COLON:
    {
        typeName = "TOKEN_DOUBLE_COLON";
    }

    case TOKEN_COMMA:
    {
        typeName = "TOKEN_COMMA";
    }

    case TOKEN_LBRACE:
    {
        typeName = "TOKEN_LBRACE";
    }

    case TOKEN_RBRACE:
    {
        typeName = "TOKEN_RBRACE";
    }

    case TOKEN_LBRACKET:
    {
        typeName = "TOKEN_LBRACKET";
    }

    case TOKEN_RBRACKET:
    {
        typeName = "TOKEN_RBRACKET";
    }

    case TOKEN_EQUALS:
    {
        typeName = "TOKEN_EQUALS";
    }

    case TOKEN_SEMI:
    {
        typeName = "TOKEN_SEMI";
    }

    case TOKEN_EOF:
    {
        typeName = "TOKEN_EOF";
    }

    default:
        break;
    }

    return typeName;
}

std::string tokenToString(Token *token)
{
    std::string typeName = tokenTypeToString(token->type);
    return "TOKEN: [" + token->data + "]\t[" + typeName + "]";
}