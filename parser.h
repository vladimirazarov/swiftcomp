#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbol_table.h"
#include "error.h"  // Assuming you have an error.h for error handling
#include <stdbool.h>
#include <stdio.h>  // For printf, remove if not needed

// Structure to represent the parser
typedef struct {
    Token current_token;         // Current token being processed
    SymbolTable* symbol_table;   // Symbol table
} Parser;

// Structure to represent AST
typedef struct TreeNode {
    TokenType type;
    char* value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Function prototypes
Parser* create_parser();
void free_parser(Parser* parser);
void accept_token(Parser* parser, TokenType expected_type);
void advance_token(Parser* parser);
void parse_program(Parser* parser);
void parse_section(Parser* parser);
void parse_statements(Parser* parser);
void parse_type(Parser* parser);
void parse_declaration_or_initialization(Parser* parser);
void parse_block(Parser* parser);
void parse_expression(Parser* parser, Token token);
void parse_function_definitions(Parser* parser);
void parse_assignment_or_function_call(Parser* parser);
void parse_while_loop(Parser* parser);
void parse_if_cond(Parser* parser);
void parse_return_statement(Parser* parser);
void parse_function_call(Parser* parser);
void parse_arguments(Parser *parser);
void parse_argument(Parser *parser);
void parse_literal_or_id(Parser *parser);
void parse_function_definition(Parser *parser);
void parse_parameters(Parser *parser);
void parse_parameter(Parser *parser);
void parse_more_parameters(Parser *parser);
void parse_expression_or_func_call(Parser *parser);
TreeNode* parde_operand(int* index, Token** tokens, int tokenCount);
TreeNode* parse_operator(int* index, Token** tokens, int tokenCount, int minPrecedence);
TreeNode* make_node(TokenType type, char* value);
int get_operator_precedence(Token token);
#endif // PARSER_H