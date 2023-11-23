#include "expressions.h"
#include "error.h"
#include "parser.h"
#include "lexer.h"
#include "ast_tree.h"
#include "semantic.h"
#include "codegen.h"

// Function to compare precedence of operators
int get_operator_precedence(TokenType token)
{
    if (token == DOUBLE_QMARK_OPERATOR)
        return 1;
    if (token == PLUS_OPERATOR || token == MINUS_OPERATOR)
        return 2;
    if (token == ASSIGNMENT_OPERATOR ||
        token == EQUALITY_OPERATOR ||
        token == NOT_EQUAL_OPERATOR ||
        token == LESS_THAN_OPERATOR ||
        token == LESS_THAN_EQUAL_OPERATOR ||
        token == GREATER_THAN_OPERATOR ||
        token == GREATER_THAN_EQUAL_OPERATOR)
        return 3;
    if (token == MULTIPLICATION_OPERATOR || token == DIVISION_OPERATOR)
        return 4;
    if (token == EXCLAMATION_MARK)
        return 5;
    return 0;
}

// Function to get operand for expression
TreeNode *get_operand(Token token)
{
    TreeNode *left = make_node(token.type, token.value, 0);
    if (token.type == IDENTIFIER ||
        token.type == INTEGER_LITERAL ||
        token.type == DOUBLE_LITERAL ||
        token.type == STRING_LITERAL ||
        token.type == NIL_LITERAL)
    {
        return left;
    }
    handle_error(SYNTACTIC_ERROR);
    return NULL;
}

// Function to parse expression and make a syntactically correct AST
TreeNode *parse_expression(Parser *parser, Token token, int min_precedence)
{
    // Handle the operand or parenthesis to get the left-most element
    TreeNode *expression_tree;
    if (parser->current_token.type == LEFT_PARENTHESIS)
    {
        advance_token(parser);
        expression_tree = parse_expression(parser, parser->current_token, 0);
        if (parser->current_token.type != RIGHT_PARENTHESIS)
        {
            handle_error(SYNTACTIC_ERROR);
        }
        advance_token(parser);
    }
    else
    {
        if (parser->current_token.value != token.value)
        {
            expression_tree = get_operand(token);
        }
        else
        {
            expression_tree = get_operand(parser->current_token);
            advance_token(parser);
        }
    }

    // Before the while loop, immediately after obtaining expression_tree
    if (parser->current_token.type == EXCLAMATION_MARK)
    {
        TreeNode *new_node = make_node(EXCLAMATION_MARK, NULL, 1);
        add_child(new_node, expression_tree); // Add the current expression_tree as a child
        expression_tree = new_node;
        advance_token(parser);
    }

    // Process operators
    while (parser->current_token.type == MINUS_OPERATOR ||
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
           parser->current_token.type == DOUBLE_QMARK_OPERATOR)
    {
        int precedence = get_operator_precedence(parser->current_token.type);
        if (precedence < min_precedence)
            break;

        if (precedence >= min_precedence && precedence)
        {
            TreeNode *new_node = make_node(parser->current_token.type, parser->current_token.value, 0);
            add_child(new_node, expression_tree); // Add the current expression_tree as a child
            advance_token(parser);

            TreeNode *right = parse_expression(parser, parser->current_token, precedence + 1);
            add_child(new_node, right); // Add the right-hand side as another child
            expression_tree = new_node;
        }
        else
        {
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
           (token.type == IDENTIFIER && look_ahead(1).type != LEFT_PARENTHESIS) ||
           (token.type == IDENTIFIER && look_ahead(1).type != ASSIGNMENT_OPERATOR);
}

EvaluatedExpressionData evaluate_expression(TreeNode *node, Context *context, bool must_be_truth_value)
{
    EvaluatedExpressionData data;
    if (node == NULL)
    {
        data.type = EMPTY;
        return data;
    }

    // Base cases for literals
    if (node->type == INTEGER_LITERAL)
    {
        data.type = INT_SYMBOL_TYPE;
        return data;
    }
    else if (node->type == DOUBLE_LITERAL)
    {
        data.type = DOUBLE_SYMBOL_TYPE;
        return data;
    }
    else if (node->type == STRING_LITERAL)
    {
        data.type = STRING_SYMBOL_TYPE;
        return data;
    }
    else if (node->type == NIL_LITERAL)
    {
        data.type = NIL_SYMBOL_TYPE;
        return data;
    }
    else if (node->type == AST_IDENTIFIER)
    {
        if (context->function_called)
        {
            Symbol *symbol = find_symbol(context->sym_table_stack, node->value);
            if (symbol == NULL)
            {
                handle_error(SEMANTIC_TYPE_DERIV_ERROR);
            }
            data.type = symbol->type_and_value.type; // Only get the type
            data.value = symbol->type_and_value.value;
        }
        return data;
    }

    // Recursive evaluation of children for their types
    EvaluatedExpressionData left_eval = evaluate_expression(node->children[0], context, must_be_truth_value);
    EvaluatedExpressionData right_eval = evaluate_expression(node->children[1], context, must_be_truth_value);

    // Type determination for operators (without calculating values)
    switch (node->type)
    {
    case PLUS_OPERATOR:
        if (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE)
        {
            data.type = INT_SYMBOL_TYPE;
        }
        else if (left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE)
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else if ((left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) || 
                 (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE))
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else if (left_eval.type == STRING_SYMBOL_TYPE && right_eval.type == STRING_SYMBOL_TYPE)
        {
            data.type = STRING_SYMBOL_TYPE;
        }
        else
        {
            // Handle type incompatibility error for addition
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        
        break;
    case MINUS_OPERATOR:
         if (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE)
        {
            data.type = INT_SYMBOL_TYPE;
        }
        else if (left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE)
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else if ((left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) || 
                 (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE))
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else
        {
            // Handle type incompatibility error for addition
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case DIVISION_OPERATOR:
        if(left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE)
        {
            data.type = INT_SYMBOL_TYPE;
        }
        else if(left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE)
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else if ((left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) || 
                 (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE))
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case MULTIPLICATION_OPERATOR:
        if(left_eval.type == INT_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE)
        {
            data.type = INT_SYMBOL_TYPE;
        }
        else if(left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE)
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else if ((left_eval.type == DOUBLE_SYMBOL_TYPE && right_eval.type == INT_SYMBOL_TYPE) || 
                 (left_eval.type == INT_SYMBOL_TYPE && right_eval.type == DOUBLE_SYMBOL_TYPE))
        {
            data.type = DOUBLE_SYMBOL_TYPE;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case EQUALITY_OPERATOR:
        if(left_eval.type == right_eval.type)
        {
            data.type = BOOL_SYMBOL_TYPE;
        } 
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case NOT_EQUAL_OPERATOR:
        if(left_eval.type == right_eval.type)
        {
            data.type = BOOL_SYMBOL_TYPE;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case LESS_THAN_OPERATOR:
        if(left_eval.type == right_eval.type)
        {
            data.type = BOOL_SYMBOL_TYPE;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case GREATER_THAN_OPERATOR:
        if(left_eval.type == right_eval.type)
        {
            data.type = BOOL_SYMBOL_TYPE;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case LESS_THAN_EQUAL_OPERATOR:
        if(left_eval.type == right_eval.type)
        {
            data.type = BOOL_SYMBOL_TYPE;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case GREATER_THAN_EQUAL_OPERATOR:
        if(left_eval.type == right_eval.type)
        {
            data.type = BOOL_SYMBOL_TYPE;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case DOUBLE_QMARK_OPERATOR:
        if(left_eval.type != NIL_SYMBOL_TYPE)
        {
            data.type = left_eval.type;
        }
        else if(left_eval.type == NIL_SYMBOL_TYPE)
        {
            data.type = right_eval.type;
        }
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
        break;
    case EXCLAMATION_MARK:
        if(left_eval.type != NIL_SYMBOL_TYPE)
        {
            data.type = left_eval.type;
        } 
        else
        {
            handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
        }
    default:
        if (right_eval.type == EMPTY)
        {
            return left_eval;
        }
        else
        {
            return right_eval;
        }
    }
        
    if(must_be_truth_value && data.type != BOOL_SYMBOL_TYPE)
    {
        handle_error(SEMANTIC_RETURN_ERROR);
    }

    return data;
}

generate_code_for_expression(const char *var_to_save, TreeNode *node, Context *context)
{
    EvaluatedExpressionData dataL;
    EvaluatedExpressionData dataR;
    if (node == NULL)
    {
        return;
    }

    if(node->children_count > 0){
        generate_code_for_expression(var_to_save, node->children[0], context);
        generate_code_for_expression(var_to_save, node->children[1], context);
    }

    switch(node->type)
    {
    case INTEGER_LITERAL:
        print_pushs("int@", node->value);
        break;
    case STRING_LITERAL:
        print_pushs("string@", node->value);
        break;
    case DOUBLE_LITERAL:
        print_pushs("float@", node->value);
        break;
    case NIL_LITERAL:
        print_pushs("nil@nil", "");
        break;
    case PLUS_OPERATOR:
        dataL = evaluate_expression(node->children[0], context, false);
        dataR = evaluate_expression(node->children[1], context, false);
        if(dataL.type == STRING_SYMBOL_TYPE)
        {
            print_createframe();
            print_pushframe();
            print_declaration("LF@", "concat1");
            print_declaration("LF@", "concat2");
            print_pops("LF@", "concat2");
            print_pops("LF@", "concat1");
            print_concat("LF@concat1", "LF@concat1", "LF@concat2");
            print_pushs("LF@", "concat1");
            print_popframe();
        }
        else
        {
            if(dataL.type == DOUBLE_SYMBOL_TYPE && dataR.type == INT_SYMBOL_TYPE)
            {
                printf("INT2FLOATS\n");
            }
            else if (dataL.type == INT_SYMBOL_TYPE && dataR.type == DOUBLE_SYMBOL_TYPE)
            {
                print_createframe();
                print_pushframe();
                print_declaration("LF@", "tempVal");
                print_pops("LF@", "tempVal");
                printf("INT2FLOATS\n");
                print_pushs("LF@", "tempVal");
                print_popframe();
            }
            printf("ADDS\n");
        }
        break;
    case MINUS_OPERATOR:
        dataL = evaluate_expression(node->children[0], context, false);
        dataR = evaluate_expression(node->children[1], context, false);
        if(dataL.type == DOUBLE_SYMBOL_TYPE && dataR.type == INT_SYMBOL_TYPE)
        {
            printf("INT2FLOATS\n");
        }
        else if (dataL.type == INT_SYMBOL_TYPE && dataR.type == DOUBLE_SYMBOL_TYPE)
        {
            print_createframe();
            print_pushframe();
            print_declaration("LF@", "tempVal");
            print_pops("LF@", "tempVal");
            printf("INT2FLOATS\n");
            print_pushs("LF@", "tempVal");
            print_popframe();
        }
        printf("SUBS\n");
        break;
    case MULTIPLICATION_OPERATOR:
        dataL = evaluate_expression(node->children[0], context, false);
        dataR = evaluate_expression(node->children[1], context, false);
        if(dataL.type == DOUBLE_SYMBOL_TYPE && dataR.type == INT_SYMBOL_TYPE)
        {
            printf("INT2FLOATS\n");
        }
        else if (dataL.type == INT_SYMBOL_TYPE && dataR.type == DOUBLE_SYMBOL_TYPE)
        {
            print_createframe();
            print_pushframe();
            print_declaration("LF@", "tempVal");
            print_pops("LF@", "tempVal");
            printf("INT2FLOATS\n");
            print_pushs("LF@", "tempVal");
            print_popframe();
        }
        printf("MULS\n");
        break;
    case DIVISION_OPERATOR:
        dataL = evaluate_expression(node->children[0], context, false);
        dataR = evaluate_expression(node->children[1], context, false);
        if(dataL.type == INT_SYMBOL_TYPE && dataR.type == INT_SYMBOL_TYPE)
        {
            printf("IDIVS\n");
        }
        else
        {
            if(dataL.type == DOUBLE_SYMBOL_TYPE && dataR.type == INT_SYMBOL_TYPE)
            {
                printf("INT2FLOATS\n");
            }
            else if (dataL.type == INT_SYMBOL_TYPE && dataR.type == DOUBLE_SYMBOL_TYPE)
            {
                print_createframe();
                print_pushframe();
                print_declaration("LF@", "tempVal");
                print_pops("LF@", "tempVal");
                printf("INT2FLOATS\n");
                print_pushs("LF@", "tempVal");
                print_popframe();
            }
            printf("DIVS\n");
        }
        break;
    case LESS_THAN_OPERATOR:
        printf("LTS\n");
        break;
    case GREATER_THAN_OPERATOR:
        printf("GTS\n");
        break;
    case EQUALITY_OPERATOR:
        printf("EQS\n");
        break;
    case LESS_THAN_EQUAL_OPERATOR:
        printf("GTS\n");
        printf("NOTS\n");
        break;
    case GREATER_THAN_EQUAL_OPERATOR:
        printf("LTS\n");
        printf("NOTS\n");
        break;
    default:
        break;
    }
}