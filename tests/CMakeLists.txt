project(mips_asm_tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(mips_asm_tests ${TEST_SOURCES})
target_link_libraries(mips_asm_tests Catch)