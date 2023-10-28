#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "parser.h"
#include "ast_tree.h"
#include "lexer.h"

// Function prototypes
TreeNode* get_operand(Token token);
TreeNode* parse_expression(Parser *parser, Token token, int min_precedence);
int get_operator_precedence(TokenType token);
#endif