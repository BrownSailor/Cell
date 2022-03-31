#include "include/lexer.h"

/*
 * lex_word
 *    Purpose: returns the token associated with the current word
 * Parameters: word - the current word to be lexed, row - the current row number, col - the current column number
 *    Returns: the token associated with the current word
 */
Token lex_word(const std::string &word, int row, int col)
{ 
    if (word == "{")
    { 
        return { .type = Token::TOK_LBRACE, .data = word, .row = row, .col = col };
    }
    else if (word == "}")
    { 
        return { .type = Token::TOK_RBRACE, .data = word, .row = row, .col = col };
    }
    else if (word == "(")
    { 
        return { .type = Token::TOK_LPAREN, .data = word, .row = row, .col = col };
    }
    else if (word == ")")
    { 
        return { .type = Token::TOK_RPAREN, .data = word, .row = row, .col = col };
    }
    else if (word == "return")
    { 
        return { .type = Token::TOK_RETURN, .data = word, .row = row, .col = col };
    }
    else if (word == "int")
    { 
        return { .type = Token::TOK_INT, .data = word, .row = row, .col = col };
    }
    else if (word == "eol")
    { 
        return { .type = Token::TOK_EOL, .data = word, .row = row, .col = col };
    }
    else if (word == "eof")
    { 
        return { .type = Token::TOK_EOF, .data = word, .row = row, .col = col };
    }
    else
    { 
        try
        { 
            std::stoi(word);
            return { .type = Token::TOK_NUM, .data = word, .row = row, .col = col };
        }
        catch (const std::exception &e)
        { 
            return { .type = Token::TOK_ID, .data = word, .row = row, .col = col };
        }
    }
}

/*
 * lex_line
 *    Purpose: lexes the current line and adds the tokens to the list of tokens
 * Parameters: line - the current line to be lexed, tokens - the list of tokens, row - the current row number
 *    Returns: none
 */
void lex_line(const std::string &line, std::vector<Token> &tokens, int row)
{
    std::string curr = "";
    int col = 1;
    int col_end = 1;

    for (char c : line)
    {
        if (c == ' ')
        {
            col_end++;

            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            continue;
        }
        else if (c == ':')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_COL, .data = ":", .row = row, .col = col });

            col_end++;
        }
        else if (c == '{')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_LBRACE, .data = "{", .row = row, .col = col });
            
            col_end++;
        }
        else if (c == '}')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_RBRACE, .data = "}", .row = row, .col = col });
            
            col_end++;
        }
        else if (c == '(')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_LPAREN, .data = "(", .row = row, .col = col });
            
            col_end++;
        }
        else if (c == ')')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_RPAREN, .data = ")", .row = row, .col = col });
            
            col_end++;
        }
        else
        {
            col_end++;
            curr += c;
        }
    }

    if (curr != "")
    {
        tokens.push_back(lex_word(curr, row, col));
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
    int row = 1;
    std::vector<Token> tokens;

    while (getline(in, line))
    {
        if (line.size() && line.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
        {
            lex_line(line, tokens, row);
            tokens.push_back({ Token::TOK_EOL, "", .row = row, .col = (int)(line.size()) + 1 });
        }

        row++;  
    }

    tokens.push_back({ Token::TOK_EOF, "", .row = row, .col = 1 });
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

    out << "\t" << token.data << "\t" << token.row << ":" << token.col << "\n";
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