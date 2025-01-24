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

void Parser::add(Bytecode bc) {
    // bytecode.push_back(bc);
}

void Parser::push(Bytecode bc) {
    // stack.push_back(bc);
}

Bytecode Parser::see() {
    if(stack.empty()) {
        // return Bytecode{Opcode::NOP, 0, 0};
    }
    // return stack.back();
}

Bytecode Parser::pop() {
    // const auto bc = stack.back();
    // stack.pop_back();
    // return bc;
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
        case Opcode::MULT:
            return "MULT";
        case Opcode::DIV:
            return "DIV";
        case Opcode::MOD:
            return "MOD";
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
            // identifier first
            case TOKEN_IDENTIFIER: {
                printf("identifier: \t");
                print_token(token, content);

                // look at the stack first, if it's a binop, pop it and push after in the bytecode
                auto lexeme = content.substr(token.start, token.end - token.start);
                // add(Bytecode{Opcode::LOAD_LOCAL, lexeme, 0});

                // const auto bc = see();
                // if(bc.opcode == Opcode::ADD || bc.opcode == Opcode::SUB || bc.opcode == Opcode::MULT || bc.opcode == Opcode::DIV) {
                //     add(pop());
                // }
                break;
            }
            case TOKEN_STAR: {
                // put it on the stack
                std::cout << "star\n";
                // push(Bytecode{Opcode::MULT, {}, {}});
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
