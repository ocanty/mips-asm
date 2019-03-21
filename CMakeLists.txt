cmake_minimum_required(VERSION 3.13)
project(mips_asm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -pthread")

set(LIB_DIR "lib")

add_library(mips_asm_lib
        include/emitter/emitter.hpp
        include/emitter/op_sequences.hpp
        include/fsm/finite_state_machine.hpp
        include/fsm/transition.hpp
        include/fsm/transition_table.hpp
        include/lexer/lexer_context.hpp
        include/lexer/lexer.hpp
        include/lexer/token.hpp
        include/lexer/token_type.hpp
        include/spec/instruction_defs.hpp
        include/spec/registers.hpp
        src/lexer/lexer.cpp
        src/lexer/token_type.cpp
        src/emitter/op_sequences.cpp
        src/emitter/emitter.cpp
        src/spec/instruction_defs.cpp include/emitter/encode.hpp src/emitter/encode.cpp)
target_include_directories(mips_asm_lib PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(mips_asm
        src/main.cpp)
target_link_libraries(mips_asm mips_asm_lib)
target_include_directories(mips_asm PRIVATE ${CMAKE_SOURCE_DIR}/include)

set(CATCH_INCLUDE_DIR tests/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

add_executable(mips_asm_test tests/emitter.cpp tests/lexer.cpp tests/main.cpp)
target_link_libraries(mips_asm_test mips_asm_lib)
target_include_directories(mips_asm_test INTERFACE ${CATCH_INCLUDE_DIR})
target_include_directories(mips_asm_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(mips_asm_test Catch)

#set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
#add_library(Catch INTERFACE)
#target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
#
#set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp emitter.cpp lexer.cpp)
#add_executable(mips_asm_tests ${TEST_SOURCES})
#target_link_libraries(mips_asm_tests Catch)





#cmake_minimum_required(VERSION 3.2)
#project(LLL)
#
#
##include_directories(${LIB_DIR}/Catch)
##include_directories(${LIB_DIR}/Eigen)
#
#add_library(LLL_LIB src/TestClass.h src/TestClass.cpp)
#
#add_executable(LLL src/main.cpp)
#target_link_libraries(LLL LLL_LIB)
#
#add_executable(LLL_TEST test/test.cpp)
#target_link_libraries(LLL_TEST LLL_LIB)