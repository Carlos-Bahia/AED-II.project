#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

tree_avl avl_search_remove(tree_avl root, char *value){
    if(root == NULL){
        return NULL;
    }

    int cmp = strcmp(value, root->value->string);

    if(cmp < 0){
        return avl_search_remove(root->left, value);
    } else if(cmp > 0){
        return avl_search_remove(root->right, value);
    } else {
        return root;
    } 
}

void avl_search(tree_avl root, char *value, FILE *archive){
    if(root == NULL){
        return;
    }

    int cmp = strncmp(value, root->value->string, strlen(value));

    if(cmp < 0){
        avl_search(root->left, value, archive);
    } else if(cmp > 0){
        avl_search(root->right, value, archive);
    } else {

        char line[1000];

        fseek(archive, root->value->position, SEEK_SET);

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

        avl_search(root->left, value, archive);
        avl_search(root->right, value, archive);
    } 
}

tree_avl insert_avl(tree_avl root, index_avl value, int *insert_avl_factor){
    if(root == NULL){
        tree_avl new = (tree_avl) malloc(sizeof(node_avl));
        new->value = (index_avl *) malloc(sizeof(index_avl));
        new->value->string = strdup(value.string);
        new->value->position = value.position;
        new->factor = 0;
        new->left = NULL;
        new->right = NULL;

        *insert_avl_factor = 1;
        return new;
    }

    if (strcmp(value.string, root->value->string) < 0) {
        root->left = insert_avl(root->left, value, insert_avl_factor);
        if(*insert_avl_factor == 1){
            if(root->factor == 0){
                root->factor = -1;
                *insert_avl_factor = 1;
            } else if (root->factor == 1){
                root->factor = 0;
                *insert_avl_factor = 0;
            } else {
                *insert_avl_factor = 0;
                return balance_avl(root);
            }
        }
    } else{
        root->right = insert_avl(root->right, value, insert_avl_factor);
        if(*insert_avl_factor == 1){
            if(root->factor == 0){
                root->factor = 1;
                *insert_avl_factor = 1;
            } else if (root->factor == -1){
                root->factor = 0;
                *insert_avl_factor = 0;
            } else {
                *insert_avl_factor = 0;
                return balance_avl(root);
            }
        }
    }

    return root;
}

tree_avl balance_avl(tree_avl root){
    if(root == NULL){
        return NULL;
    }

    if(getFactor(root) > 0){
        if(getFactor(root->right) == 1 || getFactor(root->right) == 0){
            root = rotate_simple_left(root);
        } else if(getFactor(root->right) == -1){
            root = rotate_double_left(root);
        }
    } else{
        if(getFactor(root->left) == -1 || getFactor(root->left) == 0){
            root = rotate_simple_right(root);
        } else if(getFactor(root->left) == 1){
            root = rotate_double_right(root);
        }
    }

    return root;
}

tree_avl rotate_simple_right(tree_avl root){
    tree_avl temp = root->left;

    root->left = temp->right;
    temp->right = root;

    if(temp->factor == -1){
        temp->factor = 0;
        root->factor = 0;
    } else {
        temp->factor = 1;
        root->factor = -1;
    }

    return temp;
}
tree_avl rotate_simple_left(tree_avl root){
    tree_avl temp = root->right;

    root->right = temp->left;
    temp->left = root;

    if(temp->factor == 1){
        temp->factor = 0;
        root->factor = 0;
    } else {
        temp->factor = -1;
        root->factor = 1;
    }

    return temp;
}

tree_avl rotate_double_left(tree_avl root) {
    tree_avl temp_u, temp_v;

    temp_u = root->right;
    temp_v = temp_u->left;

    temp_u->left = temp_v->right;
    temp_v->right = temp_u;
    root->right = temp_v->left;
    temp_v->left = root;

    if (temp_v->factor == -1) {
        root->factor = 0;
        temp_u->factor = 1;
    } else if (temp_v->factor == 1) {
        root->factor = -1;
        temp_u->factor = 0;
    } else {
        root->factor = 0;
        temp_u->factor = 0;
    }
    temp_v->factor = 0;

    return temp_v;
}

tree_avl rotate_double_right(tree_avl root) {
    tree_avl temp_u, temp_v;

    temp_u = root->left;
    temp_v = temp_u->right;

    temp_u->right = temp_v->left;
    temp_v->left = temp_u;
    root->left = temp_v->right;
    temp_v->right = root;

    if (temp_v->factor == -1) {
        root->factor = 0;
        temp_u->factor = 1;
    } else if (temp_v->factor == 1) {
        root->factor = -1;
        temp_u->factor = 0;
    } else {
        root->factor = 0;
        temp_u->factor = 0;
    }
    temp_v->factor = 0;

    return temp_v;
}

int getFactor(tree_avl root){
    if(root == NULL)
        return 0;

    return root->factor;
}

tree_avl remove_avl(tree_avl root, index_avl value, int *remove_avl_factor){
    if(root == NULL){
        return root;
    }

    if (strcmp(value.string, root->value->string) < 0) {
        root->left = remove_avl(root->left,value, remove_avl_factor);
        if(*remove_avl_factor == 1){
            if(getFactor(root) == 1){
                root->factor = 2;
                if(getFactor(root->right) == 0){
                    *remove_avl_factor = 0;
                } else{
                    *remove_avl_factor = 1;
                }
                root = balance_avl(root);
            } else if(getFactor(root) == 0){
                root->factor = 1;
                *remove_avl_factor = 0;
            } else{
                root->factor = 0;
                *remove_avl_factor = 1;
            }
        }
    }
    if (strcmp(value.string, root->value->string) > 0) {
        root->right = remove_avl(root->right, value, remove_avl_factor);
        if(*remove_avl_factor == 1){
            if(getFactor(root) == -1){
                root->factor = -2;
                if(getFactor(root->left) == 0){
                    *remove_avl_factor = 0;
                } else{
                    *remove_avl_factor = 1;
                }
                root = balance_avl(root);
            } else if(getFactor(root) == 0){
                root->factor = -1;
                *remove_avl_factor = 0;
            } else{
                root->factor = 0;
                *remove_avl_factor = 1;
            }
        }
    }
    if (strcmp(value.string, root->value->string) == 0) {
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
        *remove_avl_factor = 1;
        }
        else{
            tree_avl temp = root->left;
            temp = biggest_smallest_avl(temp);
            root->value = temp->value;
            temp->value->string = value.string;
            root->left = remove_avl(root->left, value, remove_avl_factor);
            if(*remove_avl_factor){
                if(getFactor(root) == -1){
                    root->factor = 0;
                    *remove_avl_factor = 0;
                } else if(getFactor(root) == 0){
                    root->factor = 1;
                    *remove_avl_factor = 0;
                } else{
                    root->factor = 2;
                    if(getFactor(root->right) == 0){
                        *remove_avl_factor = 0;
                    }else{
                        *remove_avl_factor = 1;
                    }
                    root = balance_avl(root);
                }
            }
        }
    }
    return root;
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
            printf("[%s][%d]", root->value->string, root->value->position);
            print_avl(root->left, 1);
            print_avl(root->right, 1);
        }
    } else if(type == 2){ //INORDER
        if(root != NULL){
            print_avl(root->left, 2);
            printf("[%s][%d]", root->value->string, root->value->position);
            print_avl(root->right, 2);
        } 
    } else if(type == 3){ //POSORDER
        if(root != NULL){
            print_avl(root->left, 3);
            print_avl(root->right, 3);
            printf("[%s][%d]", root->value->string, root->value->position);
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

