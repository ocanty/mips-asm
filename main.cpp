#include <iostream>

#include "lexer/lexer.hpp"

int main()
{
    as::lexer test;
    auto out = test.lex(".hello ");

    for(auto& e : out.value()) {
        std::cout << (signed int)e.getType() << " " << std::get<std::string>(e.getAttribute()) << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}