cmake_minimum_required(VERSION 3.10)
project(semantic)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")

# Source files
set(SRC_FILES
    ../semantic.c
    ../codegen.c
    ../parser.c
    ../lexer.c
    ../symbol_table.c
    ../sym_table_stack.c
    ../ast_tree.c
    ../expressions.c
    ../error.c
)

# Create a library from source files
add_library(semantic_lib STATIC ${SRC_FILES})

# Build target executable
add_executable(semantic ../semantic.c)
target_link_libraries(semantic PRIVATE semantic_lib)

# Add Google Test
add_subdirectory(/home/cigan/googletest googletest-build)

# Source files for the tests
set(TEST_FILES
    ../tests/test_main.cpp
    ../tests/test_exit.cpp
    # Add more test files if needed
)

# Build test executable
add_executable(test_semantic ${TEST_FILES})
target_link_libraries(test_semantic PRIVATE semantic_lib gtest gtest_main)

# Specify include directories
target_include_directories(semantic_lib PUBLIC include)

# Specify include directories for Google Test
target_include_directories(test_semantic PUBLIC ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
