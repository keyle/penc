#pragma once

#include <cassert>
#include <string>
#include <iostream>

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

constexpr std::string ANSI_COLOR_RED = "\033[31m";
constexpr std::string ANSI_COLOR_GREEN = "\033[32m";
constexpr std::string ANSI_COLOR_YELLOW = "\033[33m";
constexpr std::string ANSI_COLOR_BLUE = "\033[34m";
constexpr std::string ANSI_COLOR_RESET = "\033[0m";

void print_token(const Token& token);


inline void sprint(const std::string& str) {
    std::cout << str << '\n';
}
