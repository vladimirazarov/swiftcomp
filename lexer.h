#ifndef LEXER_H 

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
    INT_KEYWORD,
    DOUBLE_KEYWORD,
    STRING_KEYWORD
} TokenType;

typedef enum{       //machine states
    start,
    identifier,
    string,
    stringvalid,
    stringmultiline,
    dtype,
    Int,
    ddouble,
    identity1,
    identity2,
    nidentity1,
    Error,
    leq,
    lt0,
    mt0,
    meq,
    mins,
} State;

typedef struct token {
  TokenType type;
  char* value;
} Token;

Token get_token();
Token make_token(TokenType type, char* value);

#endif // LEXER_H 
