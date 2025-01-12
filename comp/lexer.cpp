#include "compiler.h"

Keyword keywords[] = {
    {    "ns",     TOKEN_NS},
    { "using",  TOKEN_USING},
    {  "type",   TOKEN_TYPE},
    {"extend", TOKEN_EXTEND},
    {  "enum",   TOKEN_ENUM},
    { "async",  TOKEN_ASYNC},
    {"export", TOKEN_EXPORT},
    {    "if",     TOKEN_IF},
    {  "else",   TOKEN_ELSE},
    { "while",  TOKEN_WHILE},
    {"return", TOKEN_RETURN},
    { "await",  TOKEN_AWAIT},
    {"string", TOKEN_STRING},
    { "const",  TOKEN_CONST},
    {   "int",    TOKEN_INT},
    {  "bool",   TOKEN_BOOL},
    {  "func",   TOKEN_FUNC},
    { "float",  TOKEN_FLOAT},
    {  "void",   TOKEN_VOID},
    {  "list",   TOKEN_LIST},
    { nullptr,  TOKEN_ERROR},
};

Token Lexer::scan_token() {
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
        case '(':
            return make_token(TOKEN_LPAREN);
        case ')':
            return make_token(TOKEN_RPAREN);
        case '{':
            return make_token(TOKEN_LBRACE);
        case '}':
            return make_token(TOKEN_RBRACE);
        case '[':
            return make_token(TOKEN_LBRACKET);
        case ']':
            return make_token(TOKEN_RBRACKET);
        case ':':
            if (peek() == '=') {
                advance();
                return make_token(TOKEN_AUTO_ASSIGN);
            } else {
                return make_token(TOKEN_COLON);
            }
        case '\n':
            line++;
            col = 1;
            // fall through
        case ';':
            return make_token(TOKEN_END);
        case '.':
            return make_token(TOKEN_DOT);
        case ',':
            return make_token(TOKEN_COMMA);
        case '+':
            return make_token(TOKEN_PLUS);
        case '-':
            return make_token(TOKEN_MINUS);
        case '*':
            return make_token(TOKEN_STAR);
        case '/':
            return make_token(TOKEN_SLASH);
        case '~':
            return make_token(TOKEN_TILDE);
        case '$':
            return make_token(TOKEN_DOLLAR);
        case '@':
            return make_token(TOKEN_AT);
        case '`':
            return make_token(TOKEN_BACKTICK);
        case '?':
            return make_token(TOKEN_QUESTION);
        case '!':
            if (peek() == '=') {
                advance();
                return make_token(TOKEN_BANG_EQUAL);
            } else {
                return make_token(TOKEN_EXCLAMATION);
            }
        case '=':
            if (peek() == '=') {
                advance();
                return make_token(TOKEN_EQUAL_EQUAL);
            } else if (peek() == '>') {
                advance();
                return make_token(TOKEN_ARROW);
            } else {
                return make_token(TOKEN_EQUALS);
            }
        case '>':
            if (peek() == '=') {
                advance();
                return make_token(TOKEN_GREATER_EQUAL);
            } else {
                return make_token(TOKEN_GREATER);
            }
        case '<':
            if (peek() == '=') {
                advance();
                return make_token(TOKEN_LESS_EQUAL);
            } else {
                return make_token(TOKEN_LESS);
            }
        case '&':
            if (peek() == '&') {
                advance();
                return make_token(TOKEN_BIN_AND);
            } else {
                return make_token(TOKEN_AMPERSAND);
            }
        case '|':
            if (peek() == '|') {
                advance();
                return make_token(TOKEN_BIN_OR);
            }
        case '%':
            if (within_interpolation)
                return string_with_x_quotes(true);
            else
                return make_token(TOKEN_PERCENT);
            break;
        case '"':
            // TODO parse string interpolation within here. Unroll this function and return STRING_LITERAL + INTERPOLATED + STRING_LITERAL...
            // basically we need a state to say we are within a string literal and we should only get out of it when we find an actual second `"`
            return string_with_x_quotes(true);
        case '\'':
            return string_with_x_quotes(false);

            // Add more cases as needed
    }

    return make_token(TOKEN_ERROR);
}

Token Lexer::make_token(TokenType type) {
    // return Token{type, content.substr(start, current - start), line, col};
    return Token{.type = type, .start = start, .end = current, .line = line, .col = col};
}

bool Lexer::is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::is_digit(char c) {
    return c >= '0' && c <= '9';
}

TokenType Lexer::identifier_type() {
    for (Keyword* kw = keywords; kw->name; ++kw) {
        if (content.substr(start, current - start) == kw->name)
            return kw->type;
    }
    return TOKEN_IDENTIFIER;
}

Token Lexer::identifier() {
    while (is_alpha(peek()) || is_digit(peek()))
        advance();
    return make_token(identifier_type());
}

Token Lexer::number() {
    while (is_digit(peek()))
        advance();
    return make_token(TOKEN_NUMBER_LITERAL);
}

//
// NOTE interpolation: for now we only support interpolation within double quotes
//
// we also don't support anything else than a simple ident technically
// although there is no reason why we couldn't.
//
Token Lexer::string_with_x_quotes(bool double_quote) {
    const char quote = double_quote ? '"' : '\'';
    bool interpolation = false;
    while (!is_at_end()) {
        const char peeked = peek();
        if (peeked == quote) {
            break;
        }
        if (double_quote && peeked == '%') {
            interpolation = true;
            break;
        }
        if (peeked == '\n') {  // support multiline strings
            ++line;
        }
        advance();
    }

    if (is_at_end())
        return Token{.type = TOKEN_ERROR, .start = start, .end = current, .line = line, .col = col};

    if (interpolation)
        within_interpolation = true;  // tell the lexer to resume string later

    advance();  // advanced over the closing quote or opening % of interpolation inside double quotes

    return make_token(TOKEN_STRING_LITERAL);
}

bool Lexer::is_at_end() const {
    return current >= content.size();
}

char Lexer::advance() {
    ch = content[current++];
    // if (ch == '\n') {
    //     ++line;
    //     col = 1;
    // } else {
    //     ++col;
    // }
    ++col;
    return ch;
}

char Lexer::peek() {
    return content[current];
}

char Lexer::peek_next() {
    if (content.length() <= current + 1)
        return '\0';
    return content[current + 1];
}

void Lexer::skip_whitespace() {
    while (1) {
        char c = peek();
        if (c == ' ' || c == '\t') {  // || c == '\r' || c == '\n') {
            advance();
        } else if (c == '/' && peek_next() == '/') {
            while (peek() != '\n' && !is_at_end())
                advance();
        } else if (c == '/' && peek_next() == '*') {
            advance();
            advance();
            while (peek() != '*' && peek_next() != '/' && !is_at_end()) {
                if (peek() == '\n') {
                    line++;
                    col = 1;
                }
                advance();
            }
            advance();
            advance();
        } else {
            break;
        }
    }
}
