#include <iostream>

#include "lexer/lexer.hpp"

int main()
{
    as::lexer test;
    auto out = test.lex(".hello\n.text\n__start:\nadd $02, $t1, $a0");

    for(auto& e : out.value()) {
        std::cout << e.getName() << " " << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}