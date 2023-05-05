#include "error.hpp"

void print_location(const Token &token)
{
    std::cerr << token.file << ":" << token.row << ":" << token.col << ": ";
}
