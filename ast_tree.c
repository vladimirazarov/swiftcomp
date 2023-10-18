#include "ast_tree.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

// Make node for expression subtree
TreeNode* make_node(TokenType type, char* value) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->type = type;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_leaf(TreeNode* leaf){
    if (leaf == NULL) {
        return;
    }

    free_leaf(leaf->left);
    free_leaf(leaf->right);

    free(leaf);
}

void print_tree(TreeNode *root) {
    if (root == NULL) {
        return;
    }
    fprintf(stdout, "%s ", root->value);
    print_tree(root->left);
    print_tree(root->right);
}


