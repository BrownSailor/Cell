#ifndef TYPES_H
#define TYPES_H

#include "parser.h"
#include <unordered_map>

union TypeScheme
{
    uint32_t type;
};

extern std::unordered_map<std::string, uint32_t> type_names;
extern std::unordered_map<uint32_t, std::string> type_idens;

void initialize_types();
void type_check(Node *root);

#endif
