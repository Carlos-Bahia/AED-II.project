#ifndef BST_H
#define BST_H
#include <stdio.h>

typedef struct index_bst{
    char *author;
    int position;
} index_bst;

typedef struct node_bst{
    index_bst value;
    struct node_bst *left;
    struct node_bst *right;
} node_bst;

typedef node_bst* tree_bst;

tree_bst insert_bst(tree_bst root, index_bst value);
tree_bst remove_bst(tree_bst root, index_bst value);
tree_bst biggest_smallest_bst(tree_bst root);
void bst_search(tree_bst root, char *value, FILE *archive);
tree_bst bst_search_remove(tree_bst root, char *value);
void print_bst(tree_bst root, int type);

#endif