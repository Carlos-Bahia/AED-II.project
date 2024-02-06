#ifndef AVL_H
#define AVL_H

typedef struct node_avl{
    int value;
    int factor;
    struct node_avl *left, *right;
} node_avl;

typedef node_avl* tree_avl;

tree_avl insert_avl(tree_avl root, int value, int *insert_avl_factor);
tree_avl remove_avl(tree_avl root, int value, int *remove_avl_factor);
tree_avl balance_avl(tree_avl root);
tree_avl rotate_simple_right(tree_avl root);
tree_avl rotate_simple_left(tree_avl root);
tree_avl rotate_double_left(tree_avl root);
tree_avl rotate_double_right(tree_avl root);
int maxHeight(int a, int b);
tree_avl biggest_smallest_avl(tree_avl root);
int getHeight(tree_avl node);
int getFactor(tree_avl root);
void updateHeight(tree_avl node);
void print_factor_avl(tree_avl root, int type);
void print_avl(tree_avl root, int type);

#endif