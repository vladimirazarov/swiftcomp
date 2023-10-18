// TODO:      3. Implementace pravidel
// TODO:      Popremyslet o tom ze nemuzue definovat funkci pokud jsme jiz uvnitr funkce (semantika/syntax?)

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

bool is_return_value;

// Function to create a new parser
Parser *create_parser()
{
    Parser *parser = malloc(sizeof(Parser));
    if (!parser)
    {
        handle_error(INTERNAL_ERROR);
    }
    parser->current_token = get_token();
    return parser;
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

void advance_token(Parser *parser)
{
    // This function should update parser->current_token to the next token in the stream
    parser->current_token = get_token();
}


// Function to parse a program
TreeNode *parse_program(Parser *parser)
{
    // Create the root node for the AST
    TreeNode* root = make_node(AST_PROGRAM, NULL);

    if (parser->current_token.type != END_OF_FILE)
    {
        // Parse the section and attach it to the root
        TreeNode* sectionNode = parse_section(parser);
        root->left = sectionNode;  // 'left' is used for the main child

        if (parser->current_token.type != END_OF_FILE)
        {
            // If not EOF after parsing section, it's a syntax error
            handle_error(SYNTACTIC_ERROR);
        }
    }
    else
    {
        // If EOF at the beginning, it's a syntax error
        handle_error(SYNTACTIC_ERROR);
    }
    return root;
}

TreeNode* parse_section(Parser *parser) {
    // Create a new AST node for this section
    TreeNode* sectionNode = make_node(AST_SECTION, NULL);

    // Parse statements and attach them to this section node
    TreeNode* statementsNode = parse_statements(parser);
    sectionNode->left = statementsNode;

    // Parse function definitions and attach them to this section node
    TreeNode* functionDefinitionsNode = parse_function_definitions(parser);
    sectionNode->right = functionDefinitionsNode;

    // If there are more sections, recursively parse them
    if (parser->current_token.type != END_OF_FILE) {
        TreeNode* nextSectionNode = parse_section(parser);
        sectionNode->next = nextSectionNode;
    }

    return sectionNode;
}

// Function to parse statements
TreeNode* parse_statements(Parser *parser)
{
    TreeNode* head = NULL; // The first statement node
    TreeNode* tail = NULL; // The last statement node

    while (parser->current_token.type != END_OF_FILE &&
           parser->current_token.type != FUNCTION_KEYWORD)
    {
        TreeNode* stmtNode = NULL; // The current statement node

        switch (parser->current_token.type)
        {
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
            // End of block, return the linked list of statements
            return head;
        default:
            handle_error(SYNTACTIC_ERROR);
            return NULL;
        }

        // Add the statement node to the linked list
        if (head == NULL) {
            head = stmtNode;
            tail = stmtNode;
        } else {
            tail->next = stmtNode;
            tail = stmtNode;
        }
    }

    return head;
}

TreeNode* parse_declaration_or_initialization(Parser *parser)
{
    TokenType keyword = parser->current_token.type; // Store the keyword (either VAR_KEYWORD or LET_KEYWORD)
    accept_token(parser, keyword); // Accept 'var' or 'let'

    // Accept the identifier and store its value
    char* idValue = parser->current_token.value;
    accept_token(parser, IDENTIFIER); // Accept the identifier

    TreeNode *typeNode = NULL;
    TreeNode *exprFuncCallNode = NULL;

    // Check for optional type
    if (parser->current_token.type == COLON)
    {
        accept_token(parser, COLON); // Accept ':'
        typeNode = parse_type(parser); // Parse the type and get its node
    }

    // Check for optional expression or function call
    if (parser->current_token.type == ASSIGNMENT_OPERATOR)
    {
        accept_token(parser, ASSIGNMENT_OPERATOR); // Accept '='
        exprFuncCallNode = parse_expression_or_func_call(parser); // Parse the expression or function call and get its node
    }

    // Determine the type of the node (Declaration or Initialization)
    ASTNodeType nodeType = (exprFuncCallNode != NULL) ? AST_INITIALIZATION : AST_DECLARATION;

    // Create the appropriate AST node
    TreeNode* declInitNode = make_node(nodeType, NULL);
    TreeNode* keywordNode = make_node(keyword, NULL);
    TreeNode* idNode = make_node(IDENTIFIER, idValue);

    // Link them together
    declInitNode->left = keywordNode;
    keywordNode->next = idNode;
    idNode->next = typeNode;
    if (typeNode) {
        typeNode->next = exprFuncCallNode;
    } else {
        idNode->next = exprFuncCallNode;
    }

    return declInitNode; // Return the root of this subtree
}


// Function to parse expression or function call
TreeNode* parse_expression_or_func_call(Parser *parser)
{
    TreeNode* exprFuncCallNode = NULL;

    if (parser->current_token.type == IDENTIFIER)
    {
        Token save_token = parser->current_token;
        accept_token(parser, IDENTIFIER);

        if (parser->current_token.type == LEFT_PARENTHESIS)
        {
            exprFuncCallNode = parse_function_call(parser);
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

// Function to parse function call
TreeNode* parse_function_call(Parser *parser)
{
    // Create a new AST node for this function call
    TreeNode* functionCallNode = make_node(AST_FUNCTION_CALL, NULL);

    accept_token(parser, LEFT_PARENTHESIS);  // Accept '('

    // Parse the arguments and attach them to this function call node
    TreeNode* argumentsNode = parse_arguments(parser);
    functionCallNode->left = argumentsNode;

    accept_token(parser, RIGHT_PARENTHESIS); // Accept ')'

    return functionCallNode;
}

// Function to parse assignment or function call
TreeNode* parse_assignment_or_function_call(Parser *parser)
{
    // Create a new AST node for this assignment or function call
    TreeNode* assignmentOrFunctionCallNode = NULL;

    // Save the identifier for later use
    Token identifierToken = parser->current_token;
    accept_token(parser, IDENTIFIER); // Accept the identifier

    // Check if it's an assignment or a function call
    if (parser->current_token.type == ASSIGNMENT_OPERATOR)
    {
        // It's an assignment, can be an expression or a function call
        assignmentOrFunctionCallNode = make_node(AST_ASSIGNMENT, NULL); // No value for the assignment node itself
        TreeNode* idNode = make_node(IDENTIFIER, identifierToken.value); // Create an identifier node with the value
        assignmentOrFunctionCallNode->left = idNode; // Attach the identifier node as the left child

        accept_token(parser, ASSIGNMENT_OPERATOR); // Accept '='

        // Parse the expression or function call and attach it to this assignment node
        TreeNode* expressionOrFunctionCallNode = parse_expression_or_func_call(parser);
        assignmentOrFunctionCallNode->right = expressionOrFunctionCallNode;
    }
    else if (parser->current_token.type == LEFT_PARENTHESIS)
    {
        // It's a function call
        assignmentOrFunctionCallNode = make_node(AST_FUNCTION_CALL, identifierToken.value);
        
        // Parse the function call and attach its arguments to this function call node
        TreeNode* functionCallNode = parse_function_call(parser);
        assignmentOrFunctionCallNode->left = functionCallNode->left; // Assuming functionCallNode->left contains arguments
        free(functionCallNode); // Free the temporary function call node
    }
    else
    {
        // Syntax error: Expected either '=' for assignment or '(' for function call
        handle_error(SYNTACTIC_ERROR);
    }

    return assignmentOrFunctionCallNode;
}

// Function to get ast from expression
TreeNode* get_expression_ast(Parser *parser, Token token){

    TreeNode* expressionNode = make_node(AST_EXPRESSION, NULL);
    expressionNode->left = parse_expression(parser, token, 0);
    expressionNode->right = NULL;
    //print_tree(expression); //Used for expression testing
    return expressionNode;
}

// Function to parse type
TreeNode* parse_type(Parser *parser)
{
    // Create a new AST node for this type
    TreeNode* typeNode = NULL;

    switch (parser->current_token.type)
    {
    case INT_KEYWORD:
        typeNode = make_node(AST_INT_KEYWORD, NULL);
        accept_token(parser, INT_KEYWORD);
        break;
    case INT_NULLABLE_KEYWORD:
        typeNode = make_node(AST_INT_NULLABLE_KEYWORD, NULL);
        accept_token(parser, INT_NULLABLE_KEYWORD);
        break;
    case DOUBLE_KEYWORD:
        typeNode = make_node(AST_DOUBLE_KEYWORD, NULL);
        accept_token(parser, DOUBLE_KEYWORD);
        break;
    case DOUBLE_NULLABLE_KEYWORD:
        typeNode = make_node(AST_DOUBLE_NULLABLE_KEYWORD, NULL);
        accept_token(parser, DOUBLE_NULLABLE_KEYWORD);
        break;
    case STRING_KEYWORD:
        typeNode = make_node(AST_STRING_KEYWORD, NULL);
        accept_token(parser, STRING_KEYWORD);
        break;
    case STRING_NULLABLE_KEYWORD:
        typeNode = make_node(AST_STRING_NULLABLE_KEYWORD, NULL);
        accept_token(parser, STRING_NULLABLE_KEYWORD);
        break;
    default:
        handle_error(SYNTACTIC_ERROR);
        return NULL;
    }

    return typeNode;
}

// Function to parse a block
TreeNode* parse_block(Parser *parser)
{
    // Create a new AST node for this block
    TreeNode* blockNode = make_node(AST_BLOCK, NULL);

    accept_token(parser, LEFT_CURLY_BRACE);  // Accept '{'
    
    if (parser->current_token.type != RIGHT_CURLY_BRACE) {
        // Parse the statements inside the block only if the block is not empty
        TreeNode* statementsNode = parse_statements(parser);
        blockNode->left = statementsNode;
    }
    
    accept_token(parser, RIGHT_CURLY_BRACE); // Accept '}'

    return blockNode;
}

// Function to parse a while loop
TreeNode* parse_while_loop(Parser *parser)
{
    // Create a new AST node for this while loop
    TreeNode* whileNode = make_node(AST_LOOP, NULL);

    // Expecting 'while' keyword
    accept_token(parser, WHILE_KEYWORD);

    // Parse the expression and attach it as the left child of the while node
    TreeNode* expressionNode = get_expression_ast(parser, parser->current_token);
    whileNode->left = expressionNode;

    // Parse the block and attach it as the right child of the while node
    TreeNode* blockNode = parse_block(parser);
    whileNode->right = blockNode;

    return whileNode;
}

// Function to parse an if condition
TreeNode* parse_if_cond(Parser *parser)
{
    // Create a new AST node for this if condition
    TreeNode* ifNode = make_node(AST_CONDITIONAL, NULL);

    // Expecting 'if' keyword
    accept_token(parser, IF_KEYWORD);

    // Parse the expression and attach it as the left child of the if node
    TreeNode* expressionNode;
    if (parser->current_token.type == LET_KEYWORD)
    {
        accept_token(parser, LET_KEYWORD);
        accept_token(parser, IDENTIFIER);
        // Create a node for the expression involving 'let'
        expressionNode = make_node(AST_EXPRESSION_OR_ID, NULL);
        // Attach it to the ifNode
        ifNode->left = expressionNode;
    }
    else
    {
        expressionNode = get_expression_ast(parser, parser->current_token);
        ifNode->left = expressionNode;
    }

    // Parse the block and attach it as the right child of the if node
    TreeNode* blockNode = parse_block(parser);
    ifNode->right = blockNode;

    // Check for optional 'else' part
    if (parser->current_token.type == ELSE_KEYWORD)
    {
        accept_token(parser, ELSE_KEYWORD);
        TreeNode* elseNode = make_node(AST_ELSE_PART, NULL);
        TreeNode* elseBlockNode = parse_block(parser);
        elseNode->right = elseBlockNode;  // Attach the else block to the right child of elseNode
        ifNode->right->next = elseNode;  // Attach elseNode to the next of the if block
    }

    return ifNode;
}

// Function to parse a return statement
TreeNode* parse_return_statement(Parser *parser)
{
    // Create a new AST node for this return statement
    TreeNode* returnNode = make_node(AST_RETURN_STATEMENT, NULL);

    // Expecting 'return' keyword
    accept_token(parser, RETURN_KEYWORD);

    // Check for optional expression
    if (is_return_value)
    {
        TreeNode* expressionNode = get_expression_ast(parser, parser->current_token);
        returnNode->left = expressionNode;
    }

    return returnNode;
}

// Function to parse arguments
TreeNode* parse_arguments(Parser *parser)
{
    TreeNode* head = NULL; // The first argument node
    TreeNode* tail = NULL; // The last argument node

    // Check if there are arguments
    if (parser->current_token.type != RIGHT_PARENTHESIS)
    {
        // Parse the first argument
        TreeNode* firstArgumentNode = parse_argument(parser);
        head = firstArgumentNode;
        tail = firstArgumentNode;

        // Parse more arguments
        while (parser->current_token.type == COMMA)
        {
            accept_token(parser, COMMA); // Accept ','
            
            // Parse the next argument and attach it to the tail
            TreeNode* nextArgumentNode = parse_argument(parser);
            tail->next = nextArgumentNode;
            tail = nextArgumentNode;
        }
    }

    return head;
}

// Function to parse an individual argument
TreeNode* parse_argument(Parser *parser)
{
    // Create a new AST node for this argument
    TreeNode* argumentNode = make_node(AST_ARGUMENT, NULL);

    if (parser->current_token.type == IDENTIFIER)
    {
        // Create a node for the identifier
        TreeNode* idNode = make_node(AST_IDENTIFIER, parser->current_token.value);
        argumentNode->left = idNode; // Attach the idNode to the left of the argumentNode

        accept_token(parser, IDENTIFIER);

        if (parser->current_token.type == COLON)
        {
            accept_token(parser, COLON);

            // Parse the literal or id and attach it to the right of the argumentNode
            TreeNode* literalOrIdNode = parse_literal_or_id(parser);
            argumentNode->right = literalOrIdNode;
        }
    }
    else
    {
        // Parse the literal or id and attach it to the left of the argumentNode
        TreeNode* literalOrIdNode = parse_literal_or_id(parser);
        argumentNode->left = literalOrIdNode;
    }

    return argumentNode; // Return the root of this subtree
}

// Function to parse a literal or id
TreeNode* parse_literal_or_id(Parser *parser)
{
    // Create a new AST node for this literal or id
    TreeNode* literalOrIdNode = NULL;

    switch (parser->current_token.type)
    {
    case STRING_LITERAL:
        literalOrIdNode = make_node(AST_STRING_LITERAL, parser->current_token.value);
        accept_token(parser, STRING_LITERAL);
        break;
    case INTEGER_LITERAL:
        literalOrIdNode = make_node(AST_INTEGER_LITERAL, parser->current_token.value);
        accept_token(parser, INTEGER_LITERAL);
        break;
    case DOUBLE_LITERAL:
        literalOrIdNode = make_node(AST_DOUBLE_LITERAL, parser->current_token.value);
        accept_token(parser, DOUBLE_LITERAL);
        break;
    case IDENTIFIER:
        literalOrIdNode = make_node(AST_IDENTIFIER, parser->current_token.value);
        accept_token(parser, IDENTIFIER);
        break;
    default:
        handle_error(SYNTACTIC_ERROR);
        return NULL;
    }

    return literalOrIdNode; // Return the root of this subtree
}

// Function to parse function definitions
TreeNode* parse_function_definitions(Parser *parser)
{
    TreeNode* head = NULL; // The first function definition node
    TreeNode* tail = NULL; // The last function definition node

    while (parser->current_token.type == FUNCTION_KEYWORD)
    {
        TreeNode* funcDefNode = parse_function_definition(parser);

        // Add the function definition node to the linked list
        if (head == NULL) {
            head = funcDefNode;
            tail = funcDefNode;
        } else {
            tail->next = funcDefNode;
            tail = funcDefNode;
        }
    }

    return head;
}

// Function to parse a single function definition
TreeNode* parse_function_definition(Parser *parser)
{
    // Create a new AST node for this function definition
    TreeNode* funcDefNode = make_node(AST_FUNCTION_DEFINITION, NULL);

    // Expecting 'func' keyword
    accept_token(parser, FUNCTION_KEYWORD);

    // Expecting function identifier
    TreeNode* idNode = make_node(AST_IDENTIFIER, parser->current_token.value);
    funcDefNode->left = idNode;
    TreeNode* lastNode = idNode;
    accept_token(parser, IDENTIFIER);

    // Expecting '('
    accept_token(parser, LEFT_PARENTHESIS);

    // Parse parameters
    TreeNode* paramsNode = parse_parameters(parser);
    lastNode->next = paramsNode;
    while (paramsNode && paramsNode->next != NULL) {
        paramsNode = paramsNode->next;
    }
    lastNode = paramsNode;

    // Expecting ')'
    accept_token(parser, RIGHT_PARENTHESIS);

    is_return_value = false;

    // Check for optional return type
    if (parser->current_token.type == ARROW)
    {
        accept_token(parser, ARROW); // Accept '->'
        TreeNode* returnTypeNode = parse_type(parser); // Parse the return type
        lastNode->next = returnTypeNode;
        lastNode = returnTypeNode;
        is_return_value = true; 
    }

    lastNode->next = NULL;
    // Parse the block
    TreeNode* blockNode = parse_block(parser);
    funcDefNode->right = blockNode;

    return funcDefNode;
}

// Function to parse parameters
TreeNode* parse_parameters(Parser *parser)
{
    TreeNode* head = NULL; // The first parameter node
    TreeNode* tail = NULL; // The last parameter node

    while (parser->current_token.type == IDENTIFIER || parser->current_token.type == UNDERSCORE)
    {
        TreeNode* paramNode = parse_parameter(parser); // The current parameter node

        // Add the parameter node to the linked list
        if (head == NULL) {
            head = paramNode;
            tail = paramNode;
        } else {
            tail->next = paramNode;
            tail = paramNode;
        }

        // Check for more parameters
        if (parser->current_token.type == COMMA)
        {
            // Expecting ','
            accept_token(parser, COMMA);
        }
        else
        {
            break; // No more parameters
        }
    }

    return head;
}

// Function to parse a single parameter
TreeNode* parse_parameter(Parser *parser)
{
    // Create a new AST node for this parameter
    TreeNode* paramNode = make_node(AST_PARAMETER, NULL);

    // Parse the external name (either an identifier or an underscore)
    TreeNode* externalNameNode = NULL;
    if (parser->current_token.type == IDENTIFIER)
    {
        externalNameNode = make_node(AST_EXTERNAL_NAME, parser->current_token.value);
        accept_token(parser, IDENTIFIER);
    }
    else if (parser->current_token.type == UNDERSCORE)
    {
        externalNameNode = make_node(AST_UNDERSCORE, NULL);
        accept_token(parser, UNDERSCORE);
    }

    // Parse the internal name (identifier)
    TreeNode* internalNameNode = make_node(AST_INTERNAL_NAME, parser->current_token.value);
    accept_token(parser, IDENTIFIER);

    // Combine external and internal names into a single node
    TreeNode* nameNode = make_node(AST_PARAMETER_NAME, NULL);
    nameNode->left = externalNameNode;
    nameNode->right = internalNameNode;
    paramNode->left = nameNode;

    // Expecting ':'
    accept_token(parser, COLON);

    // Parse the type
    TreeNode* typeNode = parse_type(parser);
    paramNode->right = typeNode;  

    return paramNode;
}

// Main function for testing
//int main()
//{
    //Parser *parser = create_parser();
    //TreeNode *program = parse_program(parser);
    //free_parser(parser);
    //return 0;
//}