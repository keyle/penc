#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>

#include "debug.h"
#include "lexer.h"
#include "utils.h"

std::string readfile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        perror(("Error opening file: " + filename).c_str());
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
}

void print_usage(const std::string& binary) {
    std::cerr << std::format("Usage: {} [options] <file>\n", binary);
    std::cerr << "Options:\n";
    // std::cerr << "  -o <file>    Write output to <file>\n";
    // std::cerr << "  -d           Enable debug output\n";
    std::cerr << "  -t           Print tokens\n";
    std::cerr << "  -h           Print this help message\n";
}

int main(int argc, char* argv[]) {
    std::string binary_name = argv[0];
    bool debug_mode = false;
    bool print_tokens = false;

    std::string input_file;
    std::string output_file = "out";

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];
        if (arg == "-d") {
            debug_mode = true;
        } else if (arg == "-t") {
            print_tokens = true;
        } else if (arg == "-h") {
            print_usage(binary_name);
            return 0;
        } else if (arg == "-o") {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                std::cerr << "Expected argument after -o\n";
                return 1;
            }
        } else if (arg[0] == '-') {
            std::cerr << std::format("Unknown option: {}, aborting.\n", arg);
            print_usage(binary_name);
            return 1;
        } else {
            if (input_file != "") {
                std::cerr << "Multiple input files are not yet supported.\n";
                print_usage(binary_name);
                return 1;
            }
            input_file = arg;
        }
    }

    if (debug_mode) {
        std::cerr << "Debug mode enabled\n";
    }

    if (print_tokens) {
        std::cerr << "Print tokens enabled\n";
    }

    if (argc < 2) {
        std::cerr << "No input file specified.\n";
        print_usage(binary_name);
        return 1;
    }

    std::string filename = input_file;
    std::string content = readfile(filename);

    Lexer lexer;
    lexer = {content, filename};

    if (print_tokens) {
        Token token;
        do {
            token = lexer.scan_token();
            print_token(token);
            if (token.type == TOKEN_EOF) {
                break;
            }
        } while (token.type != TOKEN_ERROR);
    }

    sprint(content);

    return 0;
}
