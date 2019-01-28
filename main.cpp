#include <iostream>
#include <fstream>
#include <algorithm>

#include "lexer/lexer.hpp"
#include "emitter/emitter.hpp"

int main()
{
    // Open assembly file
    std::fstream file("test.s", std::ios::in);


    // Seek to end to get size
    file.seekg(0, std::ios_base::end);
    auto size = file.tellg();

    // Seek back to beginning
    file.seekg(0, std::ios_base::beg);

    // Create our input buffer
    std::string input;
    input.resize(size);

    file.read(&input[0],size);

    // replace tabs with correct whitespace
    // as our lexer does not expect tabs
    std::replace(input.begin(),input.end(),'\t',' ');

    as::lexer test;

    auto tokens = test.lex(input);

    if(tokens != std::nullopt) {
        as::emitter emitter;
        auto binary = emitter.emit(tokens.value());
    }

    return 0;
}