#ifndef SEMANTIC_H 
#define SEMANTIC_H

#include "semantic.h"
#include "ast_tree.h"
#include "lexer.h"
#include "sym_table_stack.h"

typedef struct Context {
    Stack *sym_table_stack
    // Add other relevant fields like currentFunction, currentLoop, etc.
} Context;

// Function prototypes
int evaluate_expression(TreeNode* node);
#endif // SEMANTIC_H 