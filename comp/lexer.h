#pragma once

#include <cassert>
#include <string>
#include "token.h"

struct Keyword {
    const char* name;
    TokenType type;
};

extern Keyword keywords[];

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int col;
};

struct Lexer {
    std::string content;
    std::string filename;
    size_t start = 0;
    size_t current = 0;
    char ch = 0;
    int line = 1;
    int col = 1;
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
