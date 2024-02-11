#ifndef RED_BLACK_H
#define RED_BLACK_H

enum color { RED, BLACK, DOUBLE_BLACK };

typedef int data_type;

typedef struct node_rb {
    data_type value;
    enum color color;
    struct node_rb *left, *right, *father;
} node_rb;

typedef node_rb * tree_rb;

void initialize_rb(tree_rb *root);
void insert_rb(tree_rb *root, data_type value);
void remove_rb(tree_rb *root, data_type value);
void remove_double_black(tree_rb *root, tree_rb node);
tree_rb biggest_smallest_rb(tree_rb *root);
void rebalance_rb(tree_rb *root, tree_rb node);
void balance_rb(tree_rb *root, tree_rb branch);
void rotate_simple_right_rb(tree_rb *root, tree_rb branch);
void rotate_simple_left_rb(tree_rb *root, tree_rb branch);
int is_root(tree_rb branch);
void print_rb(tree_rb root, int type);
tree_rb uncle_rb(node_rb *root);
tree_rb grandfather_rb(node_rb *root);
enum color get_color(tree_rb branch);
tree_rb brother_rb(tree_rb root);
int branch_origin(tree_rb branch);

#endif