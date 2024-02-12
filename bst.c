#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tree_bst bst_search_remove(tree_bst root, char *value){
    if(root == NULL){
        return NULL;
    }

    int cmp = strcmp(value, root->value.author);

    if(cmp < 0){
        return bst_search_remove(root->left, value);
    } else if(cmp > 0){
        return bst_search_remove(root->right, value);
    } else {
        return root;
    } 
}

void bst_search(tree_bst root, char *value, FILE *archive){
    if(root == NULL){
        return;
    }

    int cmp = strncmp(value, root->value.author, strlen(value));

    if(cmp < 0){
        bst_search(root->left, value, archive);
    } else if(cmp > 0){
        bst_search(root->right, value, archive);
    } else {

        char line[1000];

        fseek(archive, root->value.position, SEEK_SET);

        fgets(line, sizeof(line), archive);

        char *token;
        char rotulos[5][20] = {"NOME", "AUTOR", "PRODUTOR", "DESCRIÇÃO", "ANO"};
        token = strtok(line, "|");
        int i = 0;
        while (token != NULL && i < 5) {
            printf("%s: %s\n", rotulos[i], token);
            token = strtok(NULL, "|");
            i++;
        }

        bst_search(root->left, value, archive);
        bst_search(root->right, value, archive);
    } 
}


tree_bst insert_bst(tree_bst root, index_bst value){
    if(root == NULL){
        tree_bst new = (tree_bst) malloc(sizeof(node_bst));
        
        new->value = value;
        new->left = NULL;
        new->right = NULL;

        return new;
    
    } else{
        if(strcmp(value.author , root->value.author) < 0){
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

tree_bst remove_bst(tree_bst root, index_bst value){
    if(root ==  NULL){
        return root;
    } else{
        if(strcmp(value.author, root->value.author) == 0 && value.position == root->value.position){
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

        } else if(strcmp(value.author , root->value.author) < 0){
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