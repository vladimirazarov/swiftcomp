#include "error.h"
#include <stdio.h>

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