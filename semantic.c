#include "semantic.h"
#include "ast_tree.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h" // Assuming parser.h contains the definition of Parser and TreeNode
#include "symbol_table.h"

const char *get_node_type_name(int type)
{
    switch (type)
    {
    case AST_IDENTIFIER:
        return "AST_IDENTIFIER";
    case AST_LEFT_PARENTHESIS:
        return "AST_LEFT_PARENTHESIS";
    case AST_RIGHT_PARENTHESIS:
        return "AST_RIGHT_PARENTHESIS";
    case AST_LEFT_CURLY_BRACE:
        return "AST_LEFT_CURLY_BRACE";
    case AST_RIGHT_CURLY_BRACE:
        return "AST_RIGHT_CURLY_BRACE";
    case AST_PLUS_OPERATOR:
        return "AST_PLUS_OPERATOR";
    case AST_MINUS_OPERATOR:
        return "AST_MINUS_OPERATOR";
    case AST_MULTIPLICATION_OPERATOR:
        return "AST_MULTIPLICATION_OPERATOR";
    case AST_DIVISION_OPERATOR:
        return "AST_DIVISION_OPERATOR";
    case AST_LESS_THAN_OPERATOR:
        return "AST_LESS_THAN_OPERATOR";
    case AST_GREATER_THAN_OPERATOR:
        return "AST_GREATER_THAN_OPERATOR";
    case AST_LESS_THAN_EQUAL_OPERATOR:
        return "AST_LESS_THAN_EQUAL_OPERATOR";
    case AST_GREATER_THAN_EQUAL_OPERATOR:
        return "AST_GREATER_THAN_EQUAL_OPERATOR";
    case AST_SEMICOLON:
        return "AST_SEMICOLON";
    case AST_ASSIGNMENT_OPERATOR:
        return "AST_ASSIGNMENT_OPERATOR";
    case AST_EQUALITY_OPERATOR:
        return "AST_EQUALITY_OPERATOR";
    case AST_NOT_EQUAL_OPERATOR:
        return "AST_NOT_EQUAL_OPERATOR";
    case AST_COMMA:
        return "AST_COMMA";
    case AST_INTEGER_LITERAL:
        return "AST_INTEGER_LITERAL";
    case AST_STRING_LITERAL:
        return "AST_STRING_LITERAL";
    case AST_MULTILINE_STRING:
        return "AST_MULTILINE_STRING";
    case AST_DOUBLE_LITERAL:
        return "AST_DOUBLE_LITERAL";
    case AST_NIL_LITERAL:
        return "AST_NIL_LITERAL";
    case AST_COLON:
        return "AST_COLON";
    case AST_RETURN_KEYWORD:
        return "AST_RETURN_KEYWORD";
    case AST_ELSE_KEYWORD:
        return "AST_ELSE_KEYWORD";
    case AST_FUNCTION_KEYWORD:
        return "AST_FUNCTION_KEYWORD";
    case AST_IF_KEYWORD:
        return "AST_IF_KEYWORD";
    case AST_WHILE_KEYWORD:
        return "AST_WHILE_KEYWORD";
    case AST_VAR_KEYWORD:
        return "AST_VAR_KEYWORD";
    case AST_LET_KEYWORD:
        return "AST_LET_KEYWORD";
    case AST_ARROW:
        return "AST_ARROW";
    case AST_END_OF_FILE:
        return "AST_END_OF_FILE";
    case AST_STRING_KEYWORD:
        return "AST_STRING_KEYWORD";
    case AST_STRING_NULLABLE_KEYWORD:
        return "AST_STRING_NULLABLE_KEYWORD";
    case AST_INT_KEYWORD:
        return "AST_INT_KEYWORD";
    case AST_INT_NULLABLE_KEYWORD:
        return "AST_INT_NULLABLE_KEYWORD";
    case AST_DOUBLE_KEYWORD:
        return "AST_DOUBLE_KEYWORD";
    case AST_DOUBLE_NULLABLE_KEYWORD:
        return "AST_DOUBLE_NULLABLE_KEYWORD";
    case AST_UNDERSCORE:
        return "AST_UNDERSCORE";
    case AST_PROGRAM:
        return "AST_PROGRAM";
    case AST_SECTION:
        return "AST_SECTION";
    case AST_STATEMENTS:
        return "AST_STATEMENTS";
    case AST_STATEMENT:
        return "AST_STATEMENT";
    case AST_DECLARATION:
        return "AST_DECLARATION";
    case AST_DECLARATION_INITIALIZATION:
        return "AST_DECLARATION_INITIALIZATION";
    case AST_OPTIONAL_TYPE:
        return "AST_OPTIONAL_TYPE";
    case AST_ASSIGNMENT:
        return "AST_ASSIGNMENT";
    case AST_CONDITIONAL:
        return "AST_CONDITIONAL";
    case AST_LOOP:
        return "AST_LOOP";
    case AST_FUNCTION_CALL:
        return "AST_FUNCTION_CALL";
    case AST_RETURN_STATEMENT:
        return "AST_RETURN_STATEMENT";
    case AST_FUNCTION_DEFINITIONS:
        return "AST_FUNCTION_DEFINITIONS";
    case AST_FUNCTION_DEFINITION:
        return "AST_FUNCTION_DEFINITION";
    case AST_OPTIONAL_RETURN_TYPE:
        return "AST_OPTIONAL_RETURN_TYPE";
    case AST_PARAMETERS:
        return "AST_PARAMETERS";
    case AST_PARAMETER:
        return "AST_PARAMETER";
    case AST_NAME:
        return "AST_NAME";
    case AST_MORE_PARAMETERS:
        return "AST_MORE_PARAMETERS";
    case AST_OPTIONAL_VAR_DECLARATION:
        return "AST_OPTIONAL_VAR_DECLARATION";
    case AST_ARGUMENTS:
        return "AST_ARGUMENTS";
    case AST_ARGUMENT:
        return "AST_ARGUMENT";
    case AST_MORE_ARGUMENTS:
        return "AST_MORE_ARGUMENTS";
    case AST_EXPRESSION_OR_ID:
        return "AST_EXPRESSION_OR_ID";
    case AST_ELSE_PART:
        return "AST_ELSE_PART";
    case AST_BLOCK:
        return "AST_BLOCK";
    case AST_PARAMETER_NAME:
        return "AST_PARAMETER_NAME";
    case AST_EXTERNAL_NAME:
        return "AST_EXTERNAL_NAME";
    case AST_INTERNAL_NAME:
        return "AST_INTERNAL_NAME";
    case AST_INITIALIZATION:
        return "AST_INITIALIZATION";
    case AST_EXPRESSION:
        return "AST_EXPRESSION";
    case AST_FUNCTION_CALL_IN_ASSIGNMENT:
        return "AST_FUNCTION_CALL_IN_ASSIGNMENT";
    case EXCLAMATION_MARK:
        return "EXCLAMATION MARK";
    case AST_MORE_SECTIONS:
        return "MORE SECTIONS";
    default:
        return "UNKNOWN";
    }
}

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case INTEGER_LITERAL:
            return "int";
        case STRING_LITERAL:
        case MULTILINE_STRING:
            return "string";
        case DOUBLE_LITERAL:
            return "double";
        case NIL_LITERAL:
            return "nil";
        default:
            return "unknown";
    }
}

// int evaluate_expression(TreeNode *node)
//{
// if (node->type == INTEGER_LITERAL)
//{
// return atoi(node->value);
//}
// else
//{
// int left_value = evaluate_expression(node->left);
// int right_value = evaluate_expression(node->right);
// if (node->type == PLUS_OPERATOR)
// return left_value + right_value;
// if (node->type == MINUS_OPERATOR)
// return left_value - right_value;
// if (node->type == MULTIPLICATION_OPERATOR)
// return left_value * right_value;
// if (node->type == DIVISION_OPERATOR)
// return left_value / right_value;
//}
// return 0;
//}

// Function to print the AST in a readable format
void print_ast(TreeNode *root, int indent_level)
{
    if (root == NULL)
        return;

    // Indentation for readability
    for (int i = 0; i < indent_level; ++i)
    {
        printf("  ");
    }

    // Print the current node's type name
    const char *node_type_name = get_node_type_name(root->type);
    printf("%s", node_type_name);

    // Print the node value if it exists
    if (root->value != NULL)
    {
        printf(" (Value: %s)", root->value);
    }
    printf("\n");

    // Recursive DFS traversal for each child
    for (size_t i = 0; i < root->children_count; ++i)
    {
        print_ast(root->children[i], indent_level + 1);
    }
}


void semantic_analysis(TreeNode *root, SymbolTable *table)
{
    if (root == NULL)
        return;

    switch (root->type)
    {
    case AST_INITIALIZATION:
    case AST_DECLARATION:
    case AST_ASSIGNMENT:
        // Implement assignment checks
        break;
    case AST_FUNCTION_CALL:
        // Implement function call checks
        break;
    case AST_CONDITIONAL:
        // Implement if statement checks
        break;
    case AST_RETURN_STATEMENT:
        // Implement return statement checks
        break;
    case AST_ARGUMENTS:
        // Implement arguments checks
        break;
    case AST_ARGUMENT:
        // Implement argument checks
        break;
    case AST_FUNCTION_DEFINITION:
        break;
    case AST_PARAMETERS:
        // Implement parameters checks
        break;
    case AST_PARAMETER:
        // Implement single parameter checks
        break;
    case AST_IDENTIFIER:
        // Implement identifier checks
        break;
    case AST_ELSE_PART:
        // Implement else part checks
        break;
    case AST_BLOCK:
        // Implement block checks
        break;
    case AST_LOOP:
        break;
    case AST_EXPRESSION:
        // Implement expression checks
        break;

    default:
        // Unknown node type
        break;
    }

    // Recursive DFS traversal for each child
    for (size_t i = 0; i < root->children_count; ++i)
    {
        semantic_analysis(root->children[i], table);
    }
}

int main()
{
    Parser *parser = create_parser();
    TreeNode *program = parse_program(parser);
    free_parser(parser);

    print_ast(program, 0);
    SymbolTable *table;
    // `buffer` is where we'll store the generated code.
    // CodeBuffer *buffer;

    // First traversal for semantic analysis
    //semantic_analysis(program, table);

    // Second traversal for code generation
    // code_generation(root, buffer);

    return 0;
}