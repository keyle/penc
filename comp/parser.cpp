#include "parser.h"
#include "token.h"
#include "utils.h"
#include <cassert>

Token& Parser::peek() {
    return tokens[start];
}

Token& Parser::yank() {
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
        case Opcode::LABEL:
            return "LABEL";
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
        token = yank();

        // TODO see notes, and rewrite is reverse polish notation RPN
        //
        // 5 2 4 * + x :=
        //

        switch (token.type) {
            // type first
            case TOKEN_VOID:
            case TOKEN_INT:
            case TOKEN_FLOAT:
            case TOKEN_BOOL:
            case TOKEN_LIST:
            case TOKEN_ARRAY: {
                assert(peek().type == TOKEN_IDENTIFIER && "Expected an identifier after type declaration");
                Token& ident = yank();
                const auto next = peek();
                if (next.type == TOKEN_EQUALS) {
                    // emit a declaration
                    print("decl const or var, emit a declaration");
                } else if (next.type == TOKEN_LPAREN) {
                    // emit a function declaration with parameters
                    print("decl func, emit a function declaration");
                } else if (next.type == TOKEN_LBRACE) {
                    // emit a function declaration without parameters
                    const auto ident_name = str_from_token(ident);
                    funcs.push_back(FuncDecl{
                        .name = ident_name,
                        .return_type = str_from_token(token)});
                    const auto index = funcs.size() - 1;
                    labels.push_back(ident_name);
                    const auto label_index = labels.size() - 1;
                    bytecode.push_back(Bytecode{Opcode::DECL_FUNC, index, {}});
                    bytecode.push_back(Bytecode{Opcode::LABEL, label_index, {}});
                    yank();  // consume the LBRACE
                    print("decl func emit a function declaration without parameters");
                } else {
                    assert(false && "Expected an equals sign or opening parenthesis after type declaration");
                }
                break;
            }
            // identifier first
            case TOKEN_IDENTIFIER: {
                printf("identifier: \t");
                print_token(token, content);
                auto next = peek();
                if (next.type == TOKEN_EQUALS) {  // Var (re)Assignment
                    // emit an assignment
                    print("TODO assignment?");
                } else if (next.type == TOKEN_LPAREN) {  // Function call
                    // CALL :func_name
                    const auto ident_name = str_from_token(token);
                    labels.push_back(ident_name);
                    const auto label_index = labels.size() - 1;
                    bytecode.push_back(Bytecode{Opcode::CALL, label_index, {}});
                    while (1) {
                        token = yank();
                        if (token.type == TOKEN_RPAREN) {
                            break;
                        }
                        // check token is a type
                        switch (token.type) {
                            case TOKEN_IDENTIFIER:
                                if (peek().type == TOKEN_LPAREN) {
                                    // argument
                                    break;
                                }
                                break;
                            case TOKEN_COMMA:  // same type as before
                                continue;
                            default:
                                printf("something else <!>: \t");
                                print_token(token, content);
                                break;
                        }
                    }
                    print("TODO function call?");
                } else {
                    printf("something else <!>: \t");
                    print_token(token, content);
                }
                break;
            }
            default:
                printf("something: \t");
                print_token(token, content);
                // std::cout << "default\n";
                break;
        }

    } while (token.type != TOKEN_EOF);
}
