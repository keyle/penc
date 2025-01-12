#pragma once

#include "token.h"
#include <cassert>
#include <iostream>
#include <string>

using std::string;
using std::vector;

enum class Opcode {
    DECL_FUNC,
    DECL_VAR,
    DECL_CONST,
    LOAD_CONSTANT,
    LOAD_LOCAL,
    STORE_LOCAL,
    LOAD_GLOBAL,
    STORE_GLOBAL,
    POP,
    ADD,
    SUB,
    JUMP_IF_FALSE,
    JUMP,
    CALL,
    RETURN,
};

struct Bytecode {
    Opcode opcode;
    size_t operand1;
    size_t operand2;
};

struct FuncDecl {
    string name;
    string return_type;
    vector<string> params = {};
};

struct Parser {
    size_t start = 0;
    size_t pos = 0;
    string& content;
    vector<Token>& tokens;
    vector<Bytecode> bytecode = {};
    vector<FuncDecl> funcdecls = {};
    void parse_tokens();

  private:
    Token& peek();
    Token& take();
    void advance();
    void add(Opcode type, const Token& token);
    string str_from_token(Token& token);
};
