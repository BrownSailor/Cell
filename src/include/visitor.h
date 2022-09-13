#ifndef VISITOR_H
#define VISITOR_H

#include "parser.h"

Node *visit_expr(Node *root);
Node *visit_array(Node *root);

#endif