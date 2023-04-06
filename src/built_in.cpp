#include "built_in.h"

static void built_in_prints(bool ln)
{
    Node *node = new Node;
    node->token = new_token(Token::TOK_ID, ln ? "println" : "print", 0, 0, "");
    node->type = Node::NODE_FUN_DEC;

    Node *in = new Node, *out = new Node;
    in->type = Node::NODE_FUN_IN;
    out->type = Node::NODE_FUN_OUT;

    node->children.push_back(in);
    node->children.push_back(out);

    Node *num = new Node, *nil = new Node;
    num->token = new_token(Token::KEY_NUM, "num", 0, 0, "");
    nil->token = new_token(Token::KEY_NIL, "nil", 0, 0, "");
    in->children.push_back(num);
    out->children.push_back(nil);

    ln ? functions["println"] = node : functions["print"] = node;
}

void initialize_built_ins()
{
    built_in_prints(false);
    built_in_prints(true);
}
