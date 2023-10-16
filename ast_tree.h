#ifndef AST_TREE_H
#define AST_TREE_H
#include "lexer.h"

// Structure to represent AST
typedef struct TreeNode {
    TokenType type;
    char* value;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Function prototypes
TreeNode* make_node(TokenType type, char* value);
void free_leaf(TreeNode* leaf);
void print_tree(TreeNode *root);
#endif