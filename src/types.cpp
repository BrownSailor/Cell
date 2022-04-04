#include "include/types.h"

std::unordered_map<Token::Type, int> TYPES = 
{
    { Token::TOK_INT, sizeof(int) },
    { Token::TOK_CHAR, sizeof(char) },
    { Token::TOK_BOOL, sizeof(bool) },
    { Token::TOK_STR, sizeof(char *) },
};
