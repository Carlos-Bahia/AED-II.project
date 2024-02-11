#include <stdio.h>
#include <stdlib.h>
#include "red-black.h"

tree_rb null_node;

void initialize_rb(tree_rb *root){
    *root = NULL;
    null_node = (tree_rb) malloc(sizeof(struct node_rb));
    null_node->color = DOUBLE_BLACK;
    null_node->value = 0;
    null_node->left = NULL;
    null_node->right = NULL;
}

void insert_rb(tree_rb *root, data_type value){
    tree_rb position, father, new;
    position = *root;
    father = NULL;

    while(position != NULL){
        father = position;
        if(value > position->value){
            position = position->right;
        } else{
            position = position->left;
        }
    }

    new = (tree_rb) malloc(sizeof(struct node_rb));
    new->value = value;
    new->left = NULL;
    new->right = NULL;
    new->father = father;
    new->color = RED;

    if(is_root(new)){
        *root = new;
    } else{
        if(value > father->value){
            father->right = new;
        } else{
            father->left = new;
        }
    }

    balance_rb(root, new);
}

void balance_rb(tree_rb *root, tree_rb branch){
    while(get_color(branch->father) == RED && get_color(branch) == RED){
        if(get_color(uncle_rb(branch)) == RED){
            uncle_rb(branch)->color = BLACK;
            branch->father->color = BLACK;
            grandfather_rb(branch)->color = RED;

            branch = grandfather_rb(branch);
            continue;
        } 
        if(branch_origin(branch) == 1 && branch_origin(branch->father) == 1){
            rotate_simple_right_rb(root, grandfather_rb(branch));
            branch->father->color = BLACK;
            branch->father->right->color = RED;
            continue;
        }
        if(branch_origin(branch) == 0 && branch_origin(branch->father) == 0){
            rotate_simple_left_rb(root, grandfather_rb(branch));
            branch->father->color = BLACK;
            branch->father->left->color = RED;
            continue;
        }
        if(branch_origin(branch) == 1 && branch_origin(branch->father) == 0){
            rotate_simple_right_rb(root, branch->father);
            //Reduct to a simple case
            branch = branch->right;
            continue;
        }
        if(branch_origin(branch) == 0 && branch_origin(branch->father) == 1){
            rotate_simple_right_rb(root, branch->father);
            //Reduct to a simple case
            branch = branch->left;
            continue;
        }
    }
    (*root)->color = BLACK;
}

void remove_rb(tree_rb *root, data_type value){
    tree_rb position = *root;

    while(position != NULL){
        if(value > position->value){
            position = position->right;
        } else if(value < position->value){
            position = position->left;
        } else{
            if(position->left != NULL && position->right != NULL){
                position->value = (biggest_smallest_rb(&position->left))->value;
                remove_rb(&(position->left), position->value);
                break;
            }

            if(position->left == NULL && position->right == NULL){
                if(is_root(position)){
                    *root = NULL;
                    free(position);
                    break;
                }

                if(get_color(position) == RED){
                    if(branch_origin(position)){
                        position->father->left = NULL;
                    } else{
                        position->father->right = NULL;
                    }
                    free(position);
                    break;
                } else{
                    null_node->color = DOUBLE_BLACK;
                    null_node->father = position->father;
                    if(branch_origin(position)){
                        position->father->left = null_node;
                    } else{
                        position->father->right = null_node;
                    }
                    free(position);
                    rebalance_rb(root, null_node);
                    break;
                }
            }
            if(position->left == NULL && position->right != NULL){
                position->right->father = position->father;
                position->right->color = BLACK;
                if(is_root(position)){
                    *root = position->right;
                } else{
                    if(branch_origin(position)){
                        position->father->left = position->right;
                    } else{
                        position->father->right = position->right;
                    }
                }
                free(position);
                break;
            }
            if(position->left != NULL && position->right == NULL){
                position->left->father = position->father;
                position->left->color = BLACK;
                if(is_root(position)){
                    *root = position->left;
                } else{
                    if(branch_origin(position)){
                        position->father->left = position->left;
                    } else{
                        position->father->right = position->left;
                    }
                }
                free(position);
                break;
            }
        }
    }
}

void rebalance_rb(tree_rb *root, tree_rb node){
    if(is_root(node)){
        node->color = BLACK;
        if(node == null_node){
            *root = NULL;
        }
        return;
    }

    if(get_color(node->father) == BLACK && get_color(brother_rb(node)) == RED &&
    get_color(brother_rb(node)->right) == BLACK && get_color(brother_rb(node)->left) == BLACK){

        if(branch_origin(node)){
            rotate_simple_left_rb(root, node->father);
        } else{
            rotate_simple_right_rb(root, node->father);
        }
        grandfather_rb(node)->color = BLACK;
        node->father->color = RED;

        rebalance_rb(root, node);
        return;
    }

    if(get_color(node->father) == BLACK && get_color(brother_rb(node)) == BLACK &&
    get_color(brother_rb(node)->right) == BLACK && get_color(brother_rb(node)->left) == BLACK){

        node->father->color = DOUBLE_BLACK;

        if(branch_origin(node)){
            node->father->right->color = RED;
        } else{
            node->father->left->color = RED;
        }
        remove_double_black(root, node);

        rebalance_rb(root, node->father);
        return;
    }

    if(get_color(node->father) == RED && get_color(brother_rb(node)) == BLACK &&
    get_color(brother_rb(node)->right) == BLACK && get_color(brother_rb(node)->right) == BLACK){
        
        if(branch_origin(node)){
            node->father->right->color = RED;
        } else{
            node->father->left->color = RED;
        }
        remove_double_black(root, node);

        node->father->color = BLACK;
        return;
    }

    if(branch_origin(node)){
        if(get_color(brother_rb(node)) == BLACK && get_color(brother_rb(node)->left) == RED &&
        get_color(brother_rb(node)->right) == BLACK){

            brother_rb(node)->color = RED;
            brother_rb(node)->left->color = BLACK;
            rotate_simple_right_rb(root, brother_rb(node));
            rebalance_rb(root, node);
            return;
        }
        if(get_color(brother_rb(node)) == BLACK && get_color(brother_rb(node)->right) == RED){

            rotate_simple_left_rb(root, node->father);
            remove_double_black(root, node);
            grandfather_rb(node)->color = node->father->color;
            node->father->color = BLACK;
            uncle_rb(node)->color = BLACK;
            return;
        }
    } else{
        if(get_color(brother_rb(node)) == BLACK && get_color(brother_rb(node)->left) == BLACK &&
        get_color(brother_rb(node)->right) == RED){
            brother_rb(node)->color = RED;
            brother_rb(node)->right->color = BLACK;
            rotate_simple_left_rb(root, brother_rb(node));
            rebalance_rb(root, node);
            return;
        }
        if(get_color(brother_rb(node)) == BLACK && get_color(brother_rb(node)->left) == RED){
            rotate_simple_right_rb(root, node->father);
            remove_double_black(root, node);
            grandfather_rb(node)->color = node->father->color;
            node->father->color = BLACK;
            uncle_rb(node)->color = BLACK;
            return;
        }
    }
}

void remove_double_black(tree_rb *root, tree_rb node){
    if(node == null_node){
        if(branch_origin(node)){
            node->father->left = NULL;
        } else{
            node->father->right = NULL;
        }
    } else{
        node->color = BLACK;
    }
}

tree_rb biggest_smallest_rb(tree_rb *root){
    return (*root) ? ((*root)->right ? biggest_smallest_rb(&((*root)->right)) : *root) : *root;
}

void rotate_simple_right_rb(tree_rb *root, tree_rb pivot) {
    tree_rb u, t1;
    u = pivot->left;
    t1 = u->right;

    int pivot_position = branch_origin(pivot);

    pivot->left = t1;
    if (t1 != NULL)
        t1->father = pivot;
    u->right = pivot;
    u->father = pivot->father;
    pivot->father = u;

    if (is_root(u)) 
        *root = u;
    else 
        if (pivot_position)
            u->father->left = u;
        else
            u->father->right = u;
}

void rotate_simple_left_rb(tree_rb *root, tree_rb pivot) {
    tree_rb u, t1;
    u = pivot->right;
    t1 = u->left;

    int pivot_position = branch_origin(pivot);

    pivot->right = t1;
    if (t1 != NULL)
        t1->father = pivot;
    u->left = pivot;
    u->father = pivot->father;
    pivot->father = u;

    if (is_root(u)) 
        *root = u;
    else 
        if (pivot_position)
            u->father->left = u;
        else
            u->father->right = u;
}

int branch_origin(tree_rb branch){
    return (branch->father != NULL && branch->father->left == branch) ? 1 : 0;
}

int is_root(tree_rb branch){
    return (branch->father == NULL) ? 1 : 0;
}

enum color get_color(tree_rb branch){
    return (branch == NULL) ? BLACK : branch->color;
}

tree_rb brother_rb(tree_rb root){
    return (root->father->left == root) ? root->father->right : root->father->left;
}

tree_rb grandfather_rb(node_rb *root){
    if(root == NULL || root->father == NULL)
        return NULL;
    
    return root->father->father;
}

tree_rb uncle_rb(node_rb *root){
    if(root->father->father != NULL){
        if(root->father->father->left == root->father)
            return root->father->father->right;
        
        return root->father->father->left;
    }
    return NULL;
}

void print_rb(tree_rb root, int type){
    if(type == 1){ //PREORDER
        if(root != NULL){
            switch(root->color){
                case RED:
                    printf("\033[1;31m[%d]", root->value);
                    break;
                case BLACK:
                    printf("\033[1;30m[%d]", root->value);
                    break;
                case DOUBLE_BLACK:
                    printf("\033[1;34m[%d]", root->value);
                    break;
            }
            print_rb(root->left, 1);
            print_rb(root->right, 1);
        }
    } else if(type == 2){ //INORDER
        if(root != NULL){
            print_rb(root->left, 2);
            switch(root->color){
                case RED:
                    printf("\033[1;31m[%d]", root->value);
                    break;
                case BLACK:
                    printf("\033[1;30m[%d]", root->value);
                    break;
                case DOUBLE_BLACK:
                    printf("\033[1;34m[%d]", root->value);
                    break;
            }
            print_rb(root->right, 2);
        } 
    } else if(type == 3){ //POSORDER
        if(root != NULL){
            print_rb(root->left, 3);
            print_rb(root->right, 3);
            switch(root->color){
                case RED:
                    printf("\033[1;31m[%d]", root->value);
                    break;
                case BLACK:
                    printf("\033[1;30m[%d]", root->value);
                    break;
                case DOUBLE_BLACK:
                    printf("\033[1;34m[%d]", root->value);
                    break;
            }
        } 
    }
}
