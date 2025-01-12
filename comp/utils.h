#pragma once

#include "token.h"
#include "parser.h"
#include <string>

using std::string;

void print_token(const Token& token, const string& content);
void print_bc(const Bytecode& bc, Parser& parser);

constexpr string ANSI_COLOR_RED = "\033[31m";
constexpr string ANSI_COLOR_GREEN = "\033[32m";
constexpr string ANSI_COLOR_YELLOW = "\033[33m";
constexpr string ANSI_COLOR_BLUE = "\033[34m";
constexpr string ANSI_COLOR_RESET = "\033[0m";


inline void print(const string& str) { std::cout << str << '\n'; }
