#ifndef TYPES_H
#define TYPES_H

#include "parser.hpp"

bool operator==(const TypeScheme &left, const TypeScheme &right);
bool operator!=(const TypeScheme &left, const TypeScheme &right);

extern std::unordered_map<std::string, uint32_t> type_names;
extern std::unordered_map<uint32_t, std::string> type_idens;

TypeScheme construct_type(uint32_t type);
std::unique_ptr<Node> type_check(std::unique_ptr<Node> root);

#endif
