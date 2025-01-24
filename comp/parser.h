#pragma once

#include "token.h"
#include <iostream>
#include <string>

using std::string;
using std::vector;

// NOTE on change: also update opcodeToString
enum class Opcode {
    NOP,
    LABEL,      //
    DECL_FUNC,  //
    DECL_VAR,
    DECL_CONST,
    LOAD_CONSTANT,
    LOAD_LOCAL,
    STORE_LOCAL,
    LOAD_GLOBAL,
    STORE_GLOBAL,
    POP,
    ADD,
    MULT,
    DIV,
    MOD,
    SUB,
    JUMP_IF_FALSE,
    JUMP,
    CALL,  //
    RETURN,
};

struct Bytecode {
    Opcode opcode;
    union Operand {
        string s;
        size_t i;
    } operand1;
    union Operand operand2;
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
    vector<Bytecode> stack = {};
    string opcodeToString(Opcode opcode);
    void parse_tokens();

  private:
    Token& peek();
    Token& yank();
    void advance();
    void add(Bytecode);
    Bytecode see();       // look at the stack
    void push(Bytecode);  // push to the stack
    Bytecode pop();
    void add(Opcode type, const Token& token);
    string str_from_token(Token& token);
};
