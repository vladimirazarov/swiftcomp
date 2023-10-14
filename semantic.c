#include "semantic.h"
#include "ast_tree.h"
#include "lexer.h"

//Function to evaluate expression
int evaluate_expression(TreeNode* node) {
    if (node->type == INTEGER_LITERAL) {
        return atoi(node->value);
    } else {
        int left_value = evaluate_expression(node->left);
        int right_value = evaluate_expression(node->right);
        if (node->type == PLUS_OPERATOR) return left_value + right_value;
        if (node->type == MINUS_OPERATOR) return left_value - right_value;
        if (node->type == MULTIPLICATION_OPERATOR) return left_value * right_value;
        if (node->type == DIVISION_OPERATOR) return left_value / right_value;
    }
    return 0;
}