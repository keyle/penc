#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>

#include "compiler.h"

string read_file(const string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        perror(("Error opening file: " + filename).c_str());
        return "";
    }
    return string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
}

void print_usage(const string& binary) {
    std::cerr << std::format("Usage: {} [options] <file>\n", binary);
    std::cerr << "Options:\n";
    // std::cerr << "  -o <file>    Write output to <file>\n";
    // std::cerr << "  -d           Enable debug output\n";
    std::cerr << "  -d           Additional debug prints\n";
    std::cerr << "  -t           Print tokens\n";
    std::cerr << "  -h           Print this help message\n";
}

int main(int argc, char* argv[]) {
    string binary_name = argv[0];
    bool debug_mode = false;
    bool print_tokens = false;

    string input_file;
    string output_file = "out";

    for (int i = 1; i < argc; i++) {
        const string arg = argv[i];
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

    if (argc < 2) {
        std::cerr << "No input file specified.\n";
        print_usage(binary_name);
        return 1;
    }

    string filename = input_file;
    string content = read_file(filename);

    if (debug_mode) {
        std::cerr << "Debug mode enabled\n";
        std::cout << content << '\n';
    }

    // lexing

    Lexer lexer = {.filename = filename, .content = content};
    Token token;
    vector<Token> tokens;

    do {
        token = lexer.scan_token();

        if (!tokens.empty() && tokens.back().type == TOKEN_END && token.type == TOKEN_END)
            continue;  // don't duplicate

        tokens.push_back(token);
    } while (token.type != TOKEN_ERROR && token.type != TOKEN_EOF);

    if (print_tokens) {
        std::cerr << "Print tokens enabled\n";
        for (const Token& tok : tokens) {
            print_token(tok, content);
        }
    }

    // parsing

    Parser parser = {.content = content, .tokens = tokens};
    parser.parse_tokens();

    return 0;
}
