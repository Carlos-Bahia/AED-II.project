#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

tree_bst insert_bst(tree_bst root, int value){
    if(root == NULL){
        tree_bst new = (tree_bst) malloc(sizeof(node_bst));
        
        new->value = value;
        new->left = NULL;
        new->right = NULL;

        return new;
    
    } else{
        if(value < root->value){
            root->left = insert_bst(root->left, value);
        } else{
            root->right = insert_bst(root->right, value);
        }

        return root;
    } 
}

tree_bst biggest_smallest_bst(tree_bst root){
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

tree_bst remove_bst(tree_bst root, int value){
    if(root ==  NULL){
        return root;
    } else{
        if(value == root->value){
            if(root->left == NULL){
                tree_bst temp = root->right;
                free(root);
                return temp;
            } else if(root->right == NULL){
                tree_bst temp = root->left;
                free(root);
                return temp;
            }

            tree_bst temp = biggest_smallest_bst(root->left);
            root->value = temp->value;
            root->left = remove_bst(root->left, temp->value);

        } else if(value < root->value){
            root->left = remove_bst(root->left, value);
        } else {
            root->right = remove_bst(root->right, value);
        }

        return root;
    }
}

void print_bst(tree_bst root, int type){
    if(type == 1){ //PREORDER
        if(root != NULL){
            printf("[%d]", root->value);
            print_bst(root->left, 1);
            print_bst(root->right, 1);
        }
    } else if(type == 2){ //INORDER
        if(root != NULL){
            print_bst(root->left, 2);
            printf("[%d]", root->value);
            print_bst(root->right, 2);
        } 
    } else if(type == 3){ //POSORDER
        if(root != NULL){
            print_bst(root->left, 3);
            print_bst(root->right, 3);
            printf("[%d]", root->value);
        } 
    }
}