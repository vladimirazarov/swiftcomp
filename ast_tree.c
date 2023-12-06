/**
 * @file ast_tree.c
 * @brief Implementation of functions for the Abstract Syntax Tree (AST).
 *
 * This file contains the definitions of functions and data structures used
 * for building and managing an Abstract Syntax Tree, which represents the
 * syntactic structure of source code in a tree-like form.
 * 
 * @author Vladimir Azarov
 */

#include "ast_tree.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>


// Function to create a new AST node
TreeNode* make_node(ASTNodeType type, char* value, int initial_child_capacity) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->type = type;
    node->value = value;
    node->children = NULL;
    node->children_count= initial_child_capacity;

    if (initial_child_capacity > 0) {
        node->children = (TreeNode**)malloc(initial_child_capacity * sizeof(TreeNode*));
        for (int i = 0; i < initial_child_capacity; ++i) {
            node->children[i] = NULL;
        }
    }

    return node;
}

// Function to add a child to an existing AST node
void add_child(TreeNode* parent, TreeNode* child) {
    // Resize array 
    if (parent->children_count % 10 == 0) {
        parent->children = (TreeNode**)realloc(parent->children, (parent->children_count + 10) * sizeof(TreeNode*));
    }
    
    // Add child
    parent->children[parent->children_count] = child;
    parent->children_count++;
}