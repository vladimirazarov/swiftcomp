/**
 * @file error.h
 * @brief Header file for error handling functions.
 *
 * This header file contains declarations for the error handling functions
 * used across different stages of the compilation process, including error
 * logging and management.
 * 
 * @author Vladimir Azarov
 */

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
void handle_error(ErrorCode code);

#endif // ERROR_H