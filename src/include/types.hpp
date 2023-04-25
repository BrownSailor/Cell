#ifndef TYPES_H
#define TYPES_H

#include "parser.hpp"
#include <unordered_map>

bool operator==(const TypeScheme &left, const TypeScheme &right);
bool operator!=(const TypeScheme &left, const TypeScheme &right);

extern std::unordered_map<std::string, uint32_t> type_names;
extern std::unordered_map<uint32_t, std::string> type_idens;

void initialize_types();
void type_check(Node *root);

#endif
