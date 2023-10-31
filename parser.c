
// PROMENNE
// Vstupni bod = Neoznačená nesouvislá sekvence příkazů, to znamena hlavni telo programu (treba jako v pythonu)
// Promenne = 1. globalni ... definovane v hlavnim tele programu, rozsah v celem programu
//            2. lokalni ... rozsah - od pocatku bloku do konce
//                           2.1. v zanorenych blocich hlavniho tela
//                           2.2. v uzivatelskych funkcich
// Rozsah platnosti
// Blok: Sekvence příkazů v těle funkce, v rámci větve podmíněného příkazu nebo příkazu cyklu.
// Proměnná je dostupná v bloku a jeho podblokách, pokud není překryta jinou proměnnou se stejným názvem.

// FUNKCE
// Funkce v IFJ23 nemusí být nutně definovány před jejich použitím, mohou být lexikálně umístěny i za kódem, kde jsou volány.
// Parametry funkce jsou předávány hodnotou a nelze je měnit.
// Návratová hodnota funkce je dána příkazem return; v případě chybějící nebo špatného typu návratové hodnoty dochází k chybě.
// V jazyce IFJ23 není třeba podporovat definice vnořených funkcí.

// CO JE PARSER?
// program kt generuje PARSE TREE pro dany RETEZEC pokud RETEZEC muze byt generovan definovanou LL-GRAMATIKOU

// VSTUP - potok tokenu
// VYSTUP - syntakticky strom

//    Declaration: let c: int;
//    Assignment:  c = 3;
//    Initialization: let c = 3;

#include "expressions.h"
#include "parser.h"
#include "lexer.h"
#include "error.h"
#include "ast_tree.h"
#include <stdbool.h>
#include <stdlib.h>

#define N 3  // The number of tokens to look ahead

bool is_return_value = false;

Token token_buffer[N];

// Function to create a new parser
Parser *create_parser()
{
    Parser *parser = malloc(sizeof(Parser));
    if (!parser)
    {
        handle_error(INTERNAL_ERROR);
    }
    init_token_buffer();
    parser->current_token = token_buffer[0];
    return parser;
}
// Function to initialize the token buffer
void init_token_buffer()
{
    for (int i = 0; i < N; ++i)
    {
        token_buffer[i] = get_token();
    }
}

// Function to look ahead n tokens
Token look_ahead(int n)
{
    if (n < N)
    {
        return token_buffer[n];
    }
    // Handle error if look-ahead too far
    handle_error(INTERNAL_ERROR);
    return token_buffer[0]; // Dummy return to make the compiler happy
}

// Function to destroy a parser
void free_parser(Parser *parser)
{
    if (parser)
    {
        free(parser);
    }
}

void accept_token(Parser *parser, TokenType expected_type)
{
    if (parser->current_token.type == expected_type)
    {
        advance_token(parser);
    }
    else
    {
        handle_error(SYNTACTIC_ERROR);
    }
}

// Function to advance to the next token
void advance_token(Parser *parser)
{
    // Pop the front of the buffer and push a new token onto the end
    for (int i = 1; i < N; ++i)
    {
        token_buffer[i - 1] = token_buffer[i];
    }
    token_buffer[N - 1] = get_token();
    // Update the current token
    parser->current_token = token_buffer[0];
}

// Function to parse a program
TreeNode *parse_program(Parser *parser) {
    TreeNode *root = make_node(AST_PROGRAM, NULL, 2);
    
    if (parser->current_token.type != END_OF_FILE) {
        TreeNode *sectionNode = parse_section(parser);
        TreeNode *moreSectionsNode = parse_more_sections(parser);  // Function to parse additional sections

        root->children[0] = sectionNode;
        root->children[1] = moreSectionsNode;
    } else {
        handle_error(SYNTACTIC_ERROR);
    }
    return root;
}

// Function to parse a section
TreeNode *parse_section(Parser *parser) {
    TreeNode *sectionNode = make_node(AST_SECTION, NULL, 2);
    
    TreeNode *statementsNode = parse_statements(parser);
    TreeNode *functionDefinitionsNode = parse_function_definitions(parser);

    sectionNode->children[0] = statementsNode;
    sectionNode->children[1] = functionDefinitionsNode;
    
    return sectionNode;
}

// Function to parse additional sections
TreeNode *parse_more_sections(Parser *parser) {
    // If we've reached the end of the file, return NULL as there are no more sections
    if (parser->current_token.type == END_OF_FILE) {
        return NULL;
    }
    
    // Create a new AST node for this additional section
    TreeNode *moreSectionsNode = make_node(AST_MORE_SECTIONS, NULL, 2);

    // Parse the next section
    TreeNode *sectionNode = parse_section(parser);
    moreSectionsNode->children[0] = sectionNode;

    // Recursively parse even more sections, if any
    TreeNode *furtherSectionsNode = parse_more_sections(parser);
    moreSectionsNode->children[1] = furtherSectionsNode;

    return moreSectionsNode;
}

// Function to parse statements
TreeNode* parse_statements(Parser *parser) {
    // Create a new AST node for AST_STATEMENTS
    TreeNode *statementsNode = make_node(AST_STATEMENTS, NULL, 0);
    
    // Initialize the children_count to 0, the add_child function will handle dynamic resizing
    statementsNode->children_count = 0;

    while (parser->current_token.type != END_OF_FILE &&
           parser->current_token.type != FUNCTION_KEYWORD) {
        TreeNode *stmtNode = NULL; // The current statement node

        switch (parser->current_token.type) {
            case VAR_KEYWORD:
            case LET_KEYWORD:
                stmtNode = parse_declaration_or_initialization(parser);
                break;
            case IDENTIFIER:
                stmtNode = parse_assignment_or_function_call(parser);
                break;
            case RETURN_KEYWORD:
                stmtNode = parse_return_statement(parser);
                break;
            case WHILE_KEYWORD:
                stmtNode = parse_while_loop(parser);
                break;
            case IF_KEYWORD:
                stmtNode = parse_if_cond(parser);
                break;
            case LEFT_CURLY_BRACE:
                stmtNode = parse_block(parser);
                break;
            case RIGHT_CURLY_BRACE:
                // End of block; return the AST_STATEMENTS node
                return statementsNode;
            default:
                handle_error(SYNTACTIC_ERROR);
                return NULL;
        }

        // Add the statement node to AST_STATEMENTS children array
        add_child(statementsNode, stmtNode);
    }
    
    return statementsNode;
}

TreeNode *parse_declaration_or_initialization(Parser *parser)
{
    // Create the root node for this subtree
    TokenType keyword = parser->current_token.type; // VAR_KEYWORD or LET_KEYWORD
    TreeNode *keywordNode;
    if (keyword == VAR_KEYWORD){
        keywordNode = make_node(AST_VAR_KEYWORD, NULL, 0);
        accept_token(parser, VAR_KEYWORD);
    }
    else if (keyword == LET_KEYWORD){
        keywordNode = make_node(AST_LET_KEYWORD, NULL, 0);
        accept_token(parser, LET_KEYWORD);
    }

    TreeNode *root = NULL; // Will point to either a Declaration or Initialization node

    // Accept the identifier and create its node
    char *idValue = parser->current_token.value;
    TreeNode *idNode = make_node(AST_IDENTIFIER, idValue, 0);

    // Save current token before advancing
    Token savedToken = parser->current_token;  
    accept_token(parser, IDENTIFIER);

    // Check for optional type
    TreeNode *typeNode = NULL;
    if (parser->current_token.type == COLON)
    {
        accept_token(parser, COLON);
        typeNode = parse_type(parser);
    }

    // Check for optional assignment (to distinguish between declaration and initialization)
    if (parser->current_token.type == ASSIGNMENT_OPERATOR)
    {
        root = make_node(AST_INITIALIZATION, NULL, 0);  // Capacity for two children: Identifier and expression/function call
        add_child(root, keywordNode);  
        add_child(root, idNode);
        if (typeNode !=NULL)
            add_child(root, typeNode);
        accept_token(parser, ASSIGNMENT_OPERATOR); // Accept '='

        // Parse the expression or function call
        TreeNode *expressionOrFunctionCallNode = parse_expression_or_func_call(parser, AST_FUNCTION_CALL_IN_ASSIGNMENT);

        add_child(root, expressionOrFunctionCallNode);
        if(typeNode == NULL)
            root->children_count = 3;  
        else
            root->children_count = 4;
    }
    else
    {
        root = make_node(AST_DECLARATION, NULL, 3); // ID, optional Type
        root->children[0] = keywordNode;
        root->children[1] = idNode;
        root->children[2] = typeNode;
        root->children_count = typeNode ? 3 : 2; // If typeNode exists, count is 2, otherwise 1
    }

    return root;
}

// Function to parse expression or function call
TreeNode *parse_expression_or_func_call(Parser *parser, ASTNodeType nodeType)
{
    TreeNode *exprFuncCallNode = NULL;

    if (parser->current_token.type == IDENTIFIER)
    {
        Token save_token = parser->current_token;
        accept_token(parser, IDENTIFIER);

        if (parser->current_token.type == LEFT_PARENTHESIS)
        {
            // Pass the identifier value when parsing a function call
            exprFuncCallNode = parse_function_call(parser, nodeType, save_token.value);
        }
        else
        {
            exprFuncCallNode = get_expression_ast(parser, save_token);
        }
    }
    else
    {
        exprFuncCallNode = get_expression_ast(parser, parser->current_token);
    }

    return exprFuncCallNode;
}

TreeNode *parse_function_call(Parser *parser, ASTNodeType nodeType, const char *identifier)
{
    // Create a new AST node for this function call
    TreeNode *functionCallNode = make_node(nodeType, identifier, 0); // Now using the identifier

    accept_token(parser, LEFT_PARENTHESIS); // Accept '('

    // Parse the arguments and attach them to this function call node
    TreeNode *argumentsNode = parse_arguments(parser);

    // Add argumentsNode to the children of functionCallNode
    add_child(functionCallNode, argumentsNode);

    accept_token(parser, RIGHT_PARENTHESIS); // Accept ')'

    return functionCallNode;
}

TreeNode *parse_assignment_or_function_call(Parser *parser)
{
    // Create a new AST node for this assignment or function call
    TreeNode *assignmentOrFunctionCallNode = NULL;

    // Save the identifier for later use
    Token identifierToken = parser->current_token;
    accept_token(parser, IDENTIFIER); // Accept the identifier

    // Check if it's an assignment or a function call
    if (parser->current_token.type == ASSIGNMENT_OPERATOR)
    {
        // It's an assignment; it can be an expression or a function call
        assignmentOrFunctionCallNode = make_node(AST_ASSIGNMENT, NULL, 2);  // Capacity for two children: Identifier and expression/function call
        TreeNode *idNode = make_node(IDENTIFIER, identifierToken.value, 0); // Create an identifier node with the value

        // Add identifier node as the first child
        add_child(assignmentOrFunctionCallNode, idNode);

        accept_token(parser, ASSIGNMENT_OPERATOR); // Accept '='

        // Parse the expression or function call
        TreeNode *expressionOrFunctionCallNode = parse_expression_or_func_call(parser, AST_FUNCTION_CALL_IN_ASSIGNMENT);

        // Add the expression or function call node as the second child
        add_child(assignmentOrFunctionCallNode, expressionOrFunctionCallNode);
    }
    else if (parser->current_token.type == LEFT_PARENTHESIS)
    {
        // It's a function call
        assignmentOrFunctionCallNode = make_node(AST_FUNCTION_CALL, identifierToken.value, 1); // Capacity for one child: function call arguments

        // Parse the function call and get its arguments
        TreeNode *functionCallNode = parse_function_call(parser, AST_FUNCTION_CALL_IN_ASSIGNMENT, identifierToken.value);

        // Add the function call arguments node as the first child
        add_child(assignmentOrFunctionCallNode, functionCallNode->children[0]);
        free(functionCallNode); // Free the temporary function call nodeparse_function_call(parser, AST_FUNCTION_CALL_IN_ASSIGNMENT);
    }
    else
    {
        // Syntax error: Expected either '=' for assignment or '(' for function call
        handle_error(SYNTACTIC_ERROR);
    }

    return assignmentOrFunctionCallNode;
}

// Function to get ast from expression
TreeNode *get_expression_ast(Parser *parser, Token token)
{
    // Create a new AST node for the expression
    TreeNode *expressionNode = make_node(AST_EXPRESSION, NULL, 1);
    
    // Parse the expression and add it as the first child
    TreeNode *childExpression = parse_expression(parser, token, 0);
    add_child(expressionNode, childExpression);
    
    return expressionNode;
}

TreeNode *parse_type(Parser *parser)
{
    // Create a new AST node for this type
    TreeNode *typeNode = NULL;

    switch (parser->current_token.type)
    {
    case INT_KEYWORD:
        typeNode = make_node(AST_INT_KEYWORD, NULL, 0);
        accept_token(parser, INT_KEYWORD);
        break;
    case INT_NULLABLE_KEYWORD:
        typeNode = make_node(AST_INT_NULLABLE_KEYWORD, NULL, 0);
        accept_token(parser, INT_NULLABLE_KEYWORD);
        break;
    case DOUBLE_KEYWORD:
        typeNode = make_node(AST_DOUBLE_KEYWORD, NULL, 0);
        accept_token(parser, DOUBLE_KEYWORD);
        break;
    case DOUBLE_NULLABLE_KEYWORD:
        typeNode = make_node(AST_DOUBLE_NULLABLE_KEYWORD, NULL, 0);
        accept_token(parser, DOUBLE_NULLABLE_KEYWORD);
        break;
    case STRING_KEYWORD:
        typeNode = make_node(AST_STRING_KEYWORD, NULL, 0);
        accept_token(parser, STRING_KEYWORD);
        break;
    case STRING_NULLABLE_KEYWORD:
        typeNode = make_node(AST_STRING_NULLABLE_KEYWORD, NULL, 0);
        accept_token(parser, STRING_NULLABLE_KEYWORD);
        break;
    default:
        handle_error(SYNTACTIC_ERROR);
        return NULL;
    }

    return typeNode;
}

TreeNode *parse_block(Parser *parser)
{
    // Create a new AST node for this block
    TreeNode *blockNode = make_node(AST_BLOCK, NULL, 1);

    accept_token(parser, LEFT_CURLY_BRACE); // Accept '{'

    if (parser->current_token.type != RIGHT_CURLY_BRACE)
    {
        // Parse the statements inside the block only if the block is not empty
        TreeNode *statementsNode = parse_statements(parser);
        add_child(blockNode, statementsNode);
    }

    accept_token(parser, RIGHT_CURLY_BRACE); // Accept '}'

    return blockNode;
}

TreeNode *parse_while_loop(Parser *parser)
{
    // Create a new AST node for this while loop
    TreeNode *whileNode = make_node(AST_LOOP, NULL, 2);

    // Expecting 'while' keyword
    accept_token(parser, WHILE_KEYWORD);

    // Parse the expression and attach it as the first child of the while node
    TreeNode *expressionNode = get_expression_ast(parser, parser->current_token);
    add_child(whileNode, expressionNode);

    // Parse the block and attach it as the second child of the while node
    TreeNode *blockNode = parse_block(parser);
    add_child(whileNode, blockNode);

    return whileNode;
}

TreeNode *parse_if_cond(Parser *parser)
{
    // Create a new AST node for this if condition with capacity for up to 3 children
    // 1st child is for the condition, 2nd for the if block, 3rd for the optional else block
    TreeNode *ifNode = make_node(AST_CONDITIONAL, NULL, 0);

    // Expecting 'if' keyword
    accept_token(parser, IF_KEYWORD);

    // Parse the expression
    TreeNode *expressionNode;
    if (parser->current_token.type == LET_KEYWORD)
    {
        accept_token(parser, LET_KEYWORD);
        expressionNode = make_node(IDENTIFIER, parser->current_token.value, 0);
        accept_token(parser, IDENTIFIER);
    }
    else
    {
        expressionNode = get_expression_ast(parser, parser->current_token);
    }
    // Attach the expression as the first child
    add_child(ifNode, expressionNode);

    // Parse the block and attach it as the second child
    TreeNode *blockNode = parse_block(parser);
    add_child(ifNode, blockNode);

    // Check for optional 'else' part
    if (parser->current_token.type == ELSE_KEYWORD)
    {
        accept_token(parser, ELSE_KEYWORD);
        TreeNode *elseBlockNode = parse_block(parser);
        TreeNode *elseNode = make_node(AST_ELSE_PART, NULL, 1); // Create a node for the else part
        add_child(elseNode, elseBlockNode); // Attach the else block as the child of elseNode

        // Attach elseNode as the third child of ifNode
        add_child(ifNode, elseNode);
    }

    return ifNode;
}

TreeNode *parse_return_statement(Parser *parser) {
    TreeNode *returnNode = make_node(AST_RETURN_STATEMENT, NULL, 1);
    accept_token(parser, RETURN_KEYWORD);

    if (is_return_value) {  // Check this
        if (is_expression_start_token(look_ahead(0))) {
            TreeNode *expressionOrFuncCallNode = parse_expression_or_func_call(parser, AST_FUNCTION_CALL);
            add_child(returnNode, expressionOrFuncCallNode);
        } else {
            handle_error(SEMANTIC_RETURN_ERROR);
        }
    } else {
        // Handle return in a void function
        if (!is_expression_start_token(look_ahead(0))) {
            handle_error(SEMANTIC_RETURN_ERROR);
        }
    }

    return returnNode;
}

TreeNode *parse_arguments(Parser *parser)
{
    // Create a new AST node for this arguments collection
    TreeNode *argumentsNode = make_node(AST_ARGUMENTS, NULL, 0); // Initialized with zero children

    // Check if there are arguments
    if (parser->current_token.type != RIGHT_PARENTHESIS)
    {
        // Parse the first argument
        TreeNode *firstArgumentNode = parse_argument(parser);

        // Attach the first argument as the first child
        add_child(argumentsNode, firstArgumentNode);

        // Parse more arguments
        while (parser->current_token.type == COMMA)
        {
            accept_token(parser, COMMA); // Accept ','

            // Parse the next argument and attach it as a child
            TreeNode *nextArgumentNode = parse_argument(parser);
            add_child(argumentsNode, nextArgumentNode);
        }
    }

    return argumentsNode;
}

TreeNode *parse_argument(Parser *parser)
{
    // Create a new AST node for this argument
    TreeNode *argumentNode = make_node(AST_ARGUMENT, NULL, 0); // Initialized with zero children

    if (parser->current_token.type == IDENTIFIER)
    {
        // Create a node for the identifier
        TreeNode *idNode = make_node(AST_IDENTIFIER, parser->current_token.value, 0);

        // Attach idNode as a child to the argumentNode
        add_child(argumentNode, idNode);

        accept_token(parser, IDENTIFIER);

        if (parser->current_token.type == COLON)
        {
            accept_token(parser, COLON);

            // Parse the literal or id
            TreeNode *literalOrIdNode = parse_literal_or_id(parser);

            // Attach literalOrIdNode as a child to the argumentNode
            add_child(argumentNode, literalOrIdNode);
        }
    }
    else
    {
        // Parse the literal or id
        TreeNode *literalOrIdNode = parse_literal_or_id(parser);

        // Attach literalOrIdNode as a child to the argumentNode
        add_child(argumentNode, literalOrIdNode);
    }

    return argumentNode; // Return the root of this subtree
}

TreeNode *parse_literal_or_id(Parser *parser)
{
    // Create a new AST node for this literal or id
    TreeNode *literalOrIdNode = NULL;

    switch (parser->current_token.type)
    {
    case STRING_LITERAL:
        literalOrIdNode = make_node(AST_STRING_LITERAL, parser->current_token.value, 0);  // Initialize with zero children
        accept_token(parser, STRING_LITERAL);
        break;
    case INTEGER_LITERAL:
        literalOrIdNode = make_node(AST_INTEGER_LITERAL, parser->current_token.value, 0);  // Initialize with zero children
        accept_token(parser, INTEGER_LITERAL);
        break;
    case DOUBLE_LITERAL:
        literalOrIdNode = make_node(AST_DOUBLE_LITERAL, parser->current_token.value, 0);  // Initialize with zero children
        accept_token(parser, DOUBLE_LITERAL);
        break;
    case IDENTIFIER:
        literalOrIdNode = make_node(AST_IDENTIFIER, parser->current_token.value, 0);  // Initialize with zero children
        accept_token(parser, IDENTIFIER);
        break;
    default:
        handle_error(SYNTACTIC_ERROR);
        return NULL;
    }

    return literalOrIdNode;  // Return the root of this subtree
}

// Function to parse function definitions
TreeNode *parse_function_definitions(Parser *parser){
    TreeNode *funcDefNode = make_node(AST_FUNCTION_DEFINITIONS, NULL, 0);

    while (parser->current_token.type == FUNCTION_KEYWORD) {
        TreeNode *child = parse_function_definition(parser);
        add_child(funcDefNode, child);
    }

    return funcDefNode;
}

// Function to parse a single function definition
TreeNode *parse_function_definition(Parser *parser)
{

    accept_token(parser, FUNCTION_KEYWORD);

    TreeNode *funcDefNode = make_node(AST_FUNCTION_DEFINITION, parser->current_token.value, 0);

    accept_token(parser, IDENTIFIER);

    accept_token(parser, LEFT_PARENTHESIS);
    
    TreeNode *paramsNode = parse_parameters(parser);
    add_child(funcDefNode, paramsNode);

    accept_token(parser, RIGHT_PARENTHESIS);

    if (parser->current_token.type == ARROW) {
        accept_token(parser, ARROW);
        TreeNode *returnTypeNode = parse_type(parser);
        add_child(funcDefNode, returnTypeNode);
        is_return_value = true;
    }
    else
        is_return_value = false;

    TreeNode *blockNode = parse_block(parser);
    add_child(funcDefNode, blockNode);

    return funcDefNode;
}

// Function to parse parameters
TreeNode *parse_parameters(Parser *parser)
{
    TreeNode *paramsNode = make_node(AST_PARAMETERS, NULL, 0);

    while (parser->current_token.type == IDENTIFIER || parser->current_token.type == UNDERSCORE) {
        TreeNode *paramNode = parse_parameter(parser);
        add_child(paramsNode, paramNode);

        if (parser->current_token.type == COMMA) {
            accept_token(parser, COMMA);
        } else {
            break;
        }
    }

    return paramsNode;
}

// Function to parse a single parameter
TreeNode *parse_parameter(Parser *parser)
{
    TreeNode *paramNode = make_node(AST_PARAMETER, NULL, 0);
    
    TreeNode *nameNode = make_node(AST_PARAMETER_NAME, NULL, 0);

    if (parser->current_token.type == IDENTIFIER) {
        TreeNode *externalNameNode = make_node(AST_EXTERNAL_NAME, parser->current_token.value, 0);
        add_child(nameNode, externalNameNode);
        accept_token(parser, IDENTIFIER);
    } else {
        TreeNode *underscoreNode = make_node(AST_UNDERSCORE, NULL, 0);
        add_child(nameNode, underscoreNode);
        accept_token(parser, UNDERSCORE);
    }

    TreeNode *internalNameNode = make_node(AST_INTERNAL_NAME, parser->current_token.value, 0);
    add_child(nameNode, internalNameNode);
    accept_token(parser, IDENTIFIER);

    add_child(paramNode, nameNode);

    accept_token(parser, COLON);

    TreeNode *typeNode = parse_type(parser);
    add_child(paramNode, typeNode);

    return paramNode;
}