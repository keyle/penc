#pragma once

#include <cassert>
#include <string>

enum TokenType {
    // error types
    TOKEN_EOF,
    TOKEN_ERROR,
    // identifiers
    TOKEN_IDENTIFIER,
    TOKEN_INTERPOLATION,
    TOKEN_STRING_LITERAL,
    TOKEN_NUMBER_LITERAL,
    // keywords
    TOKEN_NS,
    TOKEN_USING,
    TOKEN_TYPE,
    TOKEN_EXTEND,
    TOKEN_IF,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_ENUM,
    TOKEN_ASYNC,
    TOKEN_AWAIT,
    TOKEN_EXPORT,
    TOKEN_CONST,
    // types
    TOKEN_VOID,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_BOOL,
    TOKEN_FUNC,
    TOKEN_LIST,
    // single character tokens
    TOKEN_COLON,
    TOKEN_ENDSTATEMENT,
    TOKEN_AT,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_BACKTICK,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_EQUALS,
    TOKEN_TILDE,
    TOKEN_QUESTION,
    TOKEN_EXCLAMATION,
    TOKEN_AMPERSAND,
    TOKEN_PERCENT,
    TOKEN_DOLLAR,
    TOKEN_BIN_AND,
    TOKEN_BIN_OR,
    TOKEN_LESS,
    TOKEN_GREATER,
    // binop and operators
    TOKEN_ARROW,
    TOKEN_AUTO_ASSIGN,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG_EQUAL,
};

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
    void skip_whitespace();
};
