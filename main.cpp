#include <iostream>

#include "lexer/lexer.hpp"

int main()
{
    as::lexer test;
    auto out = test.lex(".hello\n.text\n__start:\nadd $00,");

    for(auto& e : out.value()) {
        std::cout << e.getName() << " " << std::get<std::string>(e.getAttribute()) << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}