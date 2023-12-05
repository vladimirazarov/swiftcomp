#include "gtest/gtest.h"
#include <iostream>
#include <cstdio>

// Function to run the Swift compiler and return its exit code
int runSemantic(const std::string &input) {
    FILE *pipe = popen("./semantic", "w");
    if (!pipe) {
        std::cerr << "popen() failed!" << std::endl;
        return -1;  // Return an error code
    }

    // Write input to the standard input of the process
    fprintf(pipe, "%s", input.c_str());
    fflush(pipe);

    // Close the pipe
    int exitCode = pclose(pipe);

    return exitCode;
}



TEST(LexErrorTest, CheckExitCode) {
    // Provide input to stdin
    const std::string input = "\"unterminated";
    
    // Run the semantic program and capture the exit code
    int exitCode = runSemantic(input);

    // Print the program exit code for debugging purposes
    std::cout << "Program Exit Code: " << exitCode << std::endl;

    // Check if exit(3) is called (exit code 3 indicates a semantic error)
    ASSERT_EQ(WEXITSTATUS(exitCode), 1);
}


// Test case for semantic error: undeclared variable
TEST(SemanticErrorTest, UndeclaredVariable) {
    const std::string input = "let result: Int = x + 5";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: type mismatch in assignment
TEST(SemanticErrorTest, TypeMismatchAssignment) {
    const std::string input = "let number: String = 42";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: undefined function
TEST(SemanticErrorTest, UndefinedFunction) {
    const std::string input = "callUndefinedFunction()";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: incompatible types in binary operation
TEST(SemanticErrorTest, IncompatibleTypesBinaryOperation) {
    const std::string input = "let result: Int = true + 5";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: cyclic dependency in variable declaration
TEST(SemanticErrorTest, CyclicDependency) {
    const std::string input = "let x: Int = y + 1; let y: Int = x + 1";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: redeclaration of variable
TEST(SemanticErrorTest, RedeclarationVariable) {
    const std::string input = "let x: Int = 5; let x: Bool = true";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: redeclaration of function
TEST(SemanticErrorTest, RedeclarationFunction) {
    const std::string input = "func foo() {} func foo() {}";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: invalid return type
TEST(SemanticErrorTest, InvalidReturnType) {
    const std::string input = "func add(a: Int, b: Int) -> Bool { return a + b; }";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

// Test case for semantic error: missing return statement in non-void function
TEST(SemanticErrorTest, MissingReturnStatement) {
    const std::string input = "func getResult() -> Int {}";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

TEST(SemanticErrorTest, ReturnOutsideFunction) {
    const std::string input = "return 42;";
    int exitCode = runSemantic(input);
    ASSERT_GT(exitCode, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
