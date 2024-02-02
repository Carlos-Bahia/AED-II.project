#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

tree_avl insert_avl(tree_avl root, int value){
    if(root == NULL){
        tree_avl new = (tree_avl) malloc(sizeof(node_avl));
        new->value = value;
        new->left = NULL;
        return new;
    }

    if(value < root->value){
        root->left = insert_avl(root->left, value);
    } else if(value >= root->value){
        root->right = insert_avl(root->right, value);
    }

    return balance_avl(root);
}

tree_avl balance_avl(tree_avl root){
    if(root == NULL){
        return NULL;
    }

    if(getFactor(root) == -2){
        if(getFactor(root->left) == 1){
            root->left = rotate_simple_left(root->left);
        }
        root = rotate_simple_right(root);
    } else if(getFactor(root) == 2){
        if(getFactor(root->right) == -1){
            root->right = rotate_simple_right(root->right);
        }
        root = rotate_simple_left(root);
    }

    return root;
}

tree_avl rotate_simple_right(tree_avl root){
    tree_avl temp = root->left;

    root->left = temp->right;
    temp->right = root;

    return temp;
}
tree_avl rotate_simple_left(tree_avl root){
    tree_avl temp = root->right;

    root->right = temp->left;
    temp->left = root;

    return temp;
}

int getHeight(tree_avl root){
    if(root == NULL)
        return 0;
    
    return (maxHeight(getHeight(root->left), getHeight(root->right))+1);
}

int maxHeight(int a, int b){
    return (a > b) ? a : b;
}

int getFactor(tree_avl root){
    if(root == NULL)
        return 0;

    return(getHeight(root->right) - getHeight(root->left));
}

tree_avl remove_avl(tree_avl root, int value){
    if(root == NULL){
        return root;
    }

    if(value < root->value){
        root->left = remove_avl(root->left,value);
    }
    if(value > root->value){
        root->right = remove_avl(root->right, value);
    }

    if(value == root->value){
        if((root->left == NULL && root->right == NULL) || root->left == NULL || root->right == NULL){
            if(root->right != NULL){
                tree_avl temp = root;
                root = root->right;
                free(temp);
            } else if (root->left != NULL) {
                tree_avl temp = root;
                root = root->left;
                free(temp);
            } else{
                root = NULL;
            }
        }
        else{
            tree_avl temp = root->left;
            temp = biggest_smallest_avl(temp);
            root->value = temp->value;
            temp->value = value;
            root->left = remove_avl(root->left, value);
        }
    }
    return balance_avl(root);
}

tree_avl biggest_smallest_avl(tree_avl root){
    if(root != NULL){
        if(root->right != NULL){
            return biggest_smallest_avl(root->right);
        }
    }
    return root;
}

void print_avl(tree_avl root, int type){
    if(type == 1){ //PREORDER
        if(root != NULL){
            printf("[%d]", root->value);
            print_avl(root->left, 1);
            print_avl(root->right, 1);
        }
    } else if(type == 2){ //INORDER
        if(root != NULL){
            print_avl(root->left, 2);
            printf("[%d]", root->value);
            print_avl(root->right, 2);
        } 
    } else if(type == 3){ //POSORDER
        if(root != NULL){
            print_avl(root->left, 3);
            print_avl(root->right, 3);
            printf("[%d]", root->value);
        } 
    }
}

void print_factor_avl(tree_avl root, int type){
    if(type == 1){ //PREORDER
        if(root != NULL){
            printf("[%d]", getFactor(root));
            print_factor_avl(root->left, 1);
            print_factor_avl(root->right, 1);
        }
    } else if(type == 2){ //INORDER
        if(root != NULL){
            print_factor_avl(root->left, 2);
            printf("[%d]", getFactor(root));
            print_factor_avl(root->right, 2);
        } 
    } else if(type == 3){ //POSORDER
        if(root != NULL){
            print_factor_avl(root->left, 3);
            print_factor_avl(root->right, 3);
            printf("[%d]", getFactor(root));
        } 
    }
}

