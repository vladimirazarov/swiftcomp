#ifndef SEMANTIC_H 
#define SEMANTIC_H

#include "semantic.h"
#include "ast_tree.h"
#include "lexer.h"
#include "symbol_table.h"

// Function prototypes
int evaluate_expression(TreeNode* node);
void check_return_statements(TreeNode *node, SymbolTable *table, const char* expectedReturnType);
const char* infer_expression_type(TreeNode *exprNode, SymbolTable *table);
#endif // SEMANTIC_H 