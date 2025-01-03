#pragma once

#include <cassert>
#include <string>

enum TokenType {
    // error types
    TOKEN_EOF,
    TOKEN_ERROR,
    // identifiers
    TOKEN_IDENTIFIER,
    TOKEN_STRING_LITERAL,
    TOKEN_NUMBER_LITERAL,
    // keywords
    TOKEN_NS,
    TOKEN_USING,
    TOKEN_STRUCT,
    TOKEN_IF,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_ENUM,
    TOKEN_ASYNC,
    TOKEN_AWAIT,
    TOKEN_EXPORT,
    // types
    TOKEN_VOID,
    TOKEN_STRING,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_BOOL,
    TOKEN_ANY,
    TOKEN_LIST,
    // single character tokens
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_ARROW,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_EQUALS,
    TOKEN_TILDE,
    TOKEN_QUESTION,
    TOKEN_EXCLAMATION,
    TOKEN_DOLLAR,
    TOKEN_BIN_AND,
    TOKEN_BIN_OR,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_BANG_EQUAL,
};

struct Keyword {
    const char* name;
    TokenType type;
};

static Keyword keywords[] = {
    {"ns", TOKEN_NS},
    {"using", TOKEN_USING},
    {"struct", TOKEN_STRUCT},
    {"enum", TOKEN_ENUM},
    {"async", TOKEN_ASYNC},
    {"export", TOKEN_EXPORT},
    {"void", TOKEN_VOID},
    {"if", TOKEN_IF},
    {"while", TOKEN_WHILE},
    {"return", TOKEN_RETURN},
    {"await", TOKEN_AWAIT},
    {"string", TOKEN_STRING},
    {"any", TOKEN_ANY},
    {"int", TOKEN_INT},
    {"bool", TOKEN_BOOL},
    {"float", TOKEN_FLOAT},
    {"list", TOKEN_LIST},
    {NULL, TOKEN_ERROR},
};

struct Token {
    TokenType type;
    std::string_view lexeme;
    int line = 0;
    int col = 0;
};

struct Lexer {
    std::string content;
    std::string filename;
    size_t start = 0;
    size_t current = 0;
    char ch = 0;
    int line = 0;
    int col = 0;

    Token scan_token() {
        skip_whitespace();
        start = current;
        if (is_at_end())
            return make_token(TOKEN_EOF);
        char c = advance();
        if (is_alpha(c))
            return identifier();
        if (is_digit(c))
            return number();

        switch (c) {
            case '(': return make_token(TOKEN_LPAREN);
            case ')': return make_token(TOKEN_RPAREN);

        }
        assert(false && "TODO scan_token");
        return make_token(TOKEN_ERROR);
    }

    bool match(char expected) {
        if (is_at_end())
            return false;
        if (peek() != expected)
            return false;
        advance();
        return true;
    }

   private:
    Token make_token(TokenType type) {
        return Token{type, content.substr(start, current - start), line, col};
    }

    bool is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    TokenType identifier_type() {
        for (Keyword* kw = keywords; kw->name; kw++) {
            if (content.substr(start, current - start) == kw->name)
                return kw->type;
        }
        return TOKEN_IDENTIFIER;
    }

    Token identifier() {
        while (is_alpha(peek()) || is_digit(peek()))
            advance();
        return Token{identifier_type(), content.substr(start, current - start), line, col};
    }

    Token number() {
        while (is_digit(peek()))
            advance();
        if (peek() == '.' && is_digit(peek())) {
            advance();
            while (is_digit(peek()))
                advance();
        }
        if (peek() == '_' && is_digit(peek())) {
            advance();
            while (is_digit(peek()) || peek() == '_')
                advance();
        }
        return Token{TOKEN_NUMBER_LITERAL, content.substr(start, current - start), line, col};
    }

    bool is_at_end() const {
        return current >= content.size();
    }

    char advance() {
        ch = content[current++];
        if (ch == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
        return ch;
    }

    char peek() {
        return content[current];
    }

    void skip_whitespace() {
        while (1) {
            // TODO whitespace
            assert(false && "TODO skip_whitespace");
            char c = peek();
        }
    }
};
