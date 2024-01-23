#ifndef BST_H
#define BST_H

typedef struct node_bst{
    int value;
    struct node_bst *left;
    struct node_bst *right;
} node_bst;

typedef node_bst* tree_bst;

tree_bst insert_bst(tree_bst root, int value);
tree_bst remove_bst(tree_bst root, int value);
tree_bst biggest_smallest_bst(tree_bst root);
void print_bst(tree_bst root, int type);

#endif