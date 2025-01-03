#include <cstdio>
#include <iostream>
#include <fstream>
#include <format>

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

int main(int argc, char* argv[]) {
    std::string binary = argv[0];

    if (argc < 2) {
        std::cerr << std::format("Usage: {} <file>\n", argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    std::string content = readfile(filename);

    sprint(content);

    Lexer lexer = {content, filename};

    printf("call lexer here\n");

    return 0;
}
