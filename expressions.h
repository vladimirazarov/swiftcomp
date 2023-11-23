#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "parser.h"
#include "ast_tree.h"
#include "lexer.h"
#include "symbol_table.h"
#include "string.h"
#include "semantic.h"

// Function prototypes
TreeNode* get_operand(Token token);
TreeNode* parse_expression(Parser *parser, Token token, int min_precedence);
int get_operator_precedence(TokenType token);
bool is_expression_start_token(Token token);

EvaluatedExpressionData perform_relational_operation(TreeNode *expr, EvaluatedExpressionData left, EvaluatedExpressionData right);
EvaluatedExpressionData perform_arithmetic_operation(TreeNode *expr, EvaluatedExpressionData left, EvaluatedExpressionData right);
EvaluatedExpressionData evaluate_identifier(TreeNode *node, Context *context);
EvaluatedExpressionData evaluate_literal(TreeNode *node);
EvaluatedExpressionData evaluate_expression(TreeNode *node, Context *context, bool must_be_truth_value);
bool is_type_compatible(ValueType left, ValueType right);
ValueType get_result_type(ValueType left, ValueType right);
generate_code_for_expression(const char *var_to_save, TreeNode *node, Context *context);




#endif