#pragma once

#include "lexer.h"

#include <string>

constexpr std::string ANSI_COLOR_RED = "\033[31m";
constexpr std::string ANSI_COLOR_GREEN = "\033[32m";
constexpr std::string ANSI_COLOR_YELLOW = "\033[33m";
constexpr std::string ANSI_COLOR_BLUE = "\033[34m";
constexpr std::string ANSI_COLOR_RESET = "\033[0m";

void print_token(const Token& token);
