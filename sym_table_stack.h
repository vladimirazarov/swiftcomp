#ifndef SYM_TABLE_STACK_H
#define SYM_TABLE_STACK_H

#include "symbol_table.h"

// Define a node for the stack
typedef struct {
    SymbolTable *table;
    struct Node *next;
} SymStackNode;

// Define the stack
typedef struct {
    SymStackNode *top;
} Stack;

// Function prototypes

/**
 * Initialize an empty stack.
 * 
 * @return A pointer to the initialized stack.
 */
Stack* init_stack();

/**
 * Push a new table onto the stack.
 * 
 * @param stack The stack to push onto.
 * @param table The symbol table to push.
 */
void push(Stack *stack, SymbolTable *table);

/**
 * Pop a table from the stack.
 * 
 * @param stack The stack to pop from.
 * @return The popped symbol table.
 */
SymbolTable* pop(Stack *stack);

/**
 * Peek at the top of the stack without removing it.
 * 
 * @param stack The stack to peek at.
 * @return The symbol table at the top of the stack.
 */
SymbolTable* peek(Stack *stack);

#endif // SYM_TABLE_STACK_H