#include "semantic.h"
#include "ast_tree.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <stdbool.h>
#include "expressions.h"

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

const char *tokenTypeToString(TokenType type)
{
    switch (type)
    {
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

ValueType convert_ast_type_to_symbol_type(ASTNodeType ast_node_type)
{
    switch (ast_node_type)
    {
    case AST_INT_KEYWORD:
        return INT_SYMBOL_TYPE;
        break;
    case AST_INTEGER_LITERAL:
        return INT_SYMBOL_TYPE;
        break;
    case AST_DOUBLE_KEYWORD:
        return DOUBLE_SYMBOL_TYPE;
        break;
    case AST_DOUBLE_LITERAL:
        return DOUBLE_SYMBOL_TYPE;
        break;
    case AST_STRING_KEYWORD:
        return STRING_SYMBOL_TYPE;
        break;
    case AST_STRING_LITERAL:
        return STRING_SYMBOL_TYPE;
        break;
    case AST_NIL_LITERAL:
        return NIL_SYMBOL_TYPE;
        break;
    case AST_INT_NULLABLE_KEYWORD:
        return INT_SYMBOL_TYPE;
        break;
    case AST_DOUBLE_NULLABLE_KEYWORD:
        return DOUBLE_SYMBOL_TYPE;
        break;
    case AST_STRING_NULLABLE_KEYWORD:
        return STRING_SYMBOL_TYPE;
        break;
    default:
        break;
    }
}
bool check_is_nullable_by_type(ASTNodeType ast_node_type)
{
    switch (ast_node_type)
    {
    case AST_INT_NULLABLE_KEYWORD:
        return true;
        break;
    case AST_DOUBLE_NULLABLE_KEYWORD:
        return true;
        break;
    case AST_STRING_NULLABLE_KEYWORD:
        return true;
        break;
    default:
        return false;
        break;
    }
}

bool check_type_for_func_args(TreeNode *node, ValueType expectedType, Context *context)
{
    // This is a placeholder function. You should fill in the implementation based on your AST node structure and symbol table.
    // The following is a pseudo-code outline:

    // If the node is an identifier, look it up in the symbol table to get its type
    if (node->type == AST_IDENTIFIER)
    {
        Symbol *symbol = find_symbol(context->sym_table_stack->top, node->value);
        return symbol && symbol->type_and_value.type == expectedType;
    }
    // If the node is a literal, compare its type directly
    else if (node->type == AST_INTEGER_LITERAL && expectedType == INT_SYMBOL_TYPE)
    {
        return true;
    }
    else if (node->type == AST_DOUBLE_LITERAL && expectedType == DOUBLE_SYMBOL_TYPE)
    {
        return true;
    }
    else if (node->type == AST_STRING_LITERAL && expectedType == STRING_SYMBOL_TYPE)
    {
        return true;
    }
    // Add more cases as needed for other types of literals or expressions

    return false; // Return false if none of the types match
}

void semantic_analysis(TreeNode *root, Context *context)
{
    if (root == NULL)
        return;

    switch (root->type)
    {

    case AST_INITIALIZATION:
    {
        Modifier modifier;
        bool is_nullable = false;
        bool is_initialized = true;
        EvaluatedExpressionData data;

        // Evaluate the expression to get its type and value

        TreeNode *id = root->children[1];
        if (root->children[0]->type == VAR_KEYWORD)
        {
            modifier = VARIABLE;
        }
        else if (root->children[0]->type == LET_KEYWORD)
        {
            modifier = CONSTANT;
        }

        if (root->children_count == 3)
        {
            is_nullable = false;
            if (root->children[2]->type = AST_EXPRESSION)
                data = evaluate_expression(root->children[2], context, 0);
            if (root->children[2]->type = AST_FUNCTION_CALL)
            {
                Symbol *function_symbol = find_symbol(context->sym_table_stack->top, root->children[2]->value);
                if (function_symbol == NULL)
                    handle_error(SEMANTIC_UNDEF_ERROR);
                if (function_symbol->functionInfo->returnType == NULL)
                    handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
                else
                {
                    // semantic_analysis(root je function call, context);
                }
            }
        }
        else if (root->children_count == 4)
        {
            if (root->children[2]->type == AST_INT_NULLABLE_KEYWORD ||
                root->children[2]->type == AST_DOUBLE_NULLABLE_KEYWORD ||
                root->children[2]->type == AST_STRING_NULLABLE_KEYWORD)
            {
                is_nullable = true;
            }
            if (root->children[3]->type = AST_EXPRESSION)
                data = evaluate_expression(root->children[2], context, 0);
            if (root->children[3]->type = AST_FUNCTION_CALL)
            {
                Symbol *function_symbol = find_symbol(context->sym_table_stack->top, root->children[2]->value);
                if (function_symbol == NULL)
                    handle_error(SEMANTIC_UNDEF_ERROR);
                if (function_symbol->functionInfo->returnType == NULL)
                    handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
                else
                {
                    // semantic_analysis(root je function call, context);
                }
            }
        }

        // Check if the identifier is already declared in the current scope
        if (find_symbol(context->sym_table_stack->top, id->value) != NULL)
        {
            handle_error(SEMANTIC_UNDEF_ERROR);
        }
        else
        {
            insert_symbol(context->sym_table_stack->top, create_symbol(id->value, data, is_initialized, modifier, NULL, is_nullable));
        }
    }
    break;

    // TODO: FUNCTION CALL HANDLING
    case AST_DECLARATION:
    {
        Modifier modifier;
        bool is_nullable = false;
        bool is_initialized = false;
        ValueType type;
        EvaluatedExpressionData data;

        if (root->children[0]->type == VAR_KEYWORD)
        {
            modifier = VARIABLE;
        }
        else if (root->children[0]->type == LET_KEYWORD)
        {
            modifier = CONSTANT;
        }

        TreeNode *id = root->children[1];

        if (root->children[2]->type == AST_INT_NULLABLE_KEYWORD ||
            root->children[2]->type == AST_DOUBLE_NULLABLE_KEYWORD ||
            root->children[2]->type == AST_STRING_NULLABLE_KEYWORD)
        {
            is_nullable = true;
        }

        if (root->children[2]->type == AST_INT_NULLABLE_KEYWORD ||
            root->children[2]->type == AST_INT_KEYWORD)
            type = INT_SYMBOL_TYPE;
        if (root->children[2]->type == AST_DOUBLE_NULLABLE_KEYWORD ||
            root->children[2]->type == AST_DOUBLE_KEYWORD)
            type = DOUBLE_SYMBOL_TYPE;
        if (root->children[2]->type == AST_STRING_NULLABLE_KEYWORD ||
            root->children[2]->type == AST_STRING_KEYWORD)
            type = STRING_SYMBOL_TYPE;

        data.type = type;

        // Check if the identifier is already declared in the current scope
        if (find_symbol(context->sym_table_stack->top, id->value) != NULL)
        {
            handle_error(SEMANTIC_UNDEF_ERROR);
        }
        else
        {
            insert_symbol(context->sym_table_stack->top, create_symbol(id->value, data, is_initialized, modifier, NULL, is_nullable));
        }
    }
    break;
    case AST_ASSIGNMENT:
        // Implement assignment checks
        break;
    // TODO: CHECK IF ARG CAN BE NULL BY CHECKING SYMBOLM BLABLBALBA
    case AST_FUNCTION_CALL:
    {
            // 1. Find function symbol
            Symbol *function_symbol = find_symbol(context->sym_table_stack, root->value);
            if (!function_symbol || function_symbol->type_and_value.type != FUNCTION_SYMBOL_TYPE)
                {
                    handle_error(SEMANTIC_UNDEF_ERROR);
                }
            TreeNode *arguments_ast_node = root->children[1];
            TreeNode *external_argument_name_node;
            TreeNode *internal_argument_name_node;
            ValueType argument_type;

            // 2. Evaluate arguments and check types
            for (int i = 0; i<arguments_ast_node->children_count; i++){
                TreeNode* current_arg = arguments_ast_node->children[i];
                // Argument is specified with external name (e.g. with: "str")
                if (current_arg->children_count == 2){
                    external_argument_name_node = current_arg->children[0];
                    internal_argument_name_node = current_arg->children[1];
                    argument_type = convert_ast_type_to_symbol_type(internal_argument_name_node->type);
                }
                // Argument is specified witout external name (e.g. str)
                if (current_arg->children_count == 1){

                    argument_type = convert_ast_type_to_symbol_type(current_arg->children[0]->type);
                }
            }
            break;
    }

    case AST_FUNCTION_DEFINITION:
    {
        if (context->in_function)
        {
            handle_error(SEMANTIC_OTHER_ERROR);
        }
        else
        {
            // Enter function context
            context->in_function = true;

            // Create new symbol that will be later inserted into symtable
            Symbol *function_symbol = malloc(sizeof(Symbol));
            FunctionInfo *function_info = malloc(sizeof(FunctionInfo));
            if (!function_info)
            {
                handle_error(INTERNAL_ERROR);
            }
            function_symbol->functionInfo = function_info;
            // Check if the function identifier is valid
            char *function_name = root->value;
            function_symbol->name = function_name;
            TreeNode *params_node = root->children[0];
            function_symbol->functionInfo->parameterCount = params_node->children_count;
            Parameter *last_parameter = NULL;
            function_symbol->type_and_value.type = FUNCTION_SYMBOL_TYPE;

            for (int i = 0; i < params_node->children_count; i++)
            {
                // Allocate memory for a new parameter
                Parameter *new_parameter = (Parameter *)malloc(sizeof(Parameter));
                if (!new_parameter)
                {
                    handle_error(INTERNAL_ERROR);
                }

                // Initialize the new parameter
                new_parameter->name = NULL;       // Default to NULL
                new_parameter->identifier = NULL; // Default to NULL
                new_parameter->type = convert_ast_type_to_symbol_type(params_node->children[i]->children[1]->type);
                new_parameter->next = NULL; // The next parameter is not known yet
                new_parameter->is_nullable = check_is_nullable_by_type(params_node->children[i]->children[1]->type);
                ASTNodeType external_name_type = params_node->children[i]->children[0]->children[0]->type;
                ASTNodeType internal_name_type = params_node->children[i]->children[0]->children[1]->type;
                char *external_name = params_node->children[i]->children[0]->children[0]->value;
                char *internal_name = params_node->children[i]->children[0]->children[1]->value;

                // Handle specific cases for the name
                if (external_name_type == AST_UNDERSCORE)
                {
                    new_parameter->name = NULL;
                }
                else if (external_name_type == AST_EXTERNAL_NAME)
                {
                    if (strcmp(external_name, internal_name) == 0)
                    {
                        free(new_parameter);
                        handle_error(SEMANTIC_OTHER_ERROR);
                    }
                    new_parameter->name = strdup(external_name); // Duplicate the name
                }

                // Duplicate the identifier
                new_parameter->identifier = strdup(internal_name);

                // Append to the list
                if (last_parameter != NULL)
                {
                    last_parameter->next = new_parameter; // Link the new parameter to the list
                }
                else
                {
                    function_symbol->functionInfo->parameters = new_parameter; // This is the first parameter, set it as head of the list
                }

                // Move the last parameter pointer
                last_parameter = new_parameter;
            }
            TreeNode *body_node;
            // Check return type, if necessary
            if (root->children_count == 2)
            {
                function_symbol->functionInfo->returnType = NIL_SYMBOL_TYPE;
                body_node = root->children[1];
            }
            if (root->children_count == 3)
            {
                TreeNode *return_type_node = root->children[1];
                function_symbol->functionInfo->returnType = convert_ast_type_to_symbol_type(return_type_node->type);
                function_symbol->isNullable = check_is_nullable_by_type(return_type_node->type);
                body_node = root->children[2];
            }

            function_symbol->functionInfo->body = body_node;
            EvaluatedExpressionData data;
            if (find_symbol(context->sym_table_stack, function_symbol->name) != NULL)
            {
                handle_error(SEMANTIC_UNDEF_ERROR);
            }
            else
            {
                insert_symbol(context->sym_table_stack->top->table, create_symbol(function_symbol->name, data, false, CONSTANT, function_symbol->functionInfo, function_symbol->isNullable));
            }
            context->current_function = find_symbol(context->sym_table_stack, function_symbol->name);
            semantic_analysis(body_node, context);
            context->in_function = false;
        }
    }
    break;

    case AST_BLOCK:
    {
        // Create a new scope for the block
        SymbolTable *table;
        table = init_symbol_table();
        push(context->sym_table_stack, table);

        // Analyze block contents
        for (size_t i = 0; i < root->children_count; ++i)
        {
            semantic_analysis(root->children[i], context);
        }

        pop(context->sym_table_stack);
    }
    break;

    case AST_RETURN_STATEMENT:
    {
        if (context->current_function == NULL)
        {
            handle_error(SEMANTIC_OTHER_ERROR);
        }
        else
        {
            // Determine the type of the return expression
            EvaluatedExpressionData return_expr = evaluate_expression(root->children[0], context, false);
            // Compare against the function's return type
            if (return_expr.type != context->current_function->functionInfo->returnType)
            {
                handle_error(SEMANTIC_RETURN_ERROR);
            }
        }
    }
    break;

    case AST_CONDITIONAL:
        // Implement if statement checks
        break;
    case AST_ELSE_PART:
        break;
    case AST_LOOP:
        break;
    default:
        // Unknown node type
        break;
    }

        // Recursive DFS traversal for each child
        for (size_t i = 0; i < root->children_count; ++i)
        {
            semantic_analysis(root->children[i], context);
        }
    }

int main()
{
    Parser *parser = create_parser();
    TreeNode *program = parse_program(parser);
    free_parser(parser);

    print_ast(program, 0);
    SymbolTable *table;
    table = init_symbol_table();
    Stack *sym_table_stack;
    sym_table_stack = init_stack();
    SymStackNode *init_sym_stack_node = malloc(sizeof(SymStackNode));
    init_sym_stack_node->table = table;
    init_sym_stack_node->next = NULL;
    sym_table_stack->top = init_sym_stack_node;
    Context *context = (Context *)malloc(sizeof(Context));
    context->sym_table_stack = sym_table_stack;

    // First traversal for semantic analysis
    semantic_analysis(program, context);
    print_symbol_table(context->sym_table_stack->top->table);

    // Second traversal for code generation
    // code_generation(root, buffer);

    return 0;
}