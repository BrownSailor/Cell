#include "lexer.hpp"

std::unordered_map<std::string, Token::Type> INTRINSICS =
{
    { "num", Token::KEY_NUM },
    { "bool", Token::KEY_BOOL },
    { "tru", Token::TOK_TRU },
    { "fls", Token::TOK_FLS },
    { "str", Token::KEY_STR },
    { "unit", Token::KEY_UNIT },
    { "nil", Token::KEY_NIL },
    { "shl", Token::TOK_SHL },
    { "shr", Token::TOK_SHR },
    { "and", Token::TOK_AND },
    { "or", Token::TOK_OR },
    { "not", Token::TOK_NOT },
    { "fn", Token::KEY_FUN },
    { "print", Token::KEY_PRINT },
};

Token new_token(Token::Type type, std::string data, int col, int row, std::string file)
{
    Token token = { .type = type, .data = data, .col = col, .row = row, .file = file };
    return token;
}

static Token lex_word(const std::string &word, int col, int row, const std::string &file)
{
    if (INTRINSICS.count(word))
    {
        return new_token(INTRINSICS[word], word, col, row, file);
    }
    else
    {
        try
        {
            std::stoull(word);
            return new_token(Token::TOK_NUM, word, col, row, file);
        }
        catch (const std::exception &e)
        {
            return new_token(Token::TOK_ID, word, col, row, file);
        }
    }
}

static void lex_line(const std::string &line, std::list<Token> &tokens, int row, const std::string &file)
{
    std::string curr = "";
    int col = 1, col_end = 1;

    for (size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];
        switch (c)
        {
            case ' ':
            {
                col_end++;
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end;
                break;
            }
            
            case ';':
            {
                return;
            }

            case '\'':
            {
                c = line[++i];
                col_end++;

                while (c != '\'')
                {
                    curr += c;
                    c = line[++i];
                    col_end++;
                }

                tokens.push_back(new_token(Token::TOK_STR, curr, col, row, file)); 
                curr = "";
                col = col_end++;
                break;
            }

            case '"':
            {
                c = line[++i];
                col_end++;

                while (c != '"')
                {
                    curr += c;
                    c = line[++i];
                    col_end++;
                }

                tokens.push_back(new_token(Token::TOK_STR, curr, col, row, file)); 
                curr = "";
                col = col_end++;
                break;
            }

            case '?':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_IF, "?", col, row, file));
                break;
            }

            case '@':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_LOOP, "@", col, row, file));
                break;
            }

            case '|':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_ELSE, "|", col, row, file));
                break;
            }

            case '=':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_EQ, "=", col, row, file));
                break;
            }
            
            case '!':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                }

                col = col_end;

                if (line[i + 1] == '=')
                {
                    tokens.push_back(new_token(Token::TOK_NEQ, "!=", col, row, file));
                    i++;
                    col_end += 2;
                }

                break;
            }

            case '<':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end;

                if (line[i + 1] == '=')
                {
                    tokens.push_back(new_token(Token::TOK_LTE, "<=", col, row, file));
                    i++;
                    col_end += 2;
                }
                else
                {
                    tokens.push_back(new_token(Token::TOK_LT, "<", col, row, file));
                    col_end++;
                }
                break;
            }

            case '>':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end;

                if (line[i + 1] == '=')
                {
                    tokens.push_back(new_token(Token::TOK_GTE, ">=", col, row, file));
                    i++;
                    col_end += 2;
                }
                else
                {
                    tokens.push_back(new_token(Token::TOK_GT, ">", col, row, file));
                    col_end++;
                }
                break;
            }

            case '+':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end;
                tokens.push_back(new_token(Token::TOK_ADD, "+", col, row, file));
                col_end++;
                break;
            }

            case '-':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end;

                if (line[i + 1] == '>')
                {
                    tokens.push_back(new_token(Token::TOK_ARROW, "->", col, row, file));
                    i++;
                    col_end += 2;
                }
                else
                {
                    tokens.push_back(new_token(Token::TOK_SUB, "-", col, row, file));
                    col_end++;
                }
                break;
            }

            case '*':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_MUL, "*", col, row, file));
                break;
            }

            case '/':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_DIV, "/", col, row, file));
                break;
            }

            case '%':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_MOD, "%", col, row, file));
                break;
            }

            case ':':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_COLON, ":", col, row, file));
                break;
            }

            case '{':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_LBRACE, "{", col, row, file));
                break;
            }

            case '}':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_RBRACE, "}", col, row, file));
                break;
            }

            case '[':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_LBRACK, "[", col, row, file));
                break;
            }

            case ']':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_RBRACK, "]", col, row, file));
                break;
            }

            case '(':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_LPAREN, "(", col, row, file));
                break;
            }

            case ')':
            {
                if (curr.size())
                {
                    tokens.push_back(lex_word(curr, col, row, file));
                    curr = "";
                }

                col = col_end++;
                tokens.push_back(new_token(Token::TOK_RPAREN, ")", col, row, file));
                break;
            }

            default:
            {
                col_end++;
                curr += c;
                break;
            }
        }
    }

    if (curr.size())
    {
        tokens.push_back(lex_word(curr, col, row, file));
        curr = "";
    }
}

std::list<Token> lex(const std::string &file)
{
    std::ifstream in(file);

    std::string line;
    int row = 1;
    std::list<Token> tokens;

    while (getline(in, line))
    {
        if (line.size() && line.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
        {
            lex_line(line, tokens, row, file);
        }

        row++;
    }

    return tokens;
}

void print_token(Token token)
{
    std::cout << token.file << ":" << token.row << ":" << token.col << "\t";

    switch (token.type)
    {
        case Token::KEY_NUM: std::cout << "KEY_NUM\t"; break;
        case Token::KEY_BOOL: std::cout << "KEY_BOOL"; break;
        case Token::TOK_TRU: std::cout << "KEY_TRU\t"; break;
        case Token::TOK_FLS: std::cout << "KEY_FLS\t"; break;
        case Token::KEY_STR: std::cout << "KEY_STR\t"; break;
        case Token::KEY_UNIT: std::cout << "KEY_UNIT\t"; break;
        case Token::KEY_NIL: std::cout << "KEY_NIL\t"; break;
        case Token::KEY_PRINT: std::cout << "KEY_PRINT\t"; break;
        case Token::TOK_AND: std::cout << "TOK_AND\t"; break;
        case Token::TOK_OR: std::cout << "TOK_OR\t"; break;
        case Token::TOK_NOT: std::cout << "TOK_NOT\t"; break;
        case Token::KEY_FUN: std::cout << "KEY_FUN\t"; break;
        case Token::TOK_LPAREN: std::cout << "TOK_LPAREN"; break;
        case Token::TOK_LBRACK: std::cout << "TOK_LBRACK"; break;
        case Token::TOK_LBRACE: std::cout << "TOK_LBRACE"; break;
        case Token::TOK_RPAREN: std::cout << "TOK_RPAREN"; break;
        case Token::TOK_RBRACK: std::cout << "TOK_RBRACK"; break;
        case Token::TOK_RBRACE: std::cout << "TOK_RBRACE"; break;
        case Token::TOK_ARROW: std::cout << "TOK_ARROW"; break;
        case Token::TOK_SEMI: std::cout << "TOK_SEMI"; break;
        case Token::TOK_COLON: std::cout << "TOK_COLON"; break;
        case Token::TOK_ADD: std::cout << "TOK_ADD\t"; break;
        case Token::TOK_SUB: std::cout << "TOK_SUB\t"; break;
        case Token::TOK_MUL: std::cout << "TOK_MUL\t"; break;
        case Token::TOK_DIV: std::cout << "TOK_DIV\t"; break;
        case Token::TOK_MOD: std::cout << "TOK_MOD\t"; break;
        case Token::TOK_SHL: std::cout << "TOK_SHL\t"; break;
        case Token::TOK_SHR: std::cout << "TOK_SHR\t"; break;
        case Token::TOK_EQ: std::cout << "TOK_EQ\t"; break;
        case Token::TOK_NEQ: std::cout << "TOK_NEQ\t"; break;
        case Token::TOK_LT: std::cout << "TOK_LT\t"; break;
        case Token::TOK_LTE: std::cout << "TOK_LTE\t"; break;
        case Token::TOK_GT: std::cout << "TOK_GT\t"; break;
        case Token::TOK_GTE: std::cout << "TOK_GTE\t"; break;
        case Token::TOK_LOOP: std::cout << "TOK_LOOP"; break;
        case Token::TOK_IF: std::cout << "TOK_IF\t"; break;
        case Token::TOK_ELSE: std::cout << "TOK_ELSE"; break;
        case Token::TOK_NUM: std::cout << "TOK_NUM\t"; break;
        case Token::TOK_STR: std::cout << "TOK_STR\t"; break;
        case Token::TOK_ID: std::cout << "TOK_ID\t"; break;
    }

    std::cout << "\t" << token.data << "\n";
}
