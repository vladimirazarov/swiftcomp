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
#include "ast_tree.h"


typedef union SymbolValueU
{
    char *str_value;
    int int_value;
    double double_value;
    bool bool_value;
} SymbolValueU;

typedef enum Type
{
    STRING_SYMBOL_TYPE,
    INT_SYMBOL_TYPE,
    DOUBLE_SYMBOL_TYPE,
    BOOL_SYMBOL_TYPE,
    NIL_SYMBOL_TYPE,
    EMPTY,
    FUNCTION_SYMBOL_TYPE
} ValueType;
// Structure to represent a function parameter
typedef struct Parameter
{
    // Parameter name
    char *name;

    // Parameter identifier
    char *identifier;

    // Parameter type
    ValueType type;

    bool is_nullable;

    // Pointer to the next parameter in the list
    struct Parameter *next;
} Parameter;

// Structure to hold information about a function
typedef struct FunctionInfo
{
    // Number of parameters
    int parameterCount;

    // Return type of the function
    ValueType returnType;

    TreeNode* body;

    // List of parameters
    Parameter *parameters;
} FunctionInfo;

typedef enum
{
    // For 'let'
    CONSTANT,
    // For 'var'
    VARIABLE
} Modifier;



typedef struct EvaluatedExpressionData
{
    ValueType type;
    SymbolValueU value;
} EvaluatedExpressionData;

typedef struct
{
    // Name of the identifier
    char *name;
    // Type and value
    EvaluatedExpressionData type_and_value;
    // Must be set to true if variable was defined
    bool is_initialized;
    // Constant of variable (can be modifed or not)
    Modifier modifier;
    // Information about the function (NULL if the symbol is not a function)
    FunctionInfo *functionInfo;
    // Nullable or not
    bool isNullable;
} Symbol;

typedef struct AVLNode
{
    Symbol symbol;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct SymbolTable
{
    AVLNode *root;
} SymbolTable;

typedef struct SymStackNode {
    SymbolTable *table;
    struct Node *next;
} SymStackNode;

// Define the stack
typedef struct Stack {
    SymStackNode *top;
} Stack;

SymbolTable *init_symbol_table();
void insert_symbol(SymbolTable *table, Symbol symbol);
Symbol *find_symbol(Stack *stack, char *name);
void delete_symbol(SymbolTable *table, char *name);
Symbol create_symbol(char *name, EvaluatedExpressionData data, bool is_initialized, Modifier modifier, FunctionInfo *functionInfo, bool isNullable);
void free_symbol_table(SymbolTable *table);
void update_symbol_value(SymbolTable *table, char *name, SymbolValueU new_value);
void print_symbol_value(EvaluatedExpressionData data);
void print_symbol(Symbol symbol);
void print_avl_node(AVLNode* node);
void print_symbol_table(SymbolTable* table);
Symbol* find_in_table(SymStackNode *node, char* name);
SymbolTable* peek(Stack *stack);
SymbolTable* pop(Stack *stack);
void push(Stack *stack, SymbolTable *table);
Stack* init_stack();

#endif // SYMBOL_TABLE_H