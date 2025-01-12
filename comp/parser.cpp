#include "parser.h"
#include "token.h"
#include "utils.h"
#include <cassert>

Token& Parser::peek() {
    return tokens[start];
}

Token& Parser::take() {
    return tokens[start++];
}

void Parser::advance() {
    start++;
}

string Parser::str_from_token(Token& token) {
    return content.substr(token.start, token.end - token.start);
}

string Parser::opcodeToString(Opcode opcode) {
    switch (opcode) {
        case Opcode::DECL_FUNC:
            return "DECL_FUNC";
        case Opcode::DECL_VAR:
            return "DECL_VAR";
        case Opcode::DECL_CONST:
            return "DECL_CONST";
        case Opcode::LOAD_CONSTANT:
            return "LOAD_CONSTANT";
        case Opcode::LOAD_LOCAL:
            return "LOAD_LOCAL";
        case Opcode::STORE_LOCAL:
            return "STORE_LOCAL";
        case Opcode::LOAD_GLOBAL:
            return "LOAD_GLOBAL";
        case Opcode::STORE_GLOBAL:
            return "STORE_GLOBAL";
        case Opcode::POP:
            return "POP";
        case Opcode::ADD:
            return "ADD";
        case Opcode::SUB:
            return "SUB";
        case Opcode::JUMP_IF_FALSE:
            return "JUMP_IF_FALSE";
        case Opcode::JUMP:
            return "JUMP";
        case Opcode::CALL:
            return "CALL";
        case Opcode::RETURN:
            return "RETURN";
        default:
            return "UNKNOWN_OPCODE";
    }
}

void Parser::parse_tokens() {
    std::cout << "parsing tokens\n";
    Token& token = peek();

    do {
        token = take();

        switch (token.type) {
            case TOKEN_VOID:
            case TOKEN_INT:
            case TOKEN_FLOAT:
            case TOKEN_BOOL:
            case TOKEN_LIST:
            case TOKEN_ARRAY: {
                assert(peek().type == TOKEN_IDENTIFIER && "Expected an identifier after type declaration");
                Token& ident = take();
                auto next = peek();
                if (next.type == TOKEN_EQUALS) {
                    // emit a declaration
                    print("decl const or var, emit a declaration");
                } else if (next.type == TOKEN_LPAREN) {
                    // emit a function declaration with parameters
                    print("decl func, emit a function declaration");
                } else if (next.type == TOKEN_LBRACE) {
                    // emit a function declaration without parameters
                    funcdecls.push_back(FuncDecl{
                        .name = str_from_token(ident),
                        .return_type = str_from_token(token)});

                    bytecode.push_back(Bytecode{Opcode::DECL_FUNC, funcdecls.size() - 1, {}});
                    print("decl func emit a function declaration without parameters");
                } else {
                    assert(false && "Expected an equals sign or opening parenthesis after type declaration");
                }
                break;
            }
            case TOKEN_IDENTIFIER:
                print("Rogue identifier without type declaration");
                break;
            default:
                // std::cout << "default\n";
                break;
        }

        print_token(token, content);
    } while (token.type != TOKEN_EOF);
}
