#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"


// Initialize an empty stack
Stack* init_stack() {
    Stack *stack = (Stack*) malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

// Push a new table onto the stack
void push(Stack *stack, SymbolTable *table) {
    SymStackNode *newNode = (SymStackNode*) malloc(sizeof(SymStackNode));
    newNode->table = table;
    newNode->next = stack->top;
    stack->top = newNode;
}

// Pop a table from the stack
SymbolTable* pop(Stack *stack) {
    if (stack->top == NULL) {
        return NULL; // Stack is empty
    }
    SymStackNode *temp = stack->top;
    SymbolTable *table = temp->table;
    stack->top = temp->next;
    free(temp);
    return table;
}

// Peek at the top of the stack without removing it
SymbolTable* peek(Stack *stack) {
    if (stack->top == NULL) {
        return NULL; // Stack is empty
    }
    return stack->top->table;
}