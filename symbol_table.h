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
    Token *data; 
    // Must be set to true if variable was defined
    bool is_defined;
    // Scope of the identifier (global, local, or within a function)
    Scope scope;         
    // Constant of variable (can be modifed or not) 
    Modifier modifier;   
    // Information about the function (NULL if the symbol is not a function)
    FunctionInfo *functionInfo;
    // Nullable or not 
    bool isNullable;
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

SymbolTable* init_symbol_table();
void insert_symbol(SymbolTable* table, Symbol symbol);
Symbol* find_symbol(SymbolTable* table, char* name);
void delete_symbol(SymbolTable* table, char* name);
Symbol create_symbol(char *name, Token *data, Scope scope, Modifier modifier, FunctionInfo *functionInfo, bool isNullable);
void free_symbol_table(SymbolTable* table);
#endif // SYMBOL_TABLE_H 