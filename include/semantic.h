/**
 * @file semantic.h
 * @brief Header file for semantic analysis functions.
 *
 * This header file declares the functions and data structures used for
 * semantic analysis in the compiler, ensuring the correct meaning and usage
 * of language constructs.
 * 
 * @author Vladimir Azarov
 */

#ifndef SEMANTIC_H 
#define SEMANTIC_H

#include "lexer.h"
#include "semantic.h"
#include "ast_tree.h"
#include <stdbool.h>
#include "symbol_table.h"

typedef struct Context {
    Stack *sym_table_stack;
    Symbol  *current_function;
    bool function_is_being_declared;
    bool in_block;
    bool function_called;
    EvaluatedExpressionData last_return_value;
} Context;

bool check_type_for_func_args(TreeNode *node, ValueType expectedType, Context *context);
char *convert_double_char_to_hex_str(const char *valueStr);
char* convert_double_num_to_hex_string(double value);
// Function prototypes
#endif // SEMANTIC_H 