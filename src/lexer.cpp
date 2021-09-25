#include "include/lexer.h"
#include <iostream>

Lexer *init_lexer(std::string src)
{
    Lexer *lexer = new Lexer[1]();
    lexer->src = src;
    lexer->currIdx = 0;
    lexer->currChar = src[lexer->currIdx];

    return lexer;
}

void lexer_advance(Lexer *lexer)
{
    if (lexer->currIdx < lexer->src.size() && lexer->currChar != '\0')
    {
        lexer->currIdx++;
        lexer->currChar = lexer->src[lexer->currIdx];
    }
}

char lexer_peek(Lexer *lexer)
{
    return lexer->src[std::min(lexer->currIdx + 1, (unsigned int)(lexer->src.size() - 1))];
}

void lexer_skip_whitespace(Lexer *lexer)
{
    while (std::isspace(lexer->currChar))
    {
        lexer_advance(lexer);
    }
}

Token *lexer_parse_id(Lexer *lexer)
{
    std::string data;
    while (isalpha(lexer->currChar))
    {
        data += lexer->currChar;
        lexer_advance(lexer);
    }

    return init_token(data, TOKEN_ID);
}

Token *lexer_parse_number(Lexer *lexer)
{
    std::string data;
    while (isdigit(lexer->currChar))
    {
        data += lexer->currChar;
        lexer_advance(lexer);
    }

    return init_token(data, TOKEN_INT);
}

Token *lexer_advance_with(Lexer *lexer, Token *token)
{
    lexer_advance(lexer);

    return token;
}

Token *lexer_advance_current(Lexer *lexer, int type)
{
    std::string data;
    data += lexer->currChar;

    lexer_advance(lexer);

    return init_token(data, type);
}

Token *lexer_next_token(Lexer *lexer)
{
    while (lexer->currChar != '\0')
    {
        lexer_skip_whitespace(lexer);

        if (isalpha(lexer->currChar))
        {
            return lexer_advance_with(lexer, lexer_parse_id(lexer));
        }

        if (isdigit(lexer->currChar))
        {
            return lexer_advance_with(lexer, lexer_parse_number(lexer));
        }

        switch (lexer->currChar)
        {
        case ':':
        {
            if (lexer_peek(lexer) == ':')
            {
                lexer_advance(lexer);
                return lexer_advance_with(lexer, init_token("::", TOKEN_DOUBLE_COLON));
            }
            return lexer_advance_with(lexer, init_token(":", TOKEN_COLON));
        }

        case '{':
        {
            return lexer_advance_current(lexer, TOKEN_LBRACE);
        }

        case '}':
        {
            return lexer_advance_current(lexer, TOKEN_RBRACE);
        }

        case '[':
        {
            return lexer_advance_current(lexer, TOKEN_LBRACKET);
        }

        case ']':
        {
            return lexer_advance_current(lexer, TOKEN_RBRACKET);
        }

        case ';':
        {
            return lexer_advance_current(lexer, TOKEN_SEMI);
        }

        case ',':
        {
            return lexer_advance_current(lexer, TOKEN_COMMA);
        }

        case '\0':
            break;

        default:
            std::cout << "[Lexer]: Unexpected character `" << lexer->currChar << "`\n";
            exit(1);
        }
    }

    return init_token("\0", TOKEN_EOF);
}
