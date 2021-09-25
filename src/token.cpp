#include "include/token.h"

Token *init_token(std::string data, unsigned int type)
{
    Token *token = new Token[1]();
    token->data = data;
    token->type = type;

    return token;
}

std::string token_to_string(int type)
{
    switch (type)
    {
    case TOKEN_ID:
    {
        return "TOKEN_ID";
    }

    case TOKEN_INT:
    {
        return "TOKEN_INT";
    }

    case TOKEN_COLON:
    {
        return "TOKEN_COLON";
    }

    case TOKEN_DOUBLE_COLON:
    {
        return "TOKEN_DOUBLE_COLON";
    }

    case TOKEN_COMMA:
    {
        return "TOKEN_COMMA";
    }

    case TOKEN_LBRACE:
    {
        return "TOKEN_LBRACE";
    }

    case TOKEN_RBRACE:
    {
        return "TOKEN_RBRACE";
    }

    case TOKEN_LBRACKET:
    {
        return "TOKEN_LBRACKET";
    }

    case TOKEN_RBRACKET:
    {
        return "TOKEN_RBRACKET";
    }

    case TOKEN_EQUALS:
    {
        return "TOKEN_EQUALS";
    }

    case TOKEN_SEMI:
    {
        return "TOKEN_SEMI";
    }

    case TOKEN_EOF:
    {
        return "TOKEN_EOF";
    }

    default:
        break;
    }

    return "Unknown token type `" + std::to_string(type) + "`\n";
}