#ifndef TYPES_H
#define TYPES_H

#include "lexer.h"

/*
 * Map of all token types to their corresponding sizes for use in the assembler
 */
extern std::unordered_map<Token::Type, int> TYPES;

#endif