cmake_minimum_required(VERSION 3.13)
project(mips_asm)

set(CMAKE_CXX_STANDARD 17)

add_executable(mips_asm
        main.cpp
        fsm/finite_state_machine.hpp
        fsm/transition_table.hpp
        fsm/transition.hpp
        lexer/lexer.hpp
        lexer/lexer.cpp lexer/token_type.hpp lexer/token.hpp spec/registers.hpp spec/instruction.hpp)