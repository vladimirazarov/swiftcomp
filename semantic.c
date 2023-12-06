/**
 * @file semantic.c
 * @brief Implementation of semantic analysis functions.
 *
 * This file contains the implementation of functions responsible for
 * performing semantic analysis on the Abstract Syntax Tree (AST). It ensures
 * that the source code adheres to the semantic rules of the IFJ23 programming language.
 * 
 * @author Vladimir Azarov
 */

#include "semantic.h"
#include "ast_tree.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include <stdbool.h>
#include "expressions.h"
#include "codegen.h"


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
    case AST_IDENTIFIER:
        return AST_IDENTIFIER;
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

char *replaceSpecialChars(const char *input)
{
    size_t outputSize = strlen(input) * 4 + 1;
    char *output = malloc(outputSize);

    if (!output)
    {
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; input[i] != '\0'; ++i)
    {
        unsigned char ch = input[i];
        if (ch <= 32 || ch == 35 || ch == 92)
        {
            snprintf(&output[j], outputSize - j, "\\%03d", ch);
            j += 4;
        }
        else
        {
            output[j++] = ch;
        }
    }
    output[j] = '\0';
    return output;
}

char *convert_double_char_to_hex_str(const char *valueStr)
{
    // Convert the string to a double
    char *end;
    double value = strtod(valueStr, &end);

    // Check if the conversion was successful
    if (end == valueStr)
    {
        printf("Conversion error: Invalid input string\n");
        return NULL;
    }

    // Buffer for the formatted double
    char formattedValue[64];
    snprintf(formattedValue, sizeof(formattedValue), "%a", value);

    // Allocate memory for the final string, including the prefix and formatted value
    char *finalStr = malloc(strlen("float@") + strlen(formattedValue) + 1);
    if (!finalStr)
    {
        printf("Memory allocation error\n");
        return NULL;
    }

    strcpy(finalStr, "float@");
    strcat(finalStr, formattedValue);

    return finalStr;
}

char* convert_double_num_to_hex_string(double value) {
    // Buffer for the formatted double
    char formattedValue[64];
    snprintf(formattedValue, sizeof(formattedValue), "%a", value);

    // Allocate memory for the final string, including the prefix and formatted value
    char *finalStr = malloc(strlen("float@") + strlen(formattedValue) + 1);
    if (!finalStr) {
        printf("Memory allocation error\n");
        return NULL;
    }

    strcpy(finalStr, "float@");
    strcat(finalStr, formattedValue);

    return finalStr;
}

void process_write(TreeNode *arguments_ast_node, Context *context)
{
    for (int i = arguments_ast_node->children_count - 1; i >= 0; i--)
    {
        TreeNode *current_arg = arguments_ast_node->children[i];
        TreeNode *argument_value_node = current_arg->children[current_arg->children_count - 1];

        ValueType argument_type = argument_value_node->type;

        if (argument_type == AST_INTEGER_LITERAL)
        {
            print_pushs("int@", argument_value_node->value);
        }
        else if (argument_type == AST_DOUBLE_LITERAL)
        {
            print_pushs("", convert_double_char_to_hex_str(argument_value_node->value));
        }
        else if (argument_type == AST_STRING_LITERAL)
        {
            print_pushs("string@", replaceSpecialChars(argument_value_node->value));
        }
        else if (argument_type == AST_NIL_LITERAL)
        {
            print_pushs("nil@nil", "");
        }
        else if (argument_type == AST_IDENTIFIER)
        {
            Symbol *symbol = find_in_table(context->sym_table_stack->top, argument_value_node->value);
            if (symbol == NULL)
                handle_error(SEMANTIC_UNDEF_ERROR);
            if (context->function_called|| context->in_block)
                print_pushs("LF@", symbol->name);
            else
                print_pushs("GF@", symbol->name);
        }
    }
    print_pushs_size_t("int@", arguments_ast_node->children_count);
    print_createframe();
    print_pushframe();
    print_call("", "$write");
    print_popframe();
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

        TreeNode *id = root->children[1];

        // Check if the identifier is already declared in the current scope
        if (find_in_current_table(context->sym_table_stack->top, id->value) != NULL)
            handle_error(SEMANTIC_UNDEF_ERROR);

        if (context->in_block || context->function_called)
            print_declaration("LF@", id->value);
        else
            print_declaration("GF@", id->value);

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
            if (root->children[2]->type == AST_EXPRESSION)
            {
                data = evaluate_expression(root->children[2], context, false);
                generate_code_for_expression(id->value, root->children[2], context);
                if (context->in_block)
                    print_pops("LF@", id->value);
                else 
                    print_pops("GF@", id->value);
            }

            if (root->children[2]->type == AST_FUNCTION_CALL_IN_ASSIGNMENT)
            {
                Symbol *function_symbol = find_symbol(context->sym_table_stack, root->children[2]->value);
                if (function_symbol == NULL)
                    handle_error(SEMANTIC_UNDEF_ERROR);
                if (function_symbol->functionInfo->returnType == NULL)
                    handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
                semantic_analysis(root->children[2], context);
                if (context->function_called|| context->in_block)
                    print_assignment("LF@", id->value, "LF@", LAST_RETURN_VALUE);
                else
                    print_assignment("GF@", id->value, "GF@", LAST_RETURN_VALUE);
                data.type = function_symbol->functionInfo->returnType;
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
            if (root->children[3]->type == AST_EXPRESSION){
                data = evaluate_expression(root->children[3], context, 0);
            generate_code_for_expression(id->value, root->children[3], context);
                if (context->in_block)
                    print_pops("LF@", id->value);
                else 
                    print_pops("GF@", id->value);
            }

            if (root->children[3]->type == AST_FUNCTION_CALL_IN_ASSIGNMENT)
            {
                Symbol *function_symbol = find_symbol(context->sym_table_stack->top, root->children[2]->value);
                if (function_symbol == NULL)
                    handle_error(SEMANTIC_UNDEF_ERROR);
                if (function_symbol->functionInfo->returnType == NIL_SYMBOL_TYPE)
                    handle_error(SEMANTIC_TYPE_COMPAT_ERROR);

                semantic_analysis(root->children[2], context);
            }
        }
        insert_symbol(context->sym_table_stack->top->table, create_symbol(id->value, data, is_initialized, modifier, NULL, is_nullable));
    }
    break;

    case AST_DECLARATION:
    {
        Modifier modifier;
        bool is_nullable = false;
        bool is_initialized = false;
        EvaluatedExpressionData data;

        TreeNode *id = root->children[1];

        // Check if the identifier is already declared in the current scope
        if (find_in_current_table(context->sym_table_stack->top, id->value) != NULL)
            handle_error(SEMANTIC_UNDEF_ERROR);

        if (context->in_block || context->function_called)
            print_declaration("LF@", id->value);
        else
            print_declaration("GF@", id->value);

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
            if (root->children[2]->type == AST_INT_NULLABLE_KEYWORD ||
                root->children[2]->type == AST_DOUBLE_NULLABLE_KEYWORD ||
                root->children[2]->type == AST_STRING_NULLABLE_KEYWORD)
            {
                is_nullable = true;
            }
            ValueType type = convert_ast_type_to_symbol_type(root->children[2]->type);
            data.type = type;
        }
        insert_symbol(context->sym_table_stack->top->table, create_symbol(id->value, data, is_initialized, modifier, NULL, is_nullable));
    }
    break;

    case AST_ASSIGNMENT:
       {
        EvaluatedExpressionData data;
        TreeNode *id = root->children[2];

        Symbol *id_symbol = find_in_current_table(context->sym_table_stack->top, id->value);
        SymbolValueU new_value;
        // Check if the identifier is already declared in the current scope
        if (id_symbol == NULL)
            handle_error(SEMANTIC_UNDEF_ERROR);

            if (root->children[3]->type == AST_EXPRESSION)
            {
                data = evaluate_expression(root->children[3], context, false);

                if (data.type != id_symbol->type_and_value.type)
                    handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
                generate_code_for_expression(id->value, root->children[3], context);
                if (context->in_block)
                    print_pops("LF@", id->value);
                else 
                    print_pops("GF@", id->value);

                if (data.type == INT_SYMBOL_TYPE)
                    id_symbol->type_and_value.value.int_value = data.value.int_value;
                if (data.type == STRING_SYMBOL_TYPE)
                    id_symbol->type_and_value.value.str_value = data.value.str_value;
                if (data.type == DOUBLE_SYMBOL_TYPE)
                    id_symbol->type_and_value.value.double_value= data.value.double_value;
                if (data.type == NIL_SYMBOL_TYPE){
                    if (id_symbol->isNullable == false)
                        handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
                    id_symbol->type_and_value.value.is_nil = true;
                }
            }

            if (root->children[3]->type == AST_FUNCTION_CALL_IN_ASSIGNMENT)
            {
                EvaluatedExpressionData data = context->last_return_value;
                Symbol *function_symbol = find_symbol(context->sym_table_stack, root->children[3]->value);
                if (function_symbol == NULL)
                    handle_error(SEMANTIC_UNDEF_ERROR);
                if (function_symbol->functionInfo->returnType == NULL)
                    handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
                semantic_analysis(root->children[3], context);
                root->children[3] = NULL;
               if(context->last_return_value.type != id_symbol->type_and_value.type) 
                    handle_error(SEMANTIC_TYPE_COMPAT_ERROR);

                if (data.type == INT_SYMBOL_TYPE)
                    id_symbol->type_and_value.value.int_value = data.value.int_value;
                if (data.type == STRING_SYMBOL_TYPE)
                    id_symbol->type_and_value.value.str_value = data.value.str_value;
                if (data.type == DOUBLE_SYMBOL_TYPE)
                    id_symbol->type_and_value.value.double_value= data.value.double_value;
                if (data.type == NIL_SYMBOL_TYPE){
                    if (id_symbol->isNullable == false)
                        handle_error(SEMANTIC_TYPE_COMPAT_ERROR);
                    id_symbol->type_and_value.value.is_nil = true;
                }

                if (context->function_called|| context->in_block)
                    print_assignment("LF@", id->value, "LF@", LAST_RETURN_VALUE);
                else
                    print_assignment("GF@", id->value, "GF@", LAST_RETURN_VALUE);
            }
    }
    break;
    case AST_FUNCTION_CALL_IN_ASSIGNMENT:
    case AST_FUNCTION_CALL:
    {
        // 1. Find function symbol
        Symbol *function_symbol = find_symbol(context->sym_table_stack, root->value);
        if (!function_symbol || function_symbol->type_and_value.type != FUNCTION_SYMBOL_TYPE)
        {
            handle_error(SEMANTIC_UNDEF_ERROR);
        }
        TreeNode *arguments_ast_node;
        if(root->type == AST_FUNCTION_CALL)
            arguments_ast_node = root->children[1];
        if(root->type == AST_FUNCTION_CALL_IN_ASSIGNMENT)
            arguments_ast_node = root->children[0];

        if (strcmp(function_symbol->name, "write") == 0)
        {
            process_write(arguments_ast_node, context);
            break;
        }
        TreeNode *external_argument_name_node;
        TreeNode *internal_argument_name_node;
        ValueType argument_type;
        Symbol *id_symbol = NULL;


        // 2. Evaluate arguments and check types, create new scope and insert new symbols
        SymbolTable *function_scope_table = init_symbol_table();
        push(context->sym_table_stack, function_scope_table);

        Parameter *expected_param = function_symbol->functionInfo->parameters;
        for (int i = 0; i < arguments_ast_node->children_count; i++, expected_param = expected_param->next)
        {
            TreeNode *current_arg = arguments_ast_node->children[i];
            char *provided_external_name = (current_arg->children_count == 2) ? current_arg->children[0]->value : NULL;

            // Check if provided external name matches the expected external name, if applicable
            if (provided_external_name && expected_param->name && strcmp(expected_param->name, provided_external_name) != 0)
                handle_error(SEMANTIC_PARAM_TYPE_ERROR);

            // Evaluate the argument and check types
            ValueType argument_type = convert_ast_type_to_symbol_type(current_arg->children[current_arg->children_count - 1]->type);
            bool can_be_null = check_is_nullable_by_type(current_arg->children[current_arg->children_count - 1]->type);
            TreeNode *argument_value_node = current_arg->children[current_arg->children_count - 1];

            if (argument_value_node->type == AST_IDENTIFIER)
            {
                // Retrieve symbol for an identifier
                Symbol *id_symbol = find_in_table(context->sym_table_stack->top, argument_value_node->value);
                // Identifier wasnt defined
                if (id_symbol == NULL)
                    handle_error(SEMANTIC_UNDEF_ERROR);
                // Type check
                if (can_be_null == true)
                {
                    if ((id_symbol->type_and_value.type != expected_param->type) && argument_type != NIL_SYMBOL_TYPE)
                        handle_error(SEMANTIC_PARAM_TYPE_ERROR);
                }
                else
                {
                    if ((id_symbol->type_and_value.type != expected_param->type))
                        handle_error(SEMANTIC_PARAM_TYPE_ERROR);
                }

                // Insert symbol into new scope and change its name to be used inside function
                Symbol new_scope_symbol = *id_symbol;
                new_scope_symbol.name = strdup(expected_param->identifier);
                new_scope_symbol.isNullable = can_be_null;
                new_scope_symbol.modifier = CONSTANT;
                insert_symbol(function_scope_table, new_scope_symbol);
            }
            else
            {
                if (can_be_null == true)
                {
                    if ((expected_param->type != argument_type) && argument_type != NIL_SYMBOL_TYPE)
                        handle_error(SEMANTIC_PARAM_TYPE_ERROR);
                }
                else
                {
                    if ((expected_param->type != argument_type))
                        handle_error(SEMANTIC_PARAM_TYPE_ERROR);
                }

                EvaluatedExpressionData data;
                data.type = expected_param->type;
                ValueType type = expected_param->type;

                if (type == INT_SYMBOL_TYPE)
                {
                    data.value.int_value = atoi(argument_value_node->value);
                }
                else if (type == DOUBLE_SYMBOL_TYPE)
                {
                    data.value.double_value = atof(argument_value_node->value);
                }
                else if (type == STRING_SYMBOL_TYPE)
                {
                    // Assuming string handling is correctly implemented elsewhere
                    data.value.str_value = strdup(argument_value_node->children[current_arg->children_count - 1]->value);
                }

                Symbol new_scope_symbol = create_symbol(expected_param->identifier, data, true, CONSTANT, NULL, can_be_null);
                insert_symbol(function_scope_table, new_scope_symbol);
            }
        }

        // Ensure all parameters were provided
        if (expected_param != NULL)
            handle_error(SEMANTIC_PARAM_TYPE_ERROR);
        for (int i = arguments_ast_node->children_count - 1; i >= 0; i--)
        {

            TreeNode *current_arg = arguments_ast_node->children[i];
            TreeNode *argument_value_node = current_arg->children[current_arg->children_count - 1];
            ASTNodeType argument_type = argument_value_node->type;
            if (argument_type == AST_INTEGER_LITERAL)
            {
                print_pushs("int@", argument_value_node->value);
            }
            else if (argument_type == AST_DOUBLE_LITERAL)
            {
                print_pushs("float@", argument_value_node->value);
            }
            else if (argument_type == AST_STRING_LITERAL)
            {
                print_pushs("string@", argument_value_node->value);
            }
            else if (argument_type == AST_NIL_LITERAL)
            {
                print_pushs("nil@nil", "");
            }
            else if (argument_type == AST_IDENTIFIER)
            {
                print_pushs("LF@", argument_value_node->value);
            }
        }
        print_createframe();
        print_pushframe();

        context->function_called = true;
        context->current_function = function_symbol;

        if (!(function_symbol->functionInfo->label_was_created)) 
        {
            print_jump_to_function_end("$$", function_symbol->name);
            print_label("$", function_symbol->name);
            print_declaration("LF@", "!RETURN");
            Parameter *curParam = function_symbol->functionInfo->parameters;
            for (int i = 0; i < function_symbol->functionInfo->parameterCount; i++)
            {
                char *internal_name = curParam->name;
                print_declaration("LF@", internal_name);
                print_pops("LF@", internal_name);
                curParam = curParam->next;
            }
            semantic_analysis(function_symbol->functionInfo->body, context);
            print_label_function_end("$$", function_symbol->name); 
        }
        print_call("$", function_symbol->name);

        context->function_called = false;
        pop(context->sym_table_stack);
        print_popframe();
        if (function_symbol->functionInfo->returnType != NIL_SYMBOL_TYPE)
            print_pops("GF@", LAST_RETURN_VALUE);
    }
    break;

    case AST_BLOCK:
    {
        if (!(context->function_is_being_declared) ||  !(context->function_called))
        {
            // Create a new scope for the block
            SymbolTable *table;
            table = init_symbol_table();
            push(context->sym_table_stack, table);
            print_createframe();
            print_pushframe();
        }
        // We dont analyze function contents because they were analyzed before
        if (!(context->function_called)){ 
            // Analyze block contents
            for (size_t i = 0; i < root->children_count; ++i)
            {
                context->in_block = true;
                semantic_analysis(root->children[i], context);
                root->children[i] = NULL;
                context->in_block = false;
            }
        }

        if (!(context->function_is_being_declared)){
            pop(context->sym_table_stack);
            print_popframe();
        }
        else
            return;
    }
    break;

    case AST_RETURN_STATEMENT:
    {
        if (!context->function_called)
        {
            handle_error(SEMANTIC_OTHER_ERROR);
        }
        else  
        {
            // Determine the type of the return expression
            if (context->current_function != NULL)
            generate_code_for_expression("!RETURN", root->children[1], context);
                if (context->in_block)
                    print_pops("LF@", "!RETURN");
                else 
                    print_pops("GF@", "!RETURN");

            context->last_return_value = evaluate_expression(root->children[1], context, false);
            // if (return_expr.type != context->current_function->functionInfo->returnType)
            //{
            // handle_error(SEMANTIC_PARAM_TYPE_ERROR);
            //}
            if (context->current_function != NULL){
            print_pushs("LF@!RETURN", "");
            print_return();
            }
        }
    }
    break;

case AST_CONDITIONAL:
{

    TreeNode *condition = root->children[0];
    TreeNode *true_branch = root->children[1];
    TreeNode *false_branch = NULL;
    
    // Check if there's an 'else' branch
    if (root->children_count == 3)
        false_branch = root->children[2];

    // Analyze the condition
    EvaluatedExpressionData condition_data = evaluate_expression(condition, context, true);

    // Ensure the condition is a boolean
    if (condition_data.type != BOOL_SYMBOL_TYPE)
        handle_error(SEMANTIC_TYPE_COMPAT_ERROR);

    // Analyze the 'true' branch
    semantic_analysis(true_branch, context);

    // Analyze the 'false' branch if it exists
    if (false_branch != NULL)
        semantic_analysis(false_branch, context);
}
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
        root->children[i] = NULL;
    }
}

void add_functions_to_symtable(TreeNode *root, Context *context)
{
    if (root == NULL)
        return;

    switch (root->type)
    {

    case AST_FUNCTION_DEFINITION:
    {
        if (context->function_is_being_declared)
        {
            handle_error(SEMANTIC_OTHER_ERROR);
        }
        else
        {
            // Enter function context
            context->function_is_being_declared= true;
            // Create new symbol that will be later inserted into symtable
            Symbol *function_symbol = malloc(sizeof(Symbol));
            FunctionInfo *function_info = malloc(sizeof(FunctionInfo));
            function_symbol->functionInfo = function_info;
            // Check if the function identifier is valid
            char *function_name = root->value;
            function_symbol->name = function_name;
            TreeNode *params_node = root->children[0];
            function_symbol->functionInfo->parameterCount = params_node->children_count;
            function_symbol->functionInfo->label_was_created = false;
            Parameter *last_parameter = NULL;

            if (find_symbol(context->sym_table_stack, function_symbol->name) != NULL)
            {
                handle_error(SEMANTIC_UNDEF_ERROR);
            }

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
                else
                    new_parameter->name = strdup(external_name); // Duplicate the name

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
                root->children[1] = NULL;
            }
            if (root->children_count == 3)
            {
                TreeNode *return_type_node = root->children[1];
                function_symbol->functionInfo->returnType = convert_ast_type_to_symbol_type(return_type_node->type);
                function_symbol->isNullable = check_is_nullable_by_type(return_type_node->type);
                body_node = root->children[2];
                root->children[2] = NULL;
            }

            function_symbol->functionInfo->body = body_node->children[1];
            function_symbol->type_and_value.type = FUNCTION_SYMBOL_TYPE;
            EvaluatedExpressionData data;
            data.type = FUNCTION_SYMBOL_TYPE;

            insert_symbol(context->sym_table_stack->top->table, *function_symbol);
            context->function_is_being_declared= false;
        }
    }
    break;

        break;
    default:
        break;
    }

    // Recursive DFS traversal for each child
    for (size_t i = 0; i < root->children_count; ++i)
    {
        add_functions_to_symtable(root->children[i], context);
    }
}

void add_built_in_functions(Context *context)
{
    // printf("%s", FUNCTION_READSTRING);
    // printf("%s", FUNCTION_READINT);
    // printf("%s", FUNCTION_READDOUBLE);
    EvaluatedExpressionData data;
    char *write = strdup("write");
    data.type = FUNCTION_SYMBOL_TYPE;
    Symbol created_symbol_write = create_symbol(write, data, true, CONSTANT, NULL, false);
    insert_symbol(context->sym_table_stack->top->table, created_symbol_write);
    printf("%s", FUNCTION_WRITE);
    // printf("%s", FUNCTION_INT2DOUBLE);
    // printf("%s", FUNCTION_DOUBLE2INT);
    // printf("%s", FUNCTION_LENGTH);
    // printf("%s", FUNCTION_SUBSTRING);
    // printf("%s", FUNCTION_CHR);
    // printf("%s", FUNCTION_ORD);
}
int main()
{
    Parser *parser = create_parser();
    TreeNode *program = parse_program(parser);
    free_parser(parser);

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
    context->function_called = false;
    context->current_function = NULL;
    context->function_is_being_declared = false;
    context->in_block = false;
    
    printf(".IFJcode23\n");
    print_jump("", "$$MAIN");
    add_functions_to_symtable(program, context);
    add_built_in_functions(context);
    print_label("", "$$MAIN");
    print_declaration("GF@", LAST_RETURN_VALUE);

    semantic_analysis(program, context);

    return 0;
}