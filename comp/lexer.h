#pragma once

#include "token.h"
#include <iostream>

using std::string;
using std::vector;

struct Keyword {
    const char* name;
    TokenType type;
};

extern Keyword keywords[];
struct Lexer {
    string& filename;
    string& content;
    size_t start = 0;
    size_t current = 0;
    char ch = 0;
    size_t line = 1;
    size_t col = 1;
    bool within_interpolation = false;

    Token scan_token();

  private:
    Token make_token(TokenType type);
    bool is_alpha(char c);
    bool is_digit(char c);
    TokenType identifier_type();
    Token identifier();
    Token string_with_x_quotes(bool single_quote);
    Token string_interpolation();
    Token number();
    bool is_at_end() const;
    char advance();
    char peek();
    char peek_next();
    void skip_whitespace();
};
