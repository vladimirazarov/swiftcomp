#include "expressions.h"
#include "error.h"
#include "parser.h"
#include "lexer.h"
#include "ast_tree.h"
#include "semantic.h"

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
        token.type == STRING_LITERAL || 
        token.type == NIL_LITERAL ) {
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

// Before the while loop, immediately after obtaining expression_tree
if (parser->current_token.type == EXCLAMATION_MARK) {
    TreeNode *new_node = make_node(EXCLAMATION_MARK, NULL, 1);
    add_child(new_node, expression_tree);  // Add the current expression_tree as a child
    expression_tree = new_node;
    advance_token(parser);
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
          parser->current_token.type == DOUBLE_QMARK_OPERATOR) {
        int precedence = get_operator_precedence(parser->current_token.type);
        if (precedence < min_precedence) break;

        if(precedence >= min_precedence && precedence) {
            TreeNode* new_node = make_node(parser->current_token.type, parser->current_token.value, 0);
            add_child(new_node, expression_tree); // Add the current expression_tree as a child
            advance_token(parser);
            
                TreeNode* right = parse_expression(parser, parser->current_token, precedence + 1);
                add_child(new_node, right); // Add the right-hand side as another child
                expression_tree = new_node;
        } else {
            handle_error(SYNTACTIC_ERROR);
        }
    }
    return expression_tree;
}

// Function to check if a token can start an expression
bool is_expression_start_token(Token token)
{
    // Include other token types that can start an expression
    return (token.type == INTEGER_LITERAL) || 
           (token.type == DOUBLE_LITERAL) || 
           (token.type == STRING_LITERAL) || 
           (token.type == IDENTIFIER && look_ahead(1).type != LEFT_PARENTHESIS)  ||
           (token.type == IDENTIFIER && look_ahead(1).type != ASSIGNMENT_OPERATOR);
}

EvaluatedExpressionData evaluate_expression(TreeNode *node, Context *context, bool must_be_truth_value) {
    EvaluatedExpressionData data;
    if (node == NULL) {
        data.type = EMPTY;
        return data;
    }

    // Base cases for literals
    if (node->type == INTEGER_LITERAL) {
        data.value.int_value = atoi(node->value);
        data.type = INT_SYMBOL_TYPE;
        return data;
    } else if (node->type == DOUBLE_LITERAL) {
        data.value.double_value = atof(node->value);
        data.type = DOUBLE_SYMBOL_TYPE;
        return data;
    } else if (node->type == STRING_LITERAL) {
        // Assuming string handling is correctly implemented elsewhere
        data.value.str_value = strdup(node->value);
        data.type = STRING_SYMBOL_TYPE;
        return data;
    }
    else if (node->type == NIL_LITERAL) {
        data.type = NIL_SYMBOL_TYPE;
        return data;
    }
    else if (node->type ==  AST_IDENTIFIER) {
        Symbol *symbol = find_symbol(context->sym_table_stack->top, node->value); 
        if (symbol == NULL)
            handle_error(SEMANTIC_UNDEF_ERROR);
        data = symbol->type_and_value;
        return data;
    }

    // Recursive evaluation of children
    EvaluatedExpressionData left_eval = evaluate_expression(node->children[0], context, must_be_truth_value);
    EvaluatedExpressionData right_eval = evaluate_expression(node->children[1], context, must_be_truth_value);

    if (right_eval.type != EMPTY && left_eval.type != EMPTY)
    {
    // Perform operation according to the type of the node
    switch(node->type) {
        case PLUS_OPERATOR:
            // Addition for integers, doubles, and strings (concatenation)
            if (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) {
                data.value.int_value = left_eval.value.int_value + right_eval.value.int_value;
                data.type = INT_SYMBOL_TYPE;
            } else if (left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE) {
                data.value.double_value = left_eval.value.double_value + right_eval.value.double_value;
                data.type = DOUBLE_SYMBOL_TYPE;
            } else if (left_eval.type == STRING_SYMBOL_TYPE && right_eval.type == STRING_SYMBOL_TYPE) {
                // String concatenation logic here
            } else {
                // Handle type incompatibility error
            }
            break;
        case MINUS_OPERATOR:
            // Subtraction for integers and doubles
            if (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) {
                data.value.int_value = left_eval.value.int_value - right_eval.value.int_value;
                data.type = INT_SYMBOL_TYPE;
            } else if (left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE) {
                data.value.double_value = left_eval.value.double_value - right_eval.value.double_value;
                data.type = DOUBLE_SYMBOL_TYPE;
            } else {
                // Handle type incompatibility error
            }
            break;
        case MULTIPLICATION_OPERATOR:
            // Multiplication for integers and doubles
            if (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) {
                data.value.int_value = left_eval.value.int_value * right_eval.value.int_value;
                data.type = INT_SYMBOL_TYPE;
            } else if (left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE) {
                data.value.double_value = left_eval.value.double_value * right_eval.value.double_value;
                data.type = DOUBLE_SYMBOL_TYPE;
            } else {
                // Handle type incompatibility error
            }
            break;
        case DIVISION_OPERATOR:
            // Division for integers and doubles, handle division by zero
            if (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) {
                if (right_eval.value.int_value == 0) {
                    // Handle division by zero error
                } else {
                    data.value.int_value = left_eval.value.int_value / right_eval.value.int_value;
                    data.type = INT_SYMBOL_TYPE;
                }
            } else if (left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE) {
                if (right_eval.value.double_value == 0.0) {
                    // Handle division by zero error
                } else {
                    data.value.double_value = left_eval.value.double_value / right_eval.value.double_value;
                    data.type = DOUBLE_SYMBOL_TYPE;
                }
            } else {
                // Handle type incompatibility error
            }
            break;
        // ... Implement logic for additional operators, including relational and ?? operator
        // Make sure to handle must_be_truth_value where necessary
        default:
            // Handle unknown or unsupported node type error
            break;
            //Implement the logic for string concatenation.
            //Implement the logic for the ?? operator and the unary ! operator.
            //Add cases for relational operators and ensure type compatibility.
            //Handle division by zero and other potential errors.
            //Perform implicit type conversions where necessary, as per the requirements.
            //Handle the must_be_truth_value flag to ensure the expression is valid in a truth context when required.
            //Add proper error handling for semantic errors.
    }
    return data;
    }

    // Here we need to check if the expression should evaluate to a truth value and handle it accordingly
    if (must_be_truth_value && data.type != BOOL_SYMBOL_TYPE) {
        // Handle the error that the result is not a truth value when it is expected to be
    }
    if (right_eval.type == EMPTY && left_eval.type != EMPTY)
        return left_eval;
    if (right_eval.type != EMPTY && left_eval.type == EMPTY)
        return right_eval;
}