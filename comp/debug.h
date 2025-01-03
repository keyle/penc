#pragma once

#include "lexer.h"

#include <string>

const std::string ANSI_COLOR_RED = "\033[31m";
const std::string ANSI_COLOR_GREEN = "\033[32m";
const std::string ANSI_COLOR_YELLOW = "\033[33m";
const std::string ANSI_COLOR_BLUE = "\033[34m";
const std::string ANSI_COLOR_RESET = "\033[0m";

void print_token(Token& token);
