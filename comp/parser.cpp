#include "parser.h"
#include "token.h"
#include "utils.h"

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
                std::cout << "type, emit a declaration\n";
                break;
            }
            case TOKEN_IDENTIFIER:
                assert(false && "Rogue identifier without type declaration");
                break;
            default:
                std::cout << "default\n";
        }

        print_token(token, content);
    } while (token.type != TOKEN_EOF);
}
