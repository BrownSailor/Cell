#include "include/lexer.h"
#include <iostream>

/* initLexer
 *    Purpose: create and initialize a new lexer
 * Parameters: string of source code
 *    Returns: lexer
 */
Lexer *initLexer(std::string src)
{
    Lexer *lexer = new Lexer();
    lexer->src = src;
    lexer->currIdx = 0;
    lexer->currChar = src[lexer->currIdx];

    return lexer;
}

/* lexerAdvance
 *    Purpose: advance the lexer a character
 * Parameters: lexer
 *    Returns: None
 */
void lexerAdvance(Lexer *lexer)
{
    if (lexer->currIdx < lexer->src.size() && lexer->currChar != '\0')
    {
        lexer->currIdx++;
        lexer->currChar = lexer->src[lexer->currIdx];
    }
}

/* lexerPeek
 *    Purpose: get the next character of the file
 * Parameters: lexer
 *    Returns: char
 */
char lexerPeek(Lexer *lexer)
{
    return lexer->src[std::min(lexer->currIdx + 1, (unsigned int)(lexer->src.size() - 1))];
}

/* lexerSkipWhitespace
 *    Purpose: skip whitespaces in the source code
 * Parameters: lexer
 *    Returns: None
 */
void lexerSkipWhitespace(Lexer *lexer)
{
    while (std::isspace(lexer->currChar))
    {
        lexerAdvance(lexer);
    }
}

/* lexerParseId
 *    Purpose: parse an identifier token
 * Parameters: lexer
 *    Returns: token
 */
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

/* lexerParseNumber
 *    Purpose: parse a number
 * Parameters: lexer
 *    Returns: token
 */
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

/* lexerAdvanceWith
 *    Purpose: advance the lexer and return a token
 * Parameters: lexer, token
 *    Returns: token
 */
Token *lexerAdvanceWith(Lexer *lexer, Token *token)
{
    lexerAdvance(lexer);

    return token;
}

/* lexerAdvanceCurrent
 *    Purpose: advance the lexer while keeping track of the data we are reading
 * Parameters: lexer, int token type
 *    Returns: token
 */
Token *lexerAdvanceCurrent(Lexer *lexer, int type)
{
    std::string data;
    data += lexer->currChar;

    lexerAdvance(lexer);

    return initToken(data, type);
}

/* lexerNextToken
 *    Purpose: get the next token from the source code
 * Parameters: lexer
 *    Returns: token
 */
Token *lexerNextToken(Lexer *lexer)
{
    while (lexer->currChar != '\0')
    {
        lexerSkipWhitespace(lexer);

        if (isalpha(lexer->currChar))
        {
            return lexerParseId(lexer);
        }

        if (isdigit(lexer->currChar))
        {
            return lexerParseNumber(lexer);
        }

        switch (lexer->currChar)
        {
        case ':':
            if (lexerPeek(lexer) == ':')
            {
                lexerAdvance(lexer);
                return lexerAdvanceWith(lexer, initToken("::", TOKEN_DOUBLE_COLON));
            }
            return lexerAdvanceWith(lexer, initToken(":", TOKEN_COLON));

        case '{':
            return lexerAdvanceCurrent(lexer, TOKEN_LBRACE);

        case '}':
            return lexerAdvanceCurrent(lexer, TOKEN_RBRACE);

        case '(':
            return lexerAdvanceCurrent(lexer, TOKEN_LPAREN);

        case ')':
            return lexerAdvanceCurrent(lexer, TOKEN_RPAREN);

        case '[':
            return lexerAdvanceCurrent(lexer, TOKEN_LBRACKET);

        case ']':
            return lexerAdvanceCurrent(lexer, TOKEN_RBRACKET);

        case ';':
            return lexerAdvanceCurrent(lexer, TOKEN_SEMI);

        case ',':
            return lexerAdvanceCurrent(lexer, TOKEN_COMMA);

        case '=':
            return lexerAdvanceCurrent(lexer, TOKEN_EQUALS);

        case '?':
            return lexerAdvanceCurrent(lexer, TOKEN_QUESTION);

        case '@':
            return lexerAdvanceCurrent(lexer, TOKEN_AT);

        case '"':
            return lexerAdvanceCurrent(lexer, TOKEN_QUOTE);

        case '<':
            if (lexerPeek(lexer) == '=')
            {
                return lexerAdvanceCurrent(lexer, TOKEN_LTE);
            }

            return lexerAdvanceCurrent(lexer, TOKEN_LT);

        case '>':
            if (lexerPeek(lexer) == '=')
            {
                return lexerAdvanceCurrent(lexer, TOKEN_GTE);
            }

            return lexerAdvanceCurrent(lexer, TOKEN_GT);

        case '|':
            if (lexerPeek(lexer) == '|')
            {
                return lexerAdvanceCurrent(lexer, TOKEN_DOUBLE_LINE);
            }

            return lexerAdvanceCurrent(lexer, TOKEN_LINE);

        case '&':
            if (lexerPeek(lexer) == '&')
            {
                return lexerAdvanceCurrent(lexer, TOKEN_DOUBLE_AND);
            }

            return lexerAdvanceCurrent(lexer, TOKEN_AND);

        case '.':
            return lexerAdvanceCurrent(lexer, TOKEN_DOT);

        case '-':
            return lexerAdvanceCurrent(lexer, TOKEN_DASH);

        case '\0':
            break;

        default:
            std::cout << "[Lexer]: Unexpected character `" << lexer->currChar << "`\n";
            exit(1);
        }
    }

    return initToken("\0", TOKEN_EOF);
}
