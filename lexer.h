#ifndef LEXER_H 
#define LEXER_H 
typedef enum{
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
} token_type;             //lex type
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
} state;

typedef struct token {
  token_type type;
  char* value;
} token_t;


/*
token_t get_token();
token_t make_token(token_type typ, char* hodnota);
*/

#endif // LEXER_H 