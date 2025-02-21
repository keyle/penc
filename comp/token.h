#pragma once

#include <cstddef>

enum TokenType {
    // error types
    TOKEN_EOF = 5,
    TOKEN_ERROR,
    // identifiers
    TOKEN_IDENTIFIER = 8,
    TOKEN_INTERPOLATION,
    TOKEN_STRING_LITERAL,
    TOKEN_NUMBER_LITERAL,
    // keywords
    TOKEN_NS = 13,
    TOKEN_USING,
    TOKEN_TYPE,
    TOKEN_EXTEND,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_ENUM,
    TOKEN_STRUCT,
    TOKEN_ASYNC,
    TOKEN_AWAIT,
    TOKEN_EXPORT,
    TOKEN_CONST,
    // types
    TOKEN_VOID = 28,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_BOOL,
    TOKEN_FUNC,
    TOKEN_LIST,
    TOKEN_ARRAY,
    // single character tokens
    TOKEN_COLON = 37,
    TOKEN_END,
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
    TOKEN_ARROW = 65,
    TOKEN_AUTO_ASSIGN,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG_EQUAL,
};

struct Token {
    TokenType type;
    size_t start;
    size_t end;  // not strictly needed, can be derived from start and type, or until the next space
    size_t line;
    size_t col;
};
