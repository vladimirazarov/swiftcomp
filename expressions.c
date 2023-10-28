#include "expressions.h"
#include "error.h"
#include "parser.h"
#include "lexer.h"
#include "ast_tree.h"

//Function to compare precedence of operators
int get_operator_precedence(TokenType token) {
    if (token == DOUBLE_QMARK_OPERATOR) return 1;
    if (token == PLUS_OPERATOR || token == MINUS_OPERATOR) return 2;
    if (token == ASSIGNMENT_OPERATOR || 
        token == EQUALITY_OPERATOR || 
        token == NOT_EQUAL_OPERATOR || 
        token == LESS_THAN_OPERATOR || 
        token == LESS_THAN_EQUAL_OPERATOR || 
        token == GREATER_THAN_OPERATOR || 
        token == GREATER_THAN_EQUAL_OPERATOR) return 3;
    if (token == MULTIPLICATION_OPERATOR || token == DIVISION_OPERATOR) return 4;
    if (token == EXCLAMATION_MARK) return 5;
    return 0;
}

// Function to get operand for expression
TreeNode* get_operand(Token token) {
    TreeNode* left = make_node(token.type, token.value, 0);
    if(token.type == IDENTIFIER || 
        token.type == INTEGER_LITERAL || 
        token.type == DOUBLE_LITERAL || 
        token.type == STRING_LITERAL) {
        return left;
    }
    handle_error(SYNTACTIC_ERROR);
    return NULL;
}

// Function to parse expression and make a syntactically correct AST
TreeNode* parse_expression(Parser *parser, Token token, int min_precedence) {
    // Handle the operand or parenthesis to get the left-most element
    TreeNode* expression_tree;
    if (parser->current_token.type == LEFT_PARENTHESIS) {
        advance_token(parser);
        expression_tree = parse_expression(parser, parser->current_token, 0);
        if (parser->current_token.type != RIGHT_PARENTHESIS) {
            handle_error(SYNTACTIC_ERROR);
        }
        advance_token(parser);
    } else {
        if(parser->current_token.value != token.value){
            expression_tree = get_operand(token);
        } else {
            expression_tree = get_operand(parser->current_token);
            advance_token(parser);
        }
    }

    // Process operators
     while(parser->current_token.type == MINUS_OPERATOR ||
          parser->current_token.type == PLUS_OPERATOR ||
          parser->current_token.type == EQUALITY_OPERATOR || 
          parser->current_token.type == NOT_EQUAL_OPERATOR || 
          parser->current_token.type == LESS_THAN_OPERATOR || 
          parser->current_token.type == LESS_THAN_EQUAL_OPERATOR ||
          parser->current_token.type == GREATER_THAN_OPERATOR || 
          parser->current_token.type == GREATER_THAN_EQUAL_OPERATOR ||
          parser->current_token.type == MULTIPLICATION_OPERATOR ||
          parser->current_token.type == DIVISION_OPERATOR ||
          parser->current_token.type == LEFT_PARENTHESIS ||
          parser->current_token.type == EXCLAMATION_MARK ||
          parser->current_token.type == DOUBLE_QMARK_OPERATOR) {
        int precedence = get_operator_precedence(parser->current_token.type);
        if (precedence < min_precedence) break;

        if(precedence >= min_precedence && precedence) {
            TreeNode* new_node = make_node(parser->current_token.type, parser->current_token.value, 0);
            add_child(new_node, expression_tree); // Add the current expression_tree as a child
            advance_token(parser);
            
            // Unary operator
            if(parser->current_token.type == EXCLAMATION_MARK) {
                expression_tree = new_node;
            } 
            // Binary operator
            else {
                TreeNode* right = parse_expression(parser, parser->current_token, precedence + 1);
                add_child(new_node, right); // Add the right-hand side as another child
                expression_tree = new_node;
            }
        } else {
            handle_error(SYNTACTIC_ERROR);
        }
    }
    return expression_tree;
}