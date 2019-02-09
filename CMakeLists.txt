cmake_minimum_required(VERSION 3.13)
project(mips_asm)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pthread")
add_executable(mips_asm
        main.cpp
        fsm/finite_state_machine.hpp
        fsm/transition_table.hpp
        fsm/transition.hpp
        lexer/lexer.hpp
        lexer/lexer.cpp lexer/token_type.hpp lexer/token.hpp spec/registers.hpp spec/instruction.hpp emitter/emitter.hpp emitter/emitter.cpp emitter/op_sequences.hpp lexer/token_type.cpp emitter/emitter_context.hpp lexer/lexer_context.hpp emitter/op_sequences.cpp)