project(mips_asm)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pthread")

add_executable(mips_asm
        main.cpp
        lexer/lexer.cpp
        emitter/emitter.cpp
        lexer/token_type.cpp
        emitter/op_sequences.cpp
        emitter/emitter_context.cpp
        spec/instruction_defs.cpp)
