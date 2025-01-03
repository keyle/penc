#include <cstdio>
#include <iostream>
#include <fstream>

#include "lexer.h"

std::string readfile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        perror(("Error opening file: " + filename).c_str());
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::string err = "Usage: app <file>";
        std::cerr << err << '\n';
        return 1;
    }

    std::string filename = argv[1];
    std::string content = readfile(filename);
    std::cout << content << '\n';

    Lexer lexer = {content, filename};

    printf("call lexer here\n");

    return 0;
}
