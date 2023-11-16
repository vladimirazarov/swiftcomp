#ifndef SEMANTIC_H 
#define SEMANTIC_H

#include "lexer.h"
#include "semantic.h"
#include "ast_tree.h"
#include <stdbool.h>
#include "symbol_table.h"

typedef struct Context {
    Stack *sym_table_stack;
    // Add other relevant fields like currentFunction, currentLoop, etc.
    Symbol  *current_function;
    bool function_is_being_declared;
    bool in_block;
    bool function_called;
    EvaluatedExpressionData last_return_value;
} Context;

bool check_type_for_func_args(TreeNode *node, ValueType expectedType, Context *context);
// Function prototypes
#endif // SEMANTIC_H 