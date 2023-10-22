#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbol_table.h"
#include "ast_tree.h"
#include "error.h"  // Assuming you have an error.h for error handling
#include <stdbool.h>
#include <stdio.h>  // For printf, remove if not needed

// Structure to represent the parser
typedef struct {
    Token current_token;         // Current token being processed
    SymbolTable* symbol_table;   // Symbol table
} Parser;

// Function prototypes
Parser* create_parser();
void free_parser(Parser* parser);
void accept_token(Parser* parser, TokenType expected_type);
void advance_token(Parser* parser);
TreeNode *parse_program(Parser* parser);
TreeNode *parse_section(Parser* parser);
TreeNode *parse_statements(Parser* parser);
TreeNode *parse_type(Parser* parser);
TreeNode *parse_declaration_or_initialization(Parser* parser);
TreeNode *parse_block(Parser* parser);
TreeNode *parse_function_definitions(Parser* parser);
TreeNode *parse_assignment_or_function_call(Parser* parser);
TreeNode *parse_while_loop(Parser* parser);
TreeNode *parse_if_cond(Parser* parser);
TreeNode *parse_return_statement(Parser* parser);
TreeNode *parse_function_call(Parser* parser, ASTNodeType);
TreeNode *parse_arguments(Parser *parser);
TreeNode *parse_argument(Parser *parser);
TreeNode *parse_literal_or_id(Parser *parser);
TreeNode *parse_function_definition(Parser *parser);
TreeNode *parse_parameters(Parser *parser);
TreeNode *parse_parameter(Parser *parser);
TreeNode *parse_more_parameters(Parser *parser);
TreeNode *parse_expression_or_func_call(Parser *parser, ASTNodeType);
TreeNode *get_expression_ast(Parser *parser, Token token);
#endif // PARSER_H