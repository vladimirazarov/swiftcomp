#ifndef LEXER_H 
#define LEXER_H 
typedef enum{           //lex type
    identifiertoken,
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
    stringtoken,             //"abc"
    multilinestring,    //"""foobar"""
    doubletoken,            //13.56/E/e
    nil,                //nil
    colon,              //:
    funreturn,          //return
    funelse,            //else
    function,           //func
    funif,              //if
    funwhile,           //while
    varkw,              //var keyword
    letkw,              //let keyword
    arrow,
    eof,       
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
Token make_token(TokenType typ, char* hodnota);


#endif // LEXER_H 