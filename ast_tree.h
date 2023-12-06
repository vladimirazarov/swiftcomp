/**
 * @file ast_tree.h
 * @brief Header file for Abstract Syntax Tree (AST) functions.
 *
 * This header file contains the declarations of the functions and data
 * structures used for the Abstract Syntax Tree (AST) utilized in parsing
 * and representing source code.
 * 
 * @author Vladimir Azarov
 */

#ifndef AST_TREE_H
#define AST_TREE_H
#include "lexer.h"
#include "stdlib.h"

typedef enum {
    AST_IDENTIFIER = IDENTIFIER,
    AST_LEFT_PARENTHESIS,        // (
    AST_RIGHT_PARENTHESIS,       // )
    AST_LEFT_CURLY_BRACE,        // {
    AST_RIGHT_CURLY_BRACE,       // }
    AST_PLUS_OPERATOR,           // +
    AST_MINUS_OPERATOR,          // -
    AST_MULTIPLICATION_OPERATOR, // *
    AST_DIVISION_OPERATOR,       // /
    AST_LESS_THAN_OPERATOR,      // <
    AST_GREATER_THAN_OPERATOR,   // >
    AST_LESS_THAN_EQUAL_OPERATOR,         // <=
    AST_GREATER_THAN_EQUAL_OPERATOR,      // >=
    AST_SEMICOLON,               // ;
    AST_ASSIGNMENT_OPERATOR,     // =
    AST_EQUALITY_OPERATOR,       // ==
    AST_NOT_EQUAL_OPERATOR,      // !=
    AST_COMMA,                   // ,
    AST_INTEGER_LITERAL,         // 56
    AST_STRING_LITERAL,          // "abc"
    AST_MULTILINE_STRING,        // """foobar"""
    AST_DOUBLE_LITERAL,          // 13.56/E/e
    AST_NIL_LITERAL,             // nil
    AST_COLON,                   // :
    AST_RETURN_KEYWORD,          // return
    AST_ELSE_KEYWORD,            // else
    AST_FUNCTION_KEYWORD,        // func
    AST_IF_KEYWORD,              // if
    AST_WHILE_KEYWORD,           // while
    AST_VAR_KEYWORD,             // var keyword
    AST_LET_KEYWORD,             // let keyword
    AST_ARROW,          // ->
    AST_END_OF_FILE,              // EOF
    AST_STRING_KEYWORD,
    AST_STRING_NULLABLE_KEYWORD,
    AST_INT_KEYWORD,
    AST_INT_NULLABLE_KEYWORD,
    AST_DOUBLE_KEYWORD,
    AST_DOUBLE_NULLABLE_KEYWORD,
    AST_UNDERSCORE,
    AST_PROGRAM,
    AST_SECTION,
    AST_STATEMENTS,
    AST_STATEMENT,
    AST_DECLARATION,
    AST_DECLARATION_INITIALIZATION,
    AST_OPTIONAL_TYPE,
    AST_ASSIGNMENT,
    AST_CONDITIONAL,
    AST_LOOP,
    AST_FUNCTION_CALL,
    AST_RETURN_STATEMENT,
    AST_FUNCTION_DEFINITIONS,
    AST_FUNCTION_DEFINITION,
    AST_OPTIONAL_RETURN_TYPE,
    AST_PARAMETERS,
    AST_PARAMETER,
    AST_NAME,
    AST_MORE_PARAMETERS,
    AST_OPTIONAL_VAR_DECLARATION,
    AST_ARGUMENTS,
    AST_ARGUMENT,
    AST_MORE_ARGUMENTS,
    AST_EXPRESSION_OR_ID,
    AST_ELSE_PART,
    AST_BLOCK,
    AST_TYPE,
    AST_PARAMETER_NAME,
    AST_EXTERNAL_NAME,
    AST_INTERNAL_NAME,
    AST_INITIALIZATION,
    AST_EXPRESSION,
    AST_FUNCTION_CALL_IN_ASSIGNMENT,
    AST_MORE_SECTIONS
} ASTNodeType;

// Structure to represent AST
typedef struct TreeNode {
    ASTNodeType type;
    char *value;
    struct TreeNode **children; // Array for children nodes
    size_t children_count; // Number of children in the array
} TreeNode;

// Function prototypes
TreeNode* make_node(ASTNodeType type, char* value, int initial_child_capacity);
void print_ast(TreeNode *root, int indent_level);
#endif