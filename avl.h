#ifndef AVL_H
#define AVL_H

typedef struct index_avl{
    char *string;
    int position;
} index_avl;

typedef struct node_avl{
    index_avl *value;
    int factor;
    struct node_avl *left, *right;
} node_avl;

typedef node_avl* tree_avl;

tree_avl insert_avl(tree_avl root, index_avl value, int *insert_avl_factor);
tree_avl remove_avl(tree_avl root, index_avl value, int *remove_avl_factor);
tree_avl balance_avl(tree_avl root);
tree_avl rotate_simple_right(tree_avl root);
tree_avl rotate_simple_left(tree_avl root);
tree_avl rotate_double_left(tree_avl root);
tree_avl rotate_double_right(tree_avl root);
void avl_search(tree_avl root, char *value, FILE *archive);
tree_avl avl_search_remove(tree_avl root, char *value);
int maxHeight(int a, int b);
tree_avl biggest_smallest_avl(tree_avl root);
int getHeight(tree_avl node);
int getFactor(tree_avl root);
void updateHeight(tree_avl node);
void print_factor_avl(tree_avl root, int type);
void print_avl(tree_avl root, int type);

#endif