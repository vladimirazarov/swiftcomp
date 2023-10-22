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

//Function to get operand for expression
TreeNode* get_operand(Token token, TreeNode* left) {

    if(token.type == IDENTIFIER || 
        token.type == INTEGER_LITERAL || 
        token.type == DOUBLE_LITERAL || 
        token.type == STRING_LITERAL){
        left->value = token.value;
        left->type = token.type;
        return left;
    }
    handle_error(SYNTACTIC_ERROR);
    return NULL;
}

//Function to parse expression and make a syntactically correct ast
TreeNode* parse_expression(Parser *parser, Token token, int min_precedence)
{
    //Allocate memory for ast
    TreeNode* expression_tree = (TreeNode*)malloc(sizeof(TreeNode));
    expression_tree->left = NULL;
    expression_tree->right = NULL;

    //Handle parenthesis
    if (parser->current_token.type == LEFT_PARENTHESIS) {
        advance_token(parser);
        expression_tree = parse_expression(parser, parser->current_token, 0);
        if (parser->current_token.type != RIGHT_PARENTHESIS) {
            handle_error(SYNTACTIC_ERROR);
        }
        advance_token(parser);
    } 
    //Get first operand of expression
    else {
        if(parser->current_token.value != token.value){
            expression_tree = get_operand(token, expression_tree);
        } else {
            expression_tree = get_operand(parser->current_token, expression_tree);
            advance_token(parser);
        }
    }
    
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

        if(precedence < min_precedence){
            break;
        } 
        //Get operator and second operand of expression
        if(precedence >= min_precedence && precedence){
            //Handle unary operators
            if(parser->current_token.type == EXCLAMATION_MARK){
                TreeNode* new_left = make_node(parser->current_token.type, parser->current_token.value);
                advance_token(parser);
                new_left->right = NULL;
                new_left->left = expression_tree;
                expression_tree = new_left;
            } else{
                TreeNode* new_left = make_node(parser->current_token.type, parser->current_token.value);
                advance_token(parser);
                TreeNode* right = parse_expression(parser, parser->current_token, precedence + 1);
                new_left->left = expression_tree;
                new_left->right = right;
                expression_tree = new_left;
            }
        } else {
            handle_error(SYNTACTIC_ERROR);
        }
    }
    return expression_tree;
}