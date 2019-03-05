cmake_minimum_required(VERSION 3.13)

project(mips_asm)
add_subdirectory(src)
target_include_directories(mips_asm PRIVATE ${CMAKE_SOURCE_DIR}/include)

project(mips_asm_tests)
add_subdirectory(tests)
