/**
 * @file lexer.h
 * @brief Swift Language Lexer.
 *
 * Defines token types, lexer states, and functions for Swift code tokenization.
 * @author: Martin Packa 
 */

#ifndef LEXER_H 
#define LEXER_H 

typedef enum{
    IDENTIFIER,
    LEFT_PARENTHESIS,        // (
    RIGHT_PARENTHESIS,       // )
    LEFT_CURLY_BRACE,        // {
    RIGHT_CURLY_BRACE,       // }
    PLUS_OPERATOR,           // +
    MINUS_OPERATOR,          // -
    MULTIPLICATION_OPERATOR, // *
    DIVISION_OPERATOR,       // /
    LESS_THAN_OPERATOR,      // <
    GREATER_THAN_OPERATOR,   // >
    LESS_THAN_EQUAL_OPERATOR,         // <=
    GREATER_THAN_EQUAL_OPERATOR,      // >=
    SEMICOLON,               // ;
    ASSIGNMENT_OPERATOR,     // =
    EQUALITY_OPERATOR,       // ==
    NOT_EQUAL_OPERATOR,      // !=
    COMMA,                   // ,
    INTEGER_LITERAL,         // 56
    STRING_LITERAL,          // "abc"
    MULTILINE_STRING,        // """foobar"""
    DOUBLE_LITERAL,          // 13.56/E/e
    NIL_LITERAL,             // nil
    COLON,                   // :
    RETURN_KEYWORD,          // return
    ELSE_KEYWORD,            // else
    FUNCTION_KEYWORD,        // func
    IF_KEYWORD,              // if
    WHILE_KEYWORD,           // while
    VAR_KEYWORD,             // var keyword
    LET_KEYWORD,             // let keyword
    ARROW,          // ->
    END_OF_FILE,              // EOF
    STRING_KEYWORD,
    STRING_NULLABLE_KEYWORD,
    INT_KEYWORD,
    INT_NULLABLE_KEYWORD,
    DOUBLE_KEYWORD,
    DOUBLE_NULLABLE_KEYWORD,
    UNDERSCORE,
    EXCLAMATION_MARK = 100,
    DOUBLE_QMARK_OPERATOR,
} TokenType;

typedef enum{       //machine states
    start,
    identifier,
    string,
    stringvalid,
    stringmultiline,
    multilinestrvalid,
    Int,
    ddouble,
    identity1,
    nidentity1,
    Error,
    lt0,
    mt0,
    mins,
    qmark,
} State;

typedef struct token {
  TokenType type;
  char* value;
} Token;

int hextoint(char k);
int pwr(int a, int b);
int countIndentation(char *line);
void formatstring(char *str, int numspaces, int numLines);
void checkdelimiter (char *line);
char* replaceTabsWithSpaces(const char* str);

Token get_token();
Token make_token(TokenType type, char* value);

#endif // LEXER_H 
