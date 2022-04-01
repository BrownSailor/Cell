#include "include/lexer.h"

std::unordered_map<std::string, Token::Type> INTRINSICS = 
{
    // Intrinsic keywords
    { "return", Token::TOK_RETURN },

    // Unary operators
    { "~", Token::TOK_TILDA },
    { "[]", Token::TOK_ARR },

    // Binary operators
    { "+", Token::TOK_PLUS },
    { "-", Token::TOK_MINUS },
    { "*", Token::TOK_STAR },
    { "/", Token::TOK_SLASH },
    { "%", Token::TOK_PERCENT },
    { "or", Token::TOK_BOR },
    { "and", Token::TOK_BAND },
    { "xor", Token::TOK_BXOR },
    { "<<", Token::TOK_SHL },
    { ">>", Token::TOK_SHR },

    // Boolean operations
    { "==", Token::TOK_EQEQ },
    { "!=", Token::TOK_NEQ },
    { "<", Token::TOK_LT },
    { ">", Token::TOK_GT },
    { "<=", Token::TOK_LTE },
    { ">=", Token::TOK_GTE },

    // Single character tokens
    { "{", Token::TOK_LBRACE },
    { "}", Token::TOK_RBRACE },
    { "(", Token::TOK_LPAREN },
    { ")", Token::TOK_RPAREN },
    { "[", Token::TOK_LBRACK },
    { "]", Token::TOK_RBRACK },
    { ":", Token::TOK_COL },
    { ",", Token::TOK_COM },
    { "", Token::TOK_EOL },
    { "", Token::TOK_EOF }
};

/*
 * lex_word
 *    Purpose: returns the token associated with the current word
 * Parameters: word - the current word to be lexed, row - the current row number, col - the current column number
 *    Returns: the token associated with the current word
 */
Token lex_word(const std::string &word, int row, int col)
{
    if (INTRINSICS.count(word))
    {
        return { .type = INTRINSICS[word], .data = word, .row = row, .col = col };
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
void lex_line(const std::string &line, std::list<Token> &tokens, int row)
{
    std::string curr = "";
    int col = 1;
    int col_end = 1;

    for (size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];
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
        else if (c == '[')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;

            if (line[i + 1] == ']')
            {
                tokens.push_back({ .type = Token::TOK_ARR, .data = "[]", .row = row, .col = col });
                i++;
                col_end += 2;
            }
            else
            {
                tokens.push_back({ .type = Token::TOK_LBRACK, .data = "[", .row = row, .col = col });
                col_end++;
            }
        }
        else if (c == ']')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_RBRACK, .data = "]", .row = row, .col = col });
            
            col_end++;
        }
        else if (c == ',')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_COM, .data = ",", .row = row, .col = col });

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
std::list<Token> lex(const std::string &input)
{ 
    std::ifstream in;
    in.open(input);

    std::string line;
    int row = 1;
    std::list<Token> tokens;

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
        // Non built-ins
        case Token::TOK_ID:
            out << "TOKEN_ID";
            break;

        case Token::TOK_NUM:
            out << "TOKEN_NUM";
            break;

        // Intrinsic keywords
        case Token::TOK_RETURN:
            out << "TOKEN_RETURN";
            break;

        // Unary operators
        case Token::TOK_TILDA:
            out << "TOKEN_TILDA";
            break;

        case Token::TOK_ARR:
            out << "TOKEN_ARR";
            break;

        // Binary operators
        case Token::TOK_PLUS:
            out << "TOKEN_PLUS";
            break;

        case Token::TOK_MINUS:  // also unary
            out << "TOKEN_MINUS";
            break;

        case Token::TOK_STAR:
            out << "TOKEN_STAR";
            break;

        case Token::TOK_SLASH:
            out << "TOKEN_SLASH";
            break;

        case Token::TOK_PERCENT:
            out << "TOKEN_PERCENT";
            break;

        case Token::TOK_BOR:
            out << "TOKEN_BOR";
            break;

        case Token::TOK_BAND:
            out << "TOKEN_BAND";
            break;

        case Token::TOK_BXOR:
            out << "TOKEN_BXOR";
            break;

        case Token::TOK_SHL:
            out << "TOKEN_SHL";
            break;

        case Token::TOK_SHR:
            out << "TOKEN_SHR";
            break;

        // Boolean operations
        case Token::TOK_EQEQ:
            out << "TOKEN_EQEQ";
            break;

        case Token::TOK_NEQ:
            out << "TOKEN_NEQ";
            break;

        case Token::TOK_LT:
            out << "TOKEN_LT";
            break;

        case Token::TOK_GT:
            out << "TOKEN_GT";
            break;

        case Token::TOK_LTE:
            out << "TOKEN_LTE";
            break;

        case Token::TOK_GTE:
            out << "TOKEN_GTE";
            break;

        // Single character tokens
        case Token::TOK_LBRACE:
            out << "TOKEN_LBRACE";
            break;

        case Token::TOK_RBRACE:
            out << "TOKEN_RBRACE";
            break;

        case Token::TOK_LPAREN:
            out << "TOKEN_LPAREN";
            break;

        case Token::TOK_RPAREN:
            out << "TOKEN_RPAREN";
            break;

        case Token::TOK_LBRACK:
            out << "TOKEN_LBRACK";
            break;

        case Token::TOK_RBRACK:
            out << "TOKEN_RBRACK";
            break;

        case Token::TOK_COL:
            out << "TOKEN_COL";
            break;

        case Token::TOK_COM:
            out << "TOKEN_COM";
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
void print_lex(const std::list<Token> &tokens)
{
    for (const auto &token : tokens)
    {
        print_token(token, std::cout);
    }
}