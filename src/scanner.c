#include "include/scanner.h"

Scanner scanner;

void initScanner(const char *source)
{
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

static bool isAlpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}


static bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

static bool isAtEnd()
{
    return *scanner.current == '\0';
}

static char advance()
{
    scanner.current++;
    return scanner.current[-1];
}

static bool match(char expected)
{
    if (isAtEnd())
    {
        return false;
    }

    if (*scanner.current != expected)
    {
        return false;
    }

    scanner.current++;
    return true;
}

static Token makeToken(TokenType type)
{
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;

    return token;
}

static Token errorToken(const char *message)
{
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)(strlen(message));
    token.line = scanner.line;

    return token;
}

static char peek()
{
    return *scanner.current;
}

static char peekNext()
{
    if (isAtEnd())
    {
        return '\0';
    }

    return scanner.current[1];
}

static void skipWhitespace()
{
    while (true)
    {
        char c = peek();
        switch (c)
        {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;

            case '\n':
                scanner.line++;
                advance();
                break;
            
            case '/':
                if (peekNext() == '/')
                {
                    while (peek() != '\n' && !isAtEnd())
                    {
                        advance();
                    }
                }
                
                else
                {
                    return;
                }
                break;

            default:
                return;
        }
    }
}

static Token makeString()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
        {
            scanner.line++;
        }

        advance();
    }

    if (isAtEnd())
    {
        return errorToken("Unterminated string.");
    }

    // closing quotation mark
    advance();

    return makeToken(TOKEN_STRING);
}

static Token makeNumber()
{
    bool isDecimal = false;
    while (isDigit(peek()))
    {
        advance();
    }

    if (peek() == '.' && isDigit(peekNext()))
    {
        advance();
        isDecimal = true;

        while (isDigit(peek()))
        {
            advance();
        }
    }

    return makeToken(isDecimal ? TOKEN_DECIMAL : TOKEN_INTEGER);
}

static TokenType checkKeyword(int start, int length, const char *rest, TokenType type)
{
    if (scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0)
    {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static TokenType identifierType()
{
    switch (scanner.start[0])
    {
    case 'b': return checkKeyword(1, 3, "ool", TOKEN_BOOL);
    case 'c':
    {
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
                case 'h': return checkKeyword(2, 2, "ar", TOKEN_CHAR);
                case 'l': return checkKeyword(2, 3, "ass", TOKEN_CLASS);
            }
        }
    }
    case 'e':
    {
        if (scanner.current - scanner.start > 2 && scanner.start[1] == 'l')
        {
            switch (scanner.start[2])
            {
                case 'i': return checkKeyword(3, 1, "f", TOKEN_ELIF);
                case 's': return checkKeyword(3, 1, "e", TOKEN_ELSE);
            }
        }
    }
    case 'f':
    {
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
                case 'l':
                {
                    if (scanner.current - scanner.start > 2)
                    {
                        switch (scanner.start[2])
                        {
                            case 'o': return TOKEN_FLO;
                            case 's': return TOKEN_FLS;
                        }
                    }
                }
                case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
            }
        }
    }
    case 'i':
    {
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
                case 'f': return TOKEN_IF;
                case 'n': return checkKeyword(2, 1, "t", TOKEN_INT);
            }
        }
    }
    case 'n': return checkKeyword(1, 3, "ull", TOKEN_NULL);
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 't':
    {
        if (scanner.current - scanner.start > 1)
        {
            switch (scanner.start[1])
            {
                case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                case 'r': return checkKeyword(2, 1, "u", TOKEN_TRU);
            }
        }
    }
    case 'v': return checkKeyword(1, 3, "oid", TOKEN_VOID);
    case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token makeIdentifier()
{
    while (isAlpha(peek()) || isDigit(peek()))
    {
        advance();
    }

    return makeToken(identifierType());
}

Token scanToken()
{
    skipWhitespace();

    scanner.start = scanner.current;

    if (isAtEnd())
    {
        return makeToken(TOKEN_EOF);
    }

    char c = advance();
    if (isDigit(c))
    {
        return makeNumber();
    }

    switch (c)
    {
    case '(':
        return makeToken(TOKEN_LPAREN);
    case ')':
        return makeToken(TOKEN_RPAREN);
    case '{':
        return makeToken(TOKEN_LBRACE);
    case '}':
        return makeToken(TOKEN_RBRACE);
    case '[':
        return makeToken(TOKEN_LBRACK);
    case ']':
        return makeToken(TOKEN_RBRACK);
    case ',':
        return makeToken(TOKEN_COMMA);
    case ';':
        return makeToken(TOKEN_SEMI);
    case '*':
        return makeToken(TOKEN_STAR);
    case '/':
        return makeToken(TOKEN_SLASH);
    case '%':
        return makeToken(TOKEN_PERCENT);
    case '?':
        return makeToken(TOKEN_QUESTION);
    case ':':
        return makeToken(TOKEN_COLON);
    case '.':
        return makeToken(TOKEN_DOT);
    case '^':
        return makeToken(TOKEN_XOR);

    case '+':
        return makeToken(match('+') ? TOKEN_PLUS_PLUS : TOKEN_PLUS);
    case '-':
        return makeToken(match('-') ? TOKEN_MINUS_MINUS : TOKEN_MINUS);
    case '!':
        return makeToken(match('=') ? TOKEN_NEQ : TOKEN_NOT);
    case '=':
        return makeToken(match('=') ? TOKEN_EQEQ : TOKEN_EQ);
    case '>':
        return makeToken(match('=') ? TOKEN_GTE : match('>') ? TOKEN_SHIFT_RIGHT : TOKEN_GT);
    case '<':
        return makeToken(match('=') ? TOKEN_LTE : match('<') ? TOKEN_SHIFT_LEFT : TOKEN_LT);
    case '&':
        return makeToken(match('&') ? TOKEN_AND_AND : TOKEN_AND);
    case '|':
        return makeToken(match('|') ? TOKEN_OR_OR : TOKEN_OR);
    
    case '"':
        return makeString();
    }

    return errorToken("Unexpected character.");
}