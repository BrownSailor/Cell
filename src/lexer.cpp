#include "include/lexer.h"

std::unordered_map<std::string, Token::Type> INTRINSICS = 
{
    // Intrinsic keywords
    { "return", Token::TOK_RETURN },
    { "dump", Token::TOK_DUMP },
    { "int", Token::TOK_INT },
    { "char", Token::TOK_CHAR },
    { "bool", Token::TOK_BOOL },
    { "str", Token::TOK_STR },
    { "not", Token::TOK_BANG },
    { "or", Token::TOK_LOR },
    { "and", Token::TOK_LAND },
};

/*
 * lex_word
 *    Purpose: returns the token associated with the current word
 * Parameters: word - the current word to be lexed, row - the current row number, col - the current column number, file - current file being lexed
 *    Returns: the token associated with the current word
 */
Token lex_word(const std::string &word, int row, int col, const std::string &file)
{
    if (INTRINSICS.count(word))
    {
        return { .type = INTRINSICS[word], .data = word, .row = row, .col = col, .file = file };
    }
    else
    { 
        try
        { 
            std::stoi(word);
            return { .type = Token::TOK_NUM, .data = word, .row = row, .col = col, .file = file };
        }
        catch (const std::exception &e)
        { 
            return { .type = Token::TOK_ID, .data = word, .row = row, .col = col, .file = file };
        }
    }
}

/*
 * lex_line
 *    Purpose: lexes the current line and adds the tokens to the list of tokens
 * Parameters: line - the current line to be lexed, tokens - the list of tokens, row - the current row number, file - current file being lexed
 *    Returns: none
 */
void lex_line(const std::string &line, std::list<Token> &tokens, int row, const std::string &file)
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
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            continue;
        }
        else if (c == '@')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
            }

            col = col_end;

            tokens.push_back({ .type = Token::TOK_LOOP, .data = "@", .row = row, .col = col, .file = file });
            col_end++;
        }
        else if (c == '&')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
            }

            col = col_end;

            if (line[i + 1] == '&')
            {
                tokens.push_back({ .type = Token::TOK_LAND, .data = "&&", .row = row, .col = col, .file = file });
                i++;
                col_end++;
            }
        }
        else if (c == '|')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
            }

            col = col_end;

            if (line[i + 1] == '|')
            {
                tokens.push_back({ .type = Token::TOK_LOR, .data = "||", .row = row, .col = col, .file = file });
                i++;
                col_end++;
            }
            // TODO: add support for else/ternary operator
        }
        else if (c == '=')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;

            if (line[i + 1] == '=')
            {
                tokens.push_back({ .type = Token::TOK_EQEQ, .data = "==", .row = row, .col = col, .file = file });
                i++;
                col_end += 2;
            }
            else
            {
                tokens.push_back({ .type = Token::TOK_EQ, .data = "=", .row = row, .col = col, .file = file });
                col_end++;
            }
        }
        else if (c == '!')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;

            if (line[i + 1] == '=')
            {
                tokens.push_back({ .type = Token::TOK_NEQ, .data = "!=", .row = row, .col = col, .file = file });
                i++;
                col_end += 2;
            }
            else
            {
                tokens.push_back({ .type = Token::TOK_BANG, .data = "!", .row = row, .col = col, .file = file });
                col_end++;
            }
        }
        else if (c == '<')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;

            if (line[i + 1] == '=')
            {
                tokens.push_back({ .type = Token::TOK_LTE, .data = "<=", .row = row, .col = col, .file = file });
                i++;
                col_end += 2;
            }
            else if (line[i + 1] == '<')
            {
                tokens.push_back({ .type = Token::TOK_SHL, .data = "<<", .row = row, .col = col, .file = file });
                i++;
                col_end += 2;
            }
            else
            {
                tokens.push_back({ .type = Token::TOK_LT, .data = "<", .row = row, .col = col, .file = file });
                col_end++;
            }
        }
        else if (c == '>')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;

            if (line[i + 1] == '=')
            {
                tokens.push_back({ .type = Token::TOK_GTE, .data = ">=", .row = row, .col = col, .file = file });
                i++;
                col_end += 2;
            }
            else if (line[i + 1] == '>')
            {
                tokens.push_back({ .type = Token::TOK_SHR, .data = ">>", .row = row, .col = col, .file = file });
                i++;
                col_end += 2;
            }
            else
            {
                tokens.push_back({ .type = Token::TOK_GT, .data = ">", .row = row, .col = col, .file = file });
                col_end++;
            }
        }
        else if (c == '+')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_PLUS, .data = "+", .row = row, .col = col, .file = file });

            col_end++;
        }
        else if (c == '-')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_MINUS, .data = "-", .row = row, .col = col, .file = file });

            col_end++;
        }
        else if (c == '*')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_STAR, .data = "*", .row = row, .col = col, .file = file });

            col_end++;
        }
        else if (c == '/')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_SLASH, .data = "/", .row = row, .col = col, .file = file });

            col_end++;
        }
        else if (c == '%')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_PERCENT, .data = "%", .row = row, .col = col, .file = file });

            col_end++;
        }
        else if (c == '~')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_TILDA, .data = "~", .row = row, .col = col, .file = file });

            col_end++;
        }
        else if (c == ':')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_COL, .data = ":", .row = row, .col = col, .file = file });

            col_end++;
        }
        else if (c == '{')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_LBRACE, .data = "{", .row = row, .col = col, .file = file });
            
            col_end++;
        }
        else if (c == '}')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_RBRACE, .data = "}", .row = row, .col = col, .file = file });
            
            col_end++;
        }
        else if (c == '(')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_LPAREN, .data = "(", .row = row, .col = col, .file = file });
            
            col_end++;
        }
        else if (c == ')')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_RPAREN, .data = ")", .row = row, .col = col, .file = file });
            
            col_end++;
        }
        else if (c == '[')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;

            tokens.push_back({ .type = Token::TOK_LBRACK, .data = "[", .row = row, .col = col, .file = file });
            col_end++;
        }
        else if (c == ']')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_RBRACK, .data = "]", .row = row, .col = col, .file = file });
            
            col_end++;
        }
        else if (c == ',')
        {
            if (curr != "")
            {
                tokens.push_back(lex_word(curr, row, col, file));
                curr = "";
            }

            col = col_end;
            tokens.push_back({ .type = Token::TOK_COM, .data = ",", .row = row, .col = col, .file = file });

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
        tokens.push_back(lex_word(curr, row, col, file));
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
            lex_line(line, tokens, row, input);
            tokens.push_back({ Token::TOK_EOL, "", .row = row, .col = (int)(line.size()) + 1, .file = input });
        }

        row++;  
    }

    tokens.push_back({ Token::TOK_EOF, "", .row = row, .col = 1, .file = input });
    return tokens;
}

/*
 * print_token
 *    Purpose: prints the token to the output stream
 * Parameters: token - the token to be printed, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to be std::cout
 */
void print_token(const Token &token, std::ostream &out, bool new_line)
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

        case Token::TOK_LIST:
            out << "TOKEN_LIST";
            break;

        case Token::TOK_FUNC:
            out << "TOKEN_FUNCTION";
            break;
        
        case Token::TOK_PROG:
            out << "TOKEN_PROGRAM";
            break;

        // Intrinsic keywords
        case Token::TOK_RETURN:
            out << "TOKEN_RETURN";
            break;

        case Token::TOK_DUMP:
            out << "TOKEN_DUMP";
            break;

        case Token::TOK_INT:
            out << "TOKEN_INT";
            break;

        case Token::TOK_CHAR:
            out << "TOKEN_CHAR";
            break;

        case Token::TOK_BOOL:
            out << "TOKEN_BOOL";
            break;

        case Token::TOK_STR:
            out << "TOKEN_STR";
            break;
        
        case Token::TOK_VOID:
            out << "TOKEN_VOID";
            break;

        case Token::TOK_BANG:
            out << "TOKEN_BANG";
            break;

        // Unary operators
        case Token::TOK_TILDA:
            out << "TOKEN_TILDA";
            break;

        // Binary operators
        case Token::TOK_PLUS:
            out << "TOKEN_PLUS";
            break;

        case Token::TOK_MINUS:  // also unary
            out << "TOKEN_MINUS";
            break;

        case Token::TOK_STAR:   // also unary
            out << "TOKEN_STAR";
            break;

        case Token::TOK_SLASH:
            out << "TOKEN_SLASH";
            break;

        case Token::TOK_PERCENT:
            out << "TOKEN_PERCENT";
            break;

        case Token::TOK_LOR:
            out << "TOKEN_LOR";
            break;

        case Token::TOK_LAND:
            out << "TOKEN_LAND";
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

        case Token::TOK_EQ:
            out << "TOKEN_EQ";
            break;

        case Token::TOK_COM:
            out << "TOKEN_COM";
            break;

        case Token::TOK_LOOP:
            out << "TOKEN_LOOP";
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

    out << ": " << token.data;
    if (new_line)
    {
        out << "\n";
    }
}

/*
 * print_location
 *    Purpose: prints the location of the token to the output stream
 * Parameters: token - the token to be printed, out - the output stream
 *    Returns: none
 *      Notes: the output stream is defaulted to be std::cout
 */
void print_location(const Token &token, std::ostream &out)
{
    out << token.file << ":" << token.row << ":" << token.col;
}

/*
 * print_lex
 *    Purpose: prints each token in the list of tokens
 * Parameters: tokens - the list of tokens, out - the output stream
 *    Returns: none
 *     Notes: the output stream is defaulted to be std::cout
 */
void print_lex(const std::list<Token> &tokens, std::ostream &out)
{
    for (const auto &token : tokens)
    {
        print_location(token, out);
        out << ":\t";
        print_token(token, out);
    }

    out << "\n";
}