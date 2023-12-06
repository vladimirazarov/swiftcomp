/**
 * @file symtable.c
 * @brief Symbol Table Implementation
 *
 * This file contains the implementation of the symbol table. The symbol table is
 * used to store and manage symbols, which represent variables, constants, and
 * functions. The table is implemented using an AVL tree.
 *
 * @author Vladimir Azarov
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "expressions.h"
#include "symbol_table.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

Symbol create_symbol(char *name, EvaluatedExpressionData data, bool is_initialized, Modifier modifier, FunctionInfo *functionInfo, bool is_nullable) {
    Symbol sym = {0};
    sym.name = strdup(name);
    sym.type_and_value = data;  
    sym.is_initialized = is_initialized; 
    sym.modifier = modifier;
    sym.functionInfo = functionInfo;
    sym.isNullable = is_nullable;
    return sym;
}

SymbolTable* init_symbol_table(){
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!table) {
        return NULL;
    }
    table->root = NULL;
    return table;
}

int height(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

int get_balance(AVLNode* node){
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

AVLNode* min_value_node(AVLNode* root) {
    AVLNode* current = root;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T3 = x->right;

    x->right = y;
    y->left = T3;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

AVLNode* insert_node(AVLNode* node, char* name, Symbol symbol) {
    if (node == NULL) {
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        newNode->symbol = symbol;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (strcmp(name, node->symbol.name) < 0) {
        node->left = insert_node(node->left, name, symbol);
    } else if (strcmp(name, node->symbol.name) > 0) {
        node->right = insert_node(node->right, name, symbol);
    } else {
        return node; 
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    if (balance > 1 && strcmp(name, node->left->symbol.name) < 0) {
        return right_rotate(node);
    }

    if (balance < -1 && strcmp(name, node->right->symbol.name) > 0) {
        return left_rotate(node);
    }

    if (balance > 1 && strcmp(name, node->left->symbol.name) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    if (balance < -1 && strcmp(name, node->right->symbol.name) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

Symbol* search(AVLNode* root, char* name) {
    while (root != NULL) {
        if (strcmp(name, root->symbol.name) == 0) {
            return &(root->symbol);
        }
        if (strcmp(name, root->symbol.name) < 0) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL;
}

AVLNode* delete_node(AVLNode* root, char* name) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(name, root->symbol.name) < 0) {
        root->left = delete_node(root->left, name);
    } else if (strcmp(name, root->symbol.name) > 0) {
        root->right = delete_node(root->right, name);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            AVLNode* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }

            free(temp);
        } else {
            AVLNode* temp = min_value_node(root->right);
            root->symbol = temp->symbol;
            root->right = delete_node(root->right, temp->symbol.name);
        }
    }

    if (root == NULL) {
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = get_balance(root);


    if (balance > 1 && get_balance(root->left) >= 0) {
        return right_rotate(root);
    }

    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0) {
        return left_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void insert_symbol(SymbolTable* table, Symbol symbol) {
    if (!table) return;
    table->root = insert_node(table->root, symbol.name, symbol);
}

// Recursive function to search through each SymbolTable in the stack
Symbol* find_in_table(SymStackNode *node, char* name) {
    if (!node) return NULL; // Reached the bottom of the stack

    Symbol* symbol = search(node->table->root, name);
    if (symbol) return symbol; // Found the symbol in the current table

    return find_in_table(node->next, name); // Recursively search in the next table
}

// Function to search only in the top SymbolTable of the stack
Symbol* find_in_current_table(SymStackNode *top_node, char* name) {
    if (!top_node) return NULL; // Stack is empty or no tables present

    // Only search in the top table without recursing further
    return search(top_node->table->root, name);
}

Symbol* find_symbol(Stack *stack, char* name) {
    if (!stack) return NULL;
    return find_in_table(stack->top, name);
}
void delete_symbol(SymbolTable* table, char* name) {
    if (!table) return;
    table->root = delete_node(table->root, name);
}

void update_symbol_value(SymbolTable *table, char *name, SymbolValueU new_value) {
    AVLNode *node = search(table->root, name);
    
    if (node) {
        node->symbol.type_and_value.value = new_value;
    } else {
        handle_error(SEMANTIC_UNDEF_ERROR);
    }
}

void free_node(AVLNode* node) {
    if (node == NULL) {
        return;
    }

    free_node(node->left);
    free_node(node->right);

    free(node);
}

void free_symbol_table(SymbolTable* table) {
    if (table == NULL) {
        return;
    }

    free_node(table->root);

    free(table);
}