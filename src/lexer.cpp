#include "include/lexer.h"

/*
 * lex_word
 *    Purpose: returns the token associated with the current word
 * Parameters: word - the current word to be lexed, line - the current line number
 *    Returns: the token associated with the current word
 */
Token lex_word(const std::string &word, int line)
{ 
    if (word == "{")
    { 
        return { .type = Token::TOK_LBRACE, .data = word, .line = line };
    }
    else if (word == "}")
    { 
        return { .type = Token::TOK_RBRACE, .data = word, .line = line };
    }
    else if (word == "(")
    { 
        return { .type = Token::TOK_LPAREN, .data = word, .line = line };
    }
    else if (word == ")")
    { 
        return { .type = Token::TOK_RPAREN, .data = word, .line = line };
    }
    else if (word == "return")
    { 
        return { .type = Token::TOK_RETURN, .data = word, .line = line };
    }
    else if (word == "int")
    { 
        return { .type = Token::TOK_INT, .data = word, .line = line };
    }
    else if (word == "eol")
    { 
        return { .type = Token::TOK_EOL, .data = word, .line = line };
    }
    else if (word == "eof")
    { 
        return { .type = Token::TOK_EOF, .data = word, .line = line };
    }
    else
    { 
        try
        { 
            std::stoi(word);
            return { .type = Token::TOK_NUM, .data = word, .line = line };
        }
        catch (const std::exception &e)
        { 
            return { .type = Token::TOK_ID, .data = word, .line = line };
        }
    }
}

/*
 * lex_line
 *    Purpose: lexes the current line and adds the tokens to the list of tokens
 * Parameters: line - the current line to be lexed, tokens - the list of tokens, line_no - the current line number
 *    Returns: none
 */
void lex_line(const std::string &line, std::vector<Token> &tokens, int line_no)
{
    std::string curr = "";
    for (char c : line)
    {
        if (c == ' ' || c == '\t')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, line_no));
                curr = "";
            }

            continue;
        }
        else if (c == ':')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, line_no));
                curr = "";
            }

            tokens.push_back({ .type = Token::TOK_COL, .data = ":", .line = line_no });
        }
        else if (c == '{')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, line_no));
                curr = "";
            }

            tokens.push_back({ .type = Token::TOK_LBRACE, .data = "{", .line = line_no });
        }
        else if (c == '}')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, line_no));
                curr = "";
            }

            tokens.push_back({ .type = Token::TOK_RBRACE, .data = "}", .line = line_no });
        }
        else if (c == '(')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, line_no));
                curr = "";
            }

            tokens.push_back({ .type = Token::TOK_LPAREN, .data = "(", .line = line_no });
        }
        else if (c == ')')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, line_no));
                curr = "";
            }

            tokens.push_back({ .type = Token::TOK_RPAREN, .data = ")", .line = line_no });
        }
        else
        {
            curr += c;
        }
    }

    if (curr != "")
    {
        tokens.push_back(lex_word(curr, line_no));
        curr = "";
    }
}

/*
 * lex
 *    Purpose: lexes the input and returns the list of tokens
 * Parameters: input - the name of the input file
 *   Returns: the list of tokens
 */
std::vector<Token> lex(const std::string &input)
{ 
    std::ifstream in;
    in.open(input);

    std::string line;
    int line_no = 1;
    std::vector<Token> tokens;

    while (getline(in, line))
    {
        if (line.size() && line.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
        {
            lex_line(line, tokens, line_no);
            tokens.push_back({ Token::TOK_EOL, "", line_no });
        }

        line_no++;  
    }

    tokens.push_back({ Token::TOK_EOF, "", line_no });
    return tokens;
}

/*
 * print_token
 *    Purpose: prints the token to the output stream
 * Parameters: token - the token to be printed, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to be std::cout
 */
void print_token(const Token &token, std::ostream &out)
{
    switch (token.type)
    {
        case Token::TOK_LBRACE:
            out << "TOKEN_LBRACE: ";
            break;
        
        case Token::TOK_RBRACE:
            out << "TOKEN_RBRACE: ";
            break;

        case Token::TOK_LPAREN:
            out << "TOKEN_LPAREN: ";
            break;

        case Token::TOK_RPAREN:
            out << "TOKEN_RPAREN: ";
            break;

        case Token::TOK_INT:
            out << "TOKEN_INT: ";
            break;

        case Token::TOK_RETURN:
            out << "TOKEN_RETURN: ";
            break;
        
        case Token::TOK_ID: 
            out << "TOKEN_ID: ";
            break;

        case Token::TOK_NUM:
            out << "TOKEN_NUM: ";
            break;

        case Token::TOK_COL:
            out << "TOKEN_COLON: ";
            break;
        
        case Token::TOK_COM:
            out << "TOKEN_COMMA: ";
            break;

        case Token::TOK_EOL:
            out << "TOKEN_EOL";
            break;

        case Token::TOK_EOF:
            out << "TOKEN_EOF";
            break;
        
        default:
            out << "Unknown token: ";
    }

    out << "\t" << token.data << "\tline " << token.line << "\n";
}

/*
 * print_lex
 *    Purpose: prints each token in the list of tokens
 * Parameters: tokens - the list of tokens
 *    Returns: none
 */
void print_lex(const std::vector<Token> &tokens)
{
    for (const auto &token : tokens)
    {
        print_token(token, std::cout);
    }
}