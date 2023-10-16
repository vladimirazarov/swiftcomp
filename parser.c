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
#include "symbol_table.h"
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
    parser->symbol_table = init_symbol_table();
    parser->current_token = get_token();
    return parser;
}

// Function to destroy a parser
void free_parser(Parser *parser)
{
    if (parser)
    {
        free_symbol_table(parser->symbol_table);
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
void parse_program(Parser *parser)
{
    if (parser->current_token.type != END_OF_FILE)
    {
        parse_section(parser);

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
}

// Function to parse a section
void parse_section(Parser *parser)
{
    // Parse statements
    parse_statements(parser);

    // Parse function definitions
    parse_function_definitions(parser);

    // Check if there are more sections to parse
    if (parser->current_token.type != END_OF_FILE)
    {
        // If there are more tokens, it means there might be another section
        parse_section(parser);
    }
}

// Function to parse statements
void parse_statements(Parser *parser)
{
    while (parser->current_token.type != END_OF_FILE &&
           parser->current_token.type != FUNCTION_KEYWORD)
    {
        switch (parser->current_token.type)
        {
        case VAR_KEYWORD:
        case LET_KEYWORD:
            parse_declaration_or_initialization(parser);
            break;
        case IDENTIFIER:
            parse_assignment_or_function_call(parser);
            break;
        case RETURN_KEYWORD:
            parse_return_statement(parser);
            break;
        case WHILE_KEYWORD:
            parse_while_loop(parser);
            break;
        case IF_KEYWORD:
            parse_if_cond(parser);
            break;
        case LEFT_CURLY_BRACE:
            parse_block(parser);
            break;
        case RIGHT_CURLY_BRACE:
            return;
        default:
            handle_error(SYNTACTIC_ERROR);
            return;
        }
    }
}

// Function to parse declaration or initialization
void parse_declaration_or_initialization(Parser *parser)
{
    TokenType keyword = parser->current_token.type; // Store the keyword (either VAR_KEYWORD or LET_KEYWORD)
    accept_token(parser, keyword);                  // Accept 'var' or 'let'

    accept_token(parser, IDENTIFIER); // Accept the identifier

    // Check for optional type
    if (parser->current_token.type == COLON)
    {
        accept_token(parser, COLON); // Accept ':'
        parse_type(parser);          // Parse the type
    }
    // Check for optional expression or function call
    if (parser->current_token.type == ASSIGNMENT_OPERATOR)
    {
        accept_token(parser, ASSIGNMENT_OPERATOR); 
        parse_expression_or_func_call(parser);
    }
}

void parse_expression_or_func_call(Parser *parser)
{
    if (parser->current_token.type == IDENTIFIER)
    {
        Token save_token = parser->current_token;
        accept_token(parser, IDENTIFIER);

        if (parser->current_token.type == LEFT_PARENTHESIS)
        {
            parse_function_call(parser);
        }
        else
        {
            get_expression_ast(parser, save_token);
        }
    }
    else
    {
        get_expression_ast(parser, parser->current_token);
    }
}

void parse_function_call(Parser *parser)
{
    accept_token(parser, LEFT_PARENTHESIS);  // Accept '('
    parse_arguments(parser);                 // Parse the arguments
    accept_token(parser, RIGHT_PARENTHESIS); // Accept ')'
    return;
}
void parse_assignment_or_function_call(Parser *parser)
{
    // Save the identifier for later use
    accept_token(parser, IDENTIFIER); // Accept the identifier

    // Check if it's an assignment or a function call
    if (parser->current_token.type == ASSIGNMENT_OPERATOR)
    {
        // It's an assignment, can be an expression or a function call
        accept_token(parser, ASSIGNMENT_OPERATOR); // Accept '='
        parse_expression_or_func_call(parser);
    }
    else if (parser->current_token.type == LEFT_PARENTHESIS)
    {
        parse_function_call(parser);
    }
    else
    {
        // Syntax error: Expected either '=' for assignment or '(' for function call
        handle_error(SYNTACTIC_ERROR);
    }
}

// Function to get ast from expression
TreeNode* get_expression_ast(Parser *parser, Token token){
    TreeNode* expression = parse_expression(parser, token, 0);
    print_tree(expression); //Used for expression testing
    return expression;
}

// Function to parse type
void parse_type(Parser *parser)
{
    switch (parser->current_token.type)
    {
    case INT_KEYWORD:
        accept_token(parser, INT_KEYWORD);
        break;
    case INT_NULLABLE_KEYWORD:
        accept_token(parser, INT_NULLABLE_KEYWORD);
        break;
    case DOUBLE_KEYWORD:
        accept_token(parser, DOUBLE_KEYWORD);
        break;
    case DOUBLE_NULLABLE_KEYWORD:
        accept_token(parser, DOUBLE_NULLABLE_KEYWORD);
        break;
    case STRING_KEYWORD:
        accept_token(parser, STRING_KEYWORD);
        break;
    case STRING_NULLABLE_KEYWORD:
        accept_token(parser, STRING_NULLABLE_KEYWORD);
        break;
    default:
        handle_error(SYNTACTIC_ERROR);
        return;
    }
}

// Function to parse a block
void parse_block(Parser *parser)
{
    accept_token(parser, LEFT_CURLY_BRACE);  // Accept '{'
    
    if (parser->current_token.type != RIGHT_CURLY_BRACE) {
        parse_statements(parser);  // Parse the statements inside the block only if the block is not empty
    }
    
    accept_token(parser, RIGHT_CURLY_BRACE); // Accept '}'
}

// Function to parse a while loop
void parse_while_loop(Parser *parser)
{
    // Expecting 'while' keyword
    accept_token(parser, WHILE_KEYWORD);

    // Parse the expression
    get_expression_ast(parser, parser->current_token);

    // Parse the block
    parse_block(parser);
}

// Function to parse an if condition
void parse_if_cond(Parser *parser)
{
    // Expecting 'if' keyword
    accept_token(parser, IF_KEYWORD);

    // Parse the expression
    if (parser->current_token.type == LET_KEYWORD)
    {
        accept_token(parser, LET_KEYWORD);
        accept_token(parser, IDENTIFIER);
    }
    else
    {
        get_expression_ast(parser, parser->current_token);
    }

    // Parse the block
    parse_block(parser);

    // Check for optional 'else' part
    if (parser->current_token.type == ELSE_KEYWORD)
    {
        accept_token(parser, ELSE_KEYWORD);
        parse_block(parser);
    }
}

// Function to parse a return statement
void parse_return_statement(Parser *parser)
{
    // Expecting 'return' keyword
    accept_token(parser, RETURN_KEYWORD);

    // Check for optional expression
    if(is_return_value)
    {
        get_expression_ast(parser, parser->current_token);
    }
}

// Function to parse arguments
void parse_arguments(Parser *parser)
{
    // Check if there are arguments
    if (parser->current_token.type != RIGHT_PARENTHESIS)
    {
        // Parse the first argument
        parse_argument(parser);

        // Parse more arguments
        while (parser->current_token.type == COMMA)
        {
            accept_token(parser, COMMA); // Accept ','
            parse_argument(parser);       // Parse the next argument
        }
    }
}

// Function to parse an individual argument
void parse_argument(Parser *parser)
{
    if (parser->current_token.type == IDENTIFIER)
    {
        accept_token(parser, IDENTIFIER);
        if (parser->current_token.type == COLON)
        {
            accept_token(parser, COLON);
            parse_literal_or_id(parser);
        }
    }
    else
    {
    // Parse the literal or id
    parse_literal_or_id(parser);
    }
}

// Function to parse a literal or id
void parse_literal_or_id(Parser *parser)
{
    switch (parser->current_token.type)
    {
    case STRING_LITERAL:
    case INTEGER_LITERAL:
    case DOUBLE_LITERAL:
    case IDENTIFIER:
        accept_token(parser, parser->current_token.type);
        break;
    default:
        handle_error(SYNTACTIC_ERROR);
        break;
    }
}

// Function to parse function definitions
void parse_function_definitions(Parser *parser)
{
    while (parser->current_token.type == FUNCTION_KEYWORD)
    {
        parse_function_definition(parser);
    }
}

// Function to parse a single function definition
void parse_function_definition(Parser *parser)
{
    // Expecting 'func' keyword
    accept_token(parser, FUNCTION_KEYWORD);

    // Expecting function identifier
    accept_token(parser, IDENTIFIER);

    // Expecting '('
    accept_token(parser, LEFT_PARENTHESIS);

    // Parse parameters
    parse_parameters(parser);

    // Expecting ')'
    accept_token(parser, RIGHT_PARENTHESIS);

    is_return_value = false;

    // Check for optional return type
    if (parser->current_token.type == ARROW)
    {
        accept_token(parser, ARROW); // Accept '->'
        parse_type(parser);          // Parse the return type
        is_return_value = true; 
    }

    // Parse the block
    parse_block(parser);
}

// Function to parse parameters
void parse_parameters(Parser *parser)
{
    if (parser->current_token.type == IDENTIFIER || parser->current_token.type == UNDERSCORE)
    {
        parse_parameter(parser);
        parse_more_parameters(parser);
    }
}

// Function to parse a single parameter
void parse_parameter(Parser *parser)
{
    // Parse the name (either an identifier or an underscore)
    if (parser->current_token.type == IDENTIFIER)
    {
        accept_token(parser, IDENTIFIER);
    }
    else if (parser->current_token.type == UNDERSCORE)
    {
        accept_token(parser, UNDERSCORE);
    }

    accept_token(parser, IDENTIFIER);
    // Expecting ':'
    accept_token(parser, COLON);

    // Parse the type
    parse_type(parser);
}

// Function to parse more parameters
void parse_more_parameters(Parser *parser)
{
    while (parser->current_token.type == COMMA)
    {
        // Expecting ','
        accept_token(parser, COMMA);

        // Parse the next parameter
        parse_parameter(parser);
    }
}

// Main function for testing
int main()
{
    Parser *parser = create_parser();
    parse_program(parser);
    free_parser(parser);
    return 0;
}