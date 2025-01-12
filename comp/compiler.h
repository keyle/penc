#pragma once

#include <cassert>
#include <iostream>
#include <string>

#include "opcode.h"
#include "token.h"

using std::string;
using std::variant;
using std::vector;

constexpr string ANSI_COLOR_RED = "\033[31m";
constexpr string ANSI_COLOR_GREEN = "\033[32m";
constexpr string ANSI_COLOR_YELLOW = "\033[33m";
constexpr string ANSI_COLOR_BLUE = "\033[34m";
constexpr string ANSI_COLOR_RESET = "\033[0m";

struct Keyword {
    const char* name;
    TokenType type;
};

extern Keyword keywords[];

struct Token {
    TokenType type;
    size_t start;
    size_t end;  // not strictly needed, can be derived from start and type, or until the next space
    size_t line;
    size_t col;
};

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

void print_token(const Token& token, const string& content);

enum class Opcode {
    DECL_FUNC,
    DECL_VAR,
    DECL_CONST,
    LOAD_CONSTANT,
    LOAD_LOCAL,
    STORE_LOCAL,
    LOAD_GLOBAL,
    STORE_GLOBAL,
    POP,
    ADD,
    SUB,
    JUMP_IF_FALSE,
    JUMP,
    CALL,
    RETURN,
};

struct Bytecode {
    Opcode opcode;
    size_t operand1;
    size_t operand2;
};

struct FuncDecl {
    string name;
    string return_type;
    vector<string> params = {};
};

struct Parser {
    size_t start = 0;
    size_t pos = 0;
    string& content;
    vector<Token>& tokens;
    vector<Bytecode> bytecode = {};
    vector<FuncDecl> funcdecls = {};
    void parse_tokens();

  private:
    Token& peek();
    Token& take();
    void advance();
    void add(Opcode type, const Token& token);
    string str_from_token(Token& token);
};

inline void print(const string& str) {
    std::cout << str << '\n';
}
