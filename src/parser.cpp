#include "include/parser.h"
#include "include/utils.h"
#include <iostream>

/* initParser
 *    Purpose: create and initialize a new parser
 * Parameters: lexer
 *    Returns: parser
 */
Parser *initParser(Lexer *lexer)
{
    Parser *parser = new Parser();
    parser->lexer = lexer;
    parser->token = lexerNextToken(lexer);

    return parser;
}

/* parserEat
 *    Purpose: advance the parser while expecting a certain token
 * Parameters: parser, token type
 *    Returns: token
 */
Token *parserEat(Parser *parser, unsigned int type)
{
    if (parser->token->type != type)
    {
        std::cout << "[Parser]: Unexpected token `" << tokenToString(parser->token)
                  << "`, expected: `" << tokenTypeToString(type) << "`\n";
        exit(1);
    }

    parser->token = lexerNextToken(parser->lexer);

    return parser->token;
}

/* parserParse
 *    Purpose: start parsing statements
 * Parameters: parser
 *    Returns: tree node
 */
Tree *parserParse(Parser *parser)
{

    return parserParseCompound(parser);
}

/* parserParseId
 *    Purpose: parse an identifier
 * Parameters: parser
 *    Returns: tree node
 */
Tree *parserParseId(Parser *parser)
{
    std::string data = parser->token->data;
    parserEat(parser, TOKEN_ID);

    Tree *tree = initTree(TREE_VARIABLE);
    tree->name = data;

    if (parser->token->type == TOKEN_EQUALS)
    {
        parserEat(parser, TOKEN_EQUALS);
        tree->type = TREE_ASSIGN;
        tree->data = parserParseExpr(parser);
    }

    else if (parser->token->type == TOKEN_COLON)
    {
        parserEat(parser, TOKEN_COLON);

        if (parser->token->type == TOKEN_LBRACKET)
        {
            parserEat(parser, TOKEN_LBRACKET);
            parserEat(parser, TOKEN_RBRACKET);
            tree->dataType = typeToInt("[]" + parser->token->data);
            parserEat(parser, TOKEN_ID);
        }

        else
        {
            tree->dataType = typeToInt(parser->token->data);
            parserEat(parser, TOKEN_ID);

            if (parser->token->type == TOKEN_EQUALS)
            {
                parserEat(parser, TOKEN_EQUALS);
                tree->type = TREE_ASSIGN;
                tree->data = parserParseExpr(parser);
            }

            else if (parser->token->type == TOKEN_LBRACE)
            {
                tree->data = parserParseCompound(parser);
            }
        }
    }

    else if (parser->token->type == TOKEN_LPAREN)
    {
        tree->data = parserParseList(parser);

        if (tree->data->type == TREE_CALL)
        {
            tree->type = TREE_CALL;
        }

        else if (tree->data->type == TREE_FUNC_DEF)
        {
            tree->type = TREE_FUNC_DEF;
        }
    }

    else if (parser->token->type == TOKEN_INT)
    {
        tree->data = parserParseInt(parser);
    }

    return tree;
}

/* parserParseInt
 *    Purpose: parse an integer
 * Parameters: parser
 *    Returns: tree node
 */
Tree *parserParseInt(Parser *parser)
{
    int value = std::stoi(parser->token->data);
    parserEat(parser, TOKEN_INT);

    Tree *tree = initTree(TREE_INT);
    tree->intValue = value;

    return tree;
}

/* parserParseExpr
 *    Purpose: parse an expression
 * Parameters: parser
 *    Returns: tree node
 */
Tree *parserParseExpr(Parser *parser)
{
    switch (parser->token->type)
    {
    case TOKEN_ID:
        return parserParseId(parser);

    case TOKEN_INT:
        return parserParseInt(parser);

    case TOKEN_LPAREN:
        return parserParseList(parser);

    case TOKEN_RPAREN:
        break;

    default:
        std::cout << "[Parser]: Unexpected token `" << tokenToString(parser->token) << "`\n";
        exit(1);
    }

    return initTree(TREE_NO_OP);
}

/* parserParseList
 *    Purpose: parse a list of parameters of some sort
 * Parameters: parser
 *    Returns: tree node
 */
Tree *parserParseList(Parser *parser)
{
    parserEat(parser, TOKEN_LPAREN);
    Tree *list = initTree(TREE_COMPOUND);
    list->children.push_back(parserParseExpr(parser));

    while (parser->token->type == TOKEN_COMMA)
    {
        parserEat(parser, TOKEN_COMMA);
        list->children.push_back(parserParseExpr(parser));
    }

    parserEat(parser, TOKEN_RPAREN);

    if (parser->token->type == TOKEN_SEMI)
    {
        parserEat(parser, TOKEN_SEMI);
        list->type = TREE_CALL;
    }

    else if (parser->token->type == TOKEN_COLON)
    {
        parserEat(parser, TOKEN_COLON);
        list->type = TREE_FUNC_DEF;

        if (parser->token->type == TOKEN_LBRACKET)
        {
            parserEat(parser, TOKEN_LBRACKET);
            parserEat(parser, TOKEN_RBRACKET);
            list->dataType = typeToInt("[]" + parser->token->data);
            parserEat(parser, TOKEN_ID);
        }

        else
        {
            list->dataType = typeToInt(parser->token->data);
            parserEat(parser, TOKEN_ID);
        }
    }

    if (parser->token->type == TOKEN_LBRACE)
    {
        list->data = parserParseCompound(parser);
    }

    return list;
}

/* parserParseCompound
 *    Purpose: parse a block or segment of code
 * Parameters: parser
 *    Returns: tree node
 */
Tree *parserParseCompound(Parser *parser)
{
    bool shouldClose = false;
    if (parser->token->type == TOKEN_LBRACE)
    {
        parserEat(parser, TOKEN_LBRACE);
        shouldClose = true;
    }

    Tree *compound = initTree(TREE_COMPOUND);

    while (parser->token->type != TOKEN_EOF && parser->token->type != TOKEN_RBRACE)
    {
        compound->children.push_back(parserParseExpr(parser));
        if (parser->token->type == TOKEN_SEMI)
        {
            parserEat(parser, TOKEN_SEMI);
        }
    }

    if (shouldClose)
    {
        parserEat(parser, TOKEN_RBRACE);
    }

    return compound;
}