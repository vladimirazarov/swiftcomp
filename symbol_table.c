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
#include "symbol_table.h"
#include "string.h"
#include "stdlib.h"
#define max(a, b) ((a) > (b) ? (a) : (b))

Symbol createSymbol(char *name, token_t *data, Scope scope, Modifier modifier, FunctionInfo *functionInfo) {
    Symbol sym;
    sym.name = name;
    sym.data = data;  
    sym.is_defined = false; 
    sym.scope = scope;
    sym.modifier = modifier;
    sym.functionInfo = functionInfo;
    return sym;
}

SymbolTable* initSymbolTable() {
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

int getBalance(AVLNode* node) {
    if (node == NULL) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

AVLNode* minValueNode(AVLNode* root) {
    AVLNode* current = root;

    while (current->left != NULL) {
        current = current->left;
    }

    return current;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T3 = x->right;

    x->right = y;
    y->left = T3;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

AVLNode* insertNode(AVLNode* node, char* name, Symbol symbol) {
    if (node == NULL) {
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        newNode->symbol = symbol;
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    if (strcmp(name, node->symbol.name) < 0) {
        node->left = insertNode(node->left, name, symbol);
    } else if (strcmp(name, node->symbol.name) > 0) {
        node->right = insertNode(node->right, name, symbol);
    } else {
        return node; 
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && strcmp(name, node->left->symbol.name) < 0) {
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(name, node->right->symbol.name) > 0) {
        return leftRotate(node);
    }

    if (balance > 1 && strcmp(name, node->left->symbol.name) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(name, node->right->symbol.name) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
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

AVLNode* deleteNode(AVLNode* root, char* name) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(name, root->symbol.name) < 0) {
        root->left = deleteNode(root->left, name);
    } else if (strcmp(name, root->symbol.name) > 0) {
        root->right = deleteNode(root->right, name);
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
            AVLNode* temp = minValueNode(root->right);
            root->symbol = temp->symbol;
            root->right = deleteNode(root->right, temp->symbol.name);
        }
    }

    if (root == NULL) {
        return root;
    }

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);


    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void insertSymbol(SymbolTable* table, Symbol symbol) {
    if (!table) return;
    table->root = insertNode(table->root, symbol.name, symbol);
}

Symbol* findSymbol(SymbolTable* table, char* name) {
    if (!table) return NULL;
    return search(table->root, name);
}

void deleteSymbol(SymbolTable* table, char* name) {
    if (!table) return;
    table->root = deleteNode(table->root, name);
}

void updateSymbolValue(SymbolTable *table, char *name, void *newValue) {
    AVLNode *node = search(table->root, name);
    
    if (node) {
        node->symbol.data->value = newValue;
    } else {
        // TODO: err
    }
}

void freeNode(AVLNode* node) {
    if (node == NULL) {
        return;
    }

    freeNode(node->left);
    freeNode(node->right);

    free(node);
}

void freeSymbolTable(SymbolTable* table) {
    if (table == NULL) {
        return;
    }

    freeNode(table->root);

    free(table);
}