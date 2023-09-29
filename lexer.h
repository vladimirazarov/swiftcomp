#ifndef LEXER_H 
#define LEXER_H 
typedef enum{           //lex type
    ID_function,        //bar
    ID_variable,        //following var/let
    type,               //String/String?
    lbracket,           //(
    rbracket,           //)
    lsetbracket,        //{
    rsetbracket,        //}
    plus,               //+
    minus,              //-
    mul,                //*
    division,           ///
    lt,                 //<
    mt,                 //>
    lte,                //<=
    mte,                //>=
    semicolon,          //;
    assign,             //=
    identity,           //==
    nidentity,          //!=
    comma,              //,
    integer,            //56
    string,             //"abc"
    multilinestring,    //"""foobar"""
    ddouble,            //13.56/E/e
    nil,                //nil
    colon,              //:
    funreturn,          //return
    funelse,            //else
    function,           //func
    funif,              //if
    funwhile,           //while
    eof,       
} TokenType; 
            
typedef enum{       //machine states
    start,
    IDfunkcie,
    varlet,
    varname,
    string,
    stringmultiline,
    dtype,
    Int,
    ddouble,
    identity1,
    identity2,
    nidentity1,
    nidentity2,
    Error,
    leq,
    lt0,
    mt0,
    meq,
} State;

typedef struct token {
  TokenType type;
  char* value;
} Token;



Token get_token();
Token make_token(TokenType typ, char* hodnota);


#endif // LEXER_H 