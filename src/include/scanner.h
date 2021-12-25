#ifndef CELL_SCANNER_H
#define CELL_SCANNER_H

#include "common.h"

typedef struct Scanner
{
    const char *start;
    const char *current;
    int line;
} Scanner;

typedef enum TokenType
{
    // 1 character operations
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACK,
    TOKEN_RBRACK,
    TOKEN_COMMA,
    TOKEN_SEMI,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_QUESTION,
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_XOR,

    // 1 or 2 character operations
    TOKEN_PLUS,
    TOKEN_PLUS_PLUS,
    TOKEN_MINUS,
    TOKEN_MINUS_MINUS,
    TOKEN_NOT,
    TOKEN_NEQ,
    TOKEN_EQ,
    TOKEN_EQEQ,
    TOKEN_GT,
    TOKEN_GTE,
    TOKEN_SHIFT_RIGHT,
    TOKEN_LT,
    TOKEN_LTE,
    TOKEN_SHIFT_LEFT,
    TOKEN_AND,
    TOKEN_AND_AND,
    TOKEN_OR,
    TOKEN_OR_OR,

    // Literals
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_INTEGER,
    TOKEN_DECIMAL,

    // Keywords
    TOKEN_INT,
    TOKEN_CHAR,
    TOKEN_BOOL,
    TOKEN_FLO,
    TOKEN_STR,
    TOKEN_VOID,
    TOKEN_CLASS,
    TOKEN_IF,
    TOKEN_ELIF,
    TOKEN_ELSE,
    TOKEN_TRU,
    TOKEN_FLS,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_PRINT,
    TOKEN_RETURN,
    TOKEN_THIS,
    TOKEN_NULL,

    TOKEN_ERROR,
    TOKEN_EOF

} TokenType;

typedef struct Token
{
    TokenType type;
    const char *start;
    int length;
    int line;
} Token;

void initScanner(const char *source);
Token scanToken();

#endif