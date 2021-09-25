#include "include/lexer.h"
#include <iostream>

Lexer *initLexer(std::string src)
{
    Lexer *lexer = new Lexer();
    lexer->src = src;
    lexer->currIdx = 0;
    lexer->currChar = src[lexer->currIdx];

    return lexer;
}

void lexerAdvance(Lexer *lexer)
{
    if (lexer->currIdx < lexer->src.size() && lexer->currChar != '\0')
    {
        lexer->currIdx++;
        lexer->currChar = lexer->src[lexer->currIdx];
    }
}

char lexerPeek(Lexer *lexer)
{
    return lexer->src[std::min(lexer->currIdx + 1, (unsigned int)(lexer->src.size() - 1))];
}

void lexerSkipWhitespace(Lexer *lexer)
{
    while (std::isspace(lexer->currChar))
    {
        lexerAdvance(lexer);
    }
}

Token *lexerParseId(Lexer *lexer)
{
    std::string data;
    while (isalpha(lexer->currChar))
    {
        data += lexer->currChar;
        lexerAdvance(lexer);
    }

    return initToken(data, TOKEN_ID);
}

Token *lexerParseNumber(Lexer *lexer)
{
    std::string data;
    while (isdigit(lexer->currChar))
    {
        data += lexer->currChar;
        lexerAdvance(lexer);
    }

    return initToken(data, TOKEN_INT);
}

Token *lexerAdvanceWith(Lexer *lexer, Token *token)
{
    lexerAdvance(lexer);

    return token;
}

Token *lexerAdvanceCurrent(Lexer *lexer, int type)
{
    std::string data;
    data += lexer->currChar;

    lexerAdvance(lexer);

    return initToken(data, type);
}

Token *lexerNextToken(Lexer *lexer)
{
    while (lexer->currChar != '\0')
    {
        lexerSkipWhitespace(lexer);

        if (isalpha(lexer->currChar))
        {
            return lexerAdvanceWith(lexer, lexerParseId(lexer));
        }

        if (isdigit(lexer->currChar))
        {
            return lexerAdvanceWith(lexer, lexerParseNumber(lexer));
        }

        switch (lexer->currChar)
        {
        case ':':
        {
            if (lexerPeek(lexer) == ':')
            {
                lexerAdvance(lexer);
                return lexerAdvanceWith(lexer, initToken("::", TOKEN_DOUBLE_COLON));
            }
            return lexerAdvanceWith(lexer, initToken(":", TOKEN_COLON));
        }

        case '{':
        {
            return lexerAdvanceCurrent(lexer, TOKEN_LBRACE);
        }

        case '}':
        {
            return lexerAdvanceCurrent(lexer, TOKEN_RBRACE);
        }

        case '[':
        {
            return lexerAdvanceCurrent(lexer, TOKEN_LBRACKET);
        }

        case ']':
        {
            return lexerAdvanceCurrent(lexer, TOKEN_RBRACKET);
        }

        case ';':
        {
            return lexerAdvanceCurrent(lexer, TOKEN_SEMI);
        }

        case ',':
        {
            return lexerAdvanceCurrent(lexer, TOKEN_COMMA);
        }

        case '\0':
            break;

        default:
            std::cout << "[Lexer]: Unexpected character `" << lexer->currChar << "`\n";
            exit(1);
        }
    }

    return initToken("\0", TOKEN_EOF);
}
