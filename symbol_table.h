/**
 * @file symbol_table.h
 * @brief Symbol Table Header File
 *
 * This file provides the data structures and function prototypes for the symbol table.
 *
 * @author Vladimir Azarov
 */

#ifndef SYMBOL_TABLE_H 
#define SYMBOL_TABLE_H 

#include "lexer.h"
#include "stdbool.h"
#include "stdlib.h"

// Structure to represent a function parameter
typedef struct Parameter {
    // Parameter name
    char *name;
    
    // Parameter type
    char *type;
    
    // Pointer to the next parameter in the list
    struct Parameter *next;
} Parameter;

// Structure to hold information about a function
typedef struct FunctionInfo {
    
    size_t func_pos; //0 for built-in functions
    size_t func_body;
    size_t func_end;

    // Number of parameters
    int parameterCount;
    
    // Return type of the function
    char *returnType;
    
    // List of parameters
    Parameter *parameters;
} FunctionInfo;

typedef enum {
    GLOBAL,
    LOCAL,
    FUNCTION
} Scope;

typedef enum {
    // For 'let'
    CONSTANT,  
    // For 'var'
    VARIABLE   
} Modifier;

typedef struct {
    // Name of the identifier
    char *name;          
    // Type and value 
    token_t *data; 
    // Must be set to true if variable was defined
    bool is_defined;
    // Scope of the identifier (global, local, or within a function)
    Scope scope;         
    // Constant of variable (can be modifed or not) 
    Modifier modifier;   
    // Information about the function (NULL if the symbol is not a function)
    FunctionInfo *functionInfo;
} Symbol;

typedef struct AVLNode {
    Symbol symbol;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct SymbolTable {
    AVLNode *root;
} SymbolTable;

SymbolTable* initSymbolTable();
void insertSymbol(SymbolTable* table, Symbol symbol);
Symbol* findSymbol(SymbolTable* table, char* name);
void deleteSymbol(SymbolTable* table, char* name);
Symbol createSymbol(char *name, token_t *data, Scope scope, Modifier modifier, FunctionInfo *functionInfo);
void freeSymbolTable(SymbolTable* table);
#endif // SYMBOL_TABLE_H 