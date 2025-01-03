#include "debug.h"
#include "lexer.h"

#include <iostream>

void print_token(Token& token) {
    std::cout << ANSI_COLOR_YELLOW << "[L" << token.line << ":" << token.col << "] " << ANSI_COLOR_RESET;
    std::cout << ANSI_COLOR_BLUE;
    switch (token.type) {
        case TOKEN_LPAREN:
            std::cout << "LPAREN";
            break;
        case TOKEN_RPAREN:
            std::cout << "RPAREN";
            break;
        case TOKEN_LBRACE:
            std::cout << "LBRACE";
            break;
        case TOKEN_RBRACE:
            std::cout << "RBRACE";
            break;
        case TOKEN_LBRACKET:
            std::cout << "LBRACKET";
            break;
        case TOKEN_RBRACKET:
            std::cout << "RBRACKET";
            break;
        case TOKEN_ARROW:
            std::cout << "ARROW";
            break;
        case TOKEN_PLUS:
            std::cout << "PLUS";
            break;
        case TOKEN_MINUS:
            std::cout << "MINUS";
            break;
        case TOKEN_STAR:
            std::cout << "STAR";
            break;
        case TOKEN_SLASH:
            std::cout << "SLASH";
            break;
        case TOKEN_EQUALS:
            std::cout << "EQUALS";
            break;
        case TOKEN_TILDE:
            std::cout << "TILDE";
            break;
        case TOKEN_QUESTION:
            std::cout << "QUESTION";
            break;
        case TOKEN_EXCLAMATION:
            std::cout << "EXCLAMATION";
            break;
        case TOKEN_DOLLAR:
            std::cout << "DOLLAR";
            break;
        case TOKEN_BIN_AND:
            std::cout << "BIN_AND";
            break;
        case TOKEN_BIN_OR:
            std::cout << "BIN_OR";
            break;
        case TOKEN_LESS:
            std::cout << "LESS";
            break;
        case TOKEN_LESS_EQUAL:
            std::cout << "LESS_EQUAL";
            break;
        case TOKEN_GREATER:
            std::cout << "GREATER";
            break;
        case TOKEN_GREATER_EQUAL:
            std::cout << "GREATER_EQUAL";
            break;
        case TOKEN_EQUAL_EQUAL:
            std::cout << "EQUAL_EQUAL";
            break;
        case TOKEN_BANG_EQUAL:
            std::cout << "BANG_EQUAL";
            break;
        case TOKEN_STRING_LITERAL:
            std::cout << "STRING_LITERAL";
            break;
        case TOKEN_NUMBER_LITERAL:
            std::cout << "NUMBER_LITERAL";
            break;
        case TOKEN_EOF:
            std::cout << "EOF";
            break;
        case TOKEN_IDENTIFIER:
            std::cout << "IDENTIFIER";
            break;
        case TOKEN_ERROR:
            std::cout << "ERROR";
            break;
        default:
            std::cout << "OTHER (" << token.type << ")";
    }
    std::cout << ANSI_COLOR_RESET << " " << token.lexeme << '\n';
}
