#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

// Enum to represent different types of errors
typedef enum {
    LEXICAL_ERROR = 1,          // Error during lexical analysis
    SYNTACTIC_ERROR,            // Error during syntactic analysis
    SEMANTIC_UNDEF_ERROR,       // Undefined function/variable, or redefinition
    SEMANTIC_PARAM_TYPE_ERROR,  // Incorrect number/type of function parameters or return type
    SEMANTIC_VAR_ERROR,         // Use of undefined/uninitialized variable
    SEMANTIC_RETURN_ERROR,      // Missing/excess expression in return statement
    SEMANTIC_TYPE_COMPAT_ERROR, // Type compatibility error in expressions
    SEMANTIC_TYPE_DERIV_ERROR,  // Error of type derivation
    SEMANTIC_OTHER_ERROR,       // Other semantic errors
    INTERNAL_ERROR = 99         // Internal compiler error
} ErrorCode;

// Function to handle errors
void handle_error(ErrorCode code) {
    switch (code) {
        case LEXICAL_ERROR:
            fprintf(stderr, "Error: Incorrect structure of the current lexeme.\n");
            break;
        case SYNTACTIC_ERROR:
            fprintf(stderr, "Error: Incorrect syntax of the program.\n");
            break;
        case SEMANTIC_UNDEF_ERROR:
            fprintf(stderr, "Error: Undefined function/variable, or redefinition.\n");
            break;
        case SEMANTIC_PARAM_TYPE_ERROR:
            fprintf(stderr, "Error: Incorrect number/type of function parameters or return type.\n");
            break;
        case SEMANTIC_VAR_ERROR:
            fprintf(stderr, "Error: Use of undefined/uninitialized variable.\n");
            break;
        case SEMANTIC_RETURN_ERROR:
            fprintf(stderr, "Error: Missing/excess expression in return statement.\n");
            break;
        case SEMANTIC_TYPE_COMPAT_ERROR:
            fprintf(stderr, "Error: Type compatibility error in expressions.\n");
            break;
        case SEMANTIC_TYPE_DERIV_ERROR:
            fprintf(stderr, "Error: Error of type derivation.\n");
            break;
        case SEMANTIC_OTHER_ERROR:
            fprintf(stderr, "Error: Other semantic errors.\n");
            break;
        case INTERNAL_ERROR:
            fprintf(stderr, "Error: Internal compiler error.\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown error.\n");
            break;
    }
    exit(code);
}

#endif // ERROR_H