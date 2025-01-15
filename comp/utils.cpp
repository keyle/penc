#include "utils.h"

#include <iostream>

void print_bc(const Bytecode& bc, Parser& parser) {
    std::cout << ANSI_COLOR_YELLOW << "Bytecode: " << ANSI_COLOR_BLUE << parser.opcodeToString(bc.opcode) << '\t' << bc.operand1 << '\t' << bc.operand2 << ANSI_COLOR_RESET << '\n';
    if (bc.opcode == Opcode::DECL_FUNC) {
        FuncDecl& decl = parser.funcs[bc.operand1];
        std::cout << ANSI_COLOR_YELLOW << "\t\tname: " << ANSI_COLOR_BLUE << decl.name << ANSI_COLOR_GREEN << "\t ret:" << decl.return_type << ANSI_COLOR_RESET << '\n';
    } else if (bc.opcode == Opcode::LABEL) {
        std::cout << ANSI_COLOR_YELLOW << "\t\tlabel: " << ANSI_COLOR_BLUE << parser.labels[bc.operand1] << ANSI_COLOR_RESET << '\n';
    }
}

void print_token(const Token& token, const std::string& content) {
    std::cout << ANSI_COLOR_YELLOW << "[L" << token.line << ":" << token.col << "]    \t" << ANSI_COLOR_RESET;
    std::cout << ANSI_COLOR_BLUE;
    bool print_token = true;
    switch (token.type) {
        case TOKEN_END:
            std::cout << "---- END ----";
            print_token = false;
            break;
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
        case TOKEN_DOT:
            std::cout << "DOT";
            break;
        case TOKEN_EQUALS:
            std::cout << "EQUALS";
            break;
        case TOKEN_TILDE:
            std::cout << "TILDE";
            break;
        case TOKEN_COMMA:
            std::cout << "COMMA";
            break;
        case TOKEN_AMPERSAND:
            std::cout << "AMPERSAND";
            break;
        case TOKEN_AT:
            std::cout << "AT";
            break;
        case TOKEN_BACKTICK:
            std::cout << "BACKTICK";
            break;
        case TOKEN_COLON:
            std::cout << "COLON";
            break;
        case TOKEN_AUTO_ASSIGN:
            std::cout << "AUTO_ASSIGN";  // :=
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
        case TOKEN_INT:
            std::cout << "INT";
            break;
        case TOKEN_STRING:
            std::cout << "STRING";
            break;
        case TOKEN_FLOAT:
            std::cout << "FLOAT";
            break;
        case TOKEN_BOOL:
            std::cout << "BOOL";
            break;
        case TOKEN_VOID:
            std::cout << "VOID";
            break;
        case TOKEN_LIST:
            std::cout << "LIST";
            break;
        case TOKEN_ARRAY:
            std::cout << "ARRAY";
            break;
        case TOKEN_CONST:
            std::cout << "CONST";
            break;
        case TOKEN_ENUM:
            std::cout << "ENUM";
            break;
        case TOKEN_STRUCT:
            std::cout << "STRUCT";
            break;
        case TOKEN_FUNC:
            std::cout << "FUNC";
            break;
        case TOKEN_NS:
            std::cout << "NAMESPACE";
            break;
        case TOKEN_USING:
            std::cout << "USING";
            break;
        case TOKEN_IF:
            std::cout << "IF";
            break;
        case TOKEN_ELSE:
            std::cout << "ELSE";
            break;
        case TOKEN_TYPE:
            std::cout << "TYPE DEF";
            break;
        case TOKEN_EXTEND:
            std::cout << "TYPE EXTENSION";
            break;
        case TOKEN_IDENTIFIER:
            std::cout << "IDENTIFIER";
            break;
            break;
        case TOKEN_INTERPOLATION:
            std::cout << "INTERPOLATION";
            break;
        case TOKEN_ERROR:
            std::cout << "*** ERROR ***";
            break;
        default:
            std::cout << "OTHER (" << token.type << ")";
    }

    if (print_token) {
        auto lexeme = content.substr(token.start, token.end - token.start);
        std::cout << ANSI_COLOR_RESET << " " << lexeme << '\n';
    } else {
        std::cout << ANSI_COLOR_RESET << '\n';
    }
}
