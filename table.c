#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <ctype.h>
#include "table.h"
#include "avl.h"
#include "bst.h"
#include "red-black.h"

extern char* strdup(const char*);

void initialize_tables(anime_table *animes){
    animes->name_tree = NULL;
    animes->author_tree = NULL;
    animes->producer_tree = NULL;
    initialize_rb(&(animes->producer_tree));
    animes->anime_data = fopen("animes_data.txt", "a+");
    animes->name_tree = load_name_tree("name_index.dat");
    animes->author_tree = load_author_tree("author_index.dat");
    load_producer_tree("producer_index.dat", &(animes->producer_tree));
}

void finalize_tables(anime_table *animes){
    fclose(animes->anime_data);
    save_avl_tree("name_index.dat", animes->name_tree);
    save_author_tree("author_index.dat", animes->author_tree);
    save_producer_tree("producer_index.dat", animes->producer_tree);
}

void save_producer_tree(char *archive, tree_rb tree){
    FILE *rb;
    rb = fopen(archive, "w+");

    if(rb != NULL){
        save_producer_tree_recursion(tree, rb);
        fclose(rb);
    }
}

void save_producer_tree_recursion(tree_rb tree, FILE *rb){
    if(tree != NULL){
        fprintf(rb, "%s|%d\n", tree->value.producer, tree->value.position);
        save_producer_tree_recursion(tree->left, rb);
        save_producer_tree_recursion(tree->right, rb);
    }
}

void load_producer_tree(char *archive, tree_rb *tree){
    FILE *rb;
    rb = fopen(archive, "a+");
    if(rb == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char line[1000];
    char *string;

    while(fgets(line, sizeof(line), rb) != NULL){
        char *newline = strchr(line, '\n');
        if(newline != NULL)
            *newline = '\0';

        string = strtok(line, "|");
        index_rb *temp = (index_rb *) malloc(sizeof(index_rb));
        temp->producer = strdup(string);

        string = strtok(NULL, "|");
        if(string != NULL){
            temp->position = atoi(string);
        }

        insert_rb(tree, *temp);
        free(temp);
    }

    fclose(rb);
}

tree_bst load_author_tree(char *archive){
    FILE *bst;
    bst = fopen(archive, "a+");
    if(bst == NULL){
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char line[1000];
    char *string;
    tree_bst tree = NULL;

    while(fgets(line, sizeof(line), bst) != NULL){
        
        char *newline = strchr(line, '\n');
        if(newline != NULL)
            *newline = '\0';

        string = strtok(line, "|");
        index_bst *temp = (index_bst *) malloc(sizeof(index_bst));
        temp->author = strdup(string);

        string = strtok(NULL, "|");
        if(string != NULL)
            temp->position = atoi(string);

        tree = insert_bst(tree, *temp);
        free(temp);
    }

    fclose(bst);
    return tree;
}

tree_avl load_name_tree(char *archive) {
    FILE *avl;
    avl = fopen(archive, "a+");
    if (avl == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    char line[1000];
    char *string;
    tree_avl tree = NULL;
    int avl_factor;

    while(fgets(line, sizeof(line), avl) != NULL){
        
        char *newline = strchr(line, '\n');
        if (newline != NULL)
            *newline = '\0';

        string = strtok(line, "|");
        index_avl *temp = (index_avl *) malloc(sizeof(index_avl));
        temp->string = strdup(string); 

        string = strtok(NULL, "|");
        if(string != NULL)
            temp->position = atoi(string);

        tree = insert_avl(tree, *temp, &avl_factor);
        free(temp); 
    }

    fclose(avl);
    return tree;
}

void save_author_tree(char *archive, tree_bst tree){
    FILE *bst;
    bst = fopen(archive, "w+");

    if(bst != NULL){
        save_author_tree_recursion(tree, bst);
        fclose(bst);
    }
}

void save_author_tree_recursion(tree_bst tree, FILE *bst){
    if(tree != NULL){
        fprintf(bst, "%s|%d\n", tree->value.author, tree->value.position);
        save_author_tree_recursion(tree->left, bst);
        save_author_tree_recursion(tree->right, bst);
    }
}

void save_avl_tree(char *archive, tree_avl tree){

    FILE *avl;
    avl = fopen(archive, "w+");

    if(avl != NULL){
        save_avl_tree_recursion(tree, avl);
        fclose(avl);
    }
}

void save_avl_tree_recursion(tree_avl tree, FILE *avl){
    if(tree != NULL){
        fprintf(avl, "%s|%d\n", tree->value->string, tree->value->position);
        save_avl_tree_recursion(tree->left, avl);
        save_avl_tree_recursion(tree->right, avl);
    }
}


void print_menu(anime_table *table){
    int option = 98;
    int scanf_result;

    while(option != 99){
        switch (option)
        {
        case 1: //ADD ANIME
            add_anime_data(table, read_anime_data());
            break;
        
        case 2://SEARCH BY: NAME (ALL OCCURENCES)
            print_avl_search(table);
            break;

        case 3: //REMOVE BY: NAME (FIRST OCCURENCE)
            remove_avl_index(table);
            break;

        case 4: //PRINT ALL ANIMES PRE-ORDER (NAME)
            print_by_name(table, table->name_tree,1);
            break;

        case 5: //PRINT ALL ANIMES IN-ORDER (NAME)
            print_by_name(table,table->name_tree,2);
            break;
        
        case 6: //SEARCH BY: AUTHOR (ALL OCCURENCES)
            print_bst_search(table);
            break;

        case 7: //REMOVE BY: AUTHOR (FIRST OCCURENCE)
            remove_bst_index(table);
            break;
        
        case 8: //PRINT ALL ANIMES PRE-ORDER (AUTHOR)
            print_by_author(table,table->author_tree,1);
            break;
        
        case 9: //PRINT ALL ANIMES IN-ORDER (AUTHOR)
            print_by_author(table,table->author_tree,2);
            break;

        case 10: //SEARCH BY: PRODUCER (ALL OCCURENCES)
            print_rb_search(table);
            break;

        case 11: //REMOVE BY: PRODUCER (FIRST OCCURENCE)
            remove_rb_index(table);
            break;
        
        case 12: //PRINT ALL ANIMES PRE-ORDER (PRODUCER)
            print_by_producer(table,table->producer_tree,1);
            break;
        
        case 13: //PRINT ALL ANIMES IN-ORDER (PRODUCERT)
            print_by_producer(table,table->producer_tree,2);
            break;

        case 98:
            printf("1 - Adicionar anime\n");
            printf("2 - Procurar por nome\n");
            printf("3 - Remover por nome\n");
            printf("4 - Listar Pre-Order por Nome\n");
            printf("5 - Listar In-Order por Nome\n");
            printf("6 - Procurar por autor\n");
            printf("7 - Remover por autor\n");
            printf("8 - Listar Pre-Order por Autor\n");
            printf("9 - Listar In-Order por Autor\n");
            printf("10 - Procurar por Produtor\n");
            printf("11 - Remover por Produtor\n");
            printf("12 - Listar Pre-Order por Produtor\n");
            printf("13 - Listar In-Order por Produtor\n");
            printf("\n");
            printf("98 - Exibir Menu\n");
            printf("99 - Fechar programa\n");
            break;

        case 99:
            printf("Salvando Alterações...\n");
            printf("Fechando programa.\n\n");
            break;

        case -1:
            printf("\n\nOPÇÃO INVÁLIDA\n\n");
            option = 98;
            break;

        default:
            printf("\n\nOPÇÃO INVÁLIDA\n\n");
            break;
        }
        printf("\n**************************\n");
        scanf_result = scanf("%d", &option);
        printf("\n**************************\n");
        if(scanf_result != 1){
            option = -1;
            //CLEAR BUFFER
            fflush(stdin); // FOR WINDOWS
            __fpurge(stdin); //FOR LINUX
        }
    }      
}

int verify_existance_in_data(anime_table *table, char *string, int option){
    if(option == 1){
        tree_rb rb_temp = table->producer_tree;
        int cmp = strcmp(string, rb_temp->value.producer);

        while(rb_temp != NULL){
            cmp = strcmp(string, rb_temp->value.producer);

            if(cmp < 0){
                rb_temp = rb_temp->left;
            } else if(cmp > 0){
                rb_temp = rb_temp->right;
            } else {
                break;
            }
        }

        if(rb_temp != NULL){
            return 1;
        }

    } else if(option == 2){
        tree_avl avl_temp = table->name_tree;
        int cmp = strcmp(string, avl_temp->value->string);

        while(avl_temp != NULL){
            cmp = strcmp(string, avl_temp->value->string);

            if(cmp < 0){
                avl_temp = avl_temp->left;
            } else if(cmp > 0){
                avl_temp = avl_temp->right;
            } else {
                break;
            }
        }
        if(avl_temp != NULL){
            return 1;
        }

    } else if(option == 3){
        tree_bst bst_temp = table->author_tree;
        int cmp = strcmp(string, bst_temp->value.author);

        while(bst_temp != NULL){
            cmp = strcmp(string, bst_temp->value.author);

            if(cmp < 0){
                bst_temp = bst_temp->left;
            } else if(cmp > 0){
                bst_temp = bst_temp->right;
            } else {
                break;
            }
        }

        if(bst_temp != NULL){
            return 1;
        }
    }

    return 0;
}

void remove_rb_index(anime_table *table){
    char *buffer = (char *) malloc(512 * sizeof(char));
    int avl_factor;
    char *temp_name = (char *) malloc(512 * sizeof(char));
    char *temp_author = (char *) malloc(512 * sizeof(char));
    

    getchar();

    printf("Nome da produtora: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    printf("\n");

    if(buffer[0] == '\n' || buffer[0] == '\0' || buffer[0] == ' ')
        return;

    if(verify_existance_in_data(table, buffer,1)){
        get_name_and_author_by_producer(table, buffer, &temp_name, &temp_author);
        rb_search_remove(table->producer_tree, buffer);

        tree_avl tree_temp = avl_search_remove(table->name_tree, temp_name);
        tree_bst bst_temp = bst_search_remove(table->author_tree, temp_author);

        if(tree_temp != NULL)
            table->name_tree = remove_avl(table->name_tree, *tree_temp->value, &avl_factor);
        if(bst_temp != NULL)
            table->author_tree = remove_bst(table->author_tree, bst_temp->value);
    }

}

void get_name_and_author_by_producer(anime_table *table, char *producer, char **name, char **author){
    char line[1000];
    char *token;

    tree_rb rb_temp = table->producer_tree;
    int cmp = strcmp(producer, rb_temp->value.producer);

    while(rb_temp != NULL){
        cmp = strcmp(producer, rb_temp->value.producer);

        if(cmp < 0){
            rb_temp = rb_temp->left;
        } else if(cmp > 0){
            rb_temp = rb_temp->right;
        } else {
            break;
        }
    }

    fseek(table->anime_data, rb_temp->value.position, SEEK_SET);
    fgets(line, sizeof(line), table->anime_data);

    token = strtok(line, "|");
    *name = token;

    token = strtok(NULL, "|");
    *author = token;
}


void print_rb_search(anime_table *table){
    char *buffer = (char *) malloc(512 * sizeof(char));

    getchar();

    printf("Nome da produtora: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    printf("\n");

    rb_search(table->producer_tree, buffer, table->anime_data);
}

void print_by_producer(anime_table *table, tree_rb tree, int option){
    if(option == 1){
        if(tree != NULL){
            print_anime_info_rb(table, tree);
            print_by_producer(table, tree->left,1);
            print_by_producer(table, tree->right,1);
        }
    } else if(option == 2){
        if(tree != NULL){
            print_by_producer(table, tree->left,2);
            print_anime_info_rb(table, tree);
            print_by_producer(table, tree->right,2);
        }
    }
}

void print_anime_info_rb(anime_table *table, tree_rb tree){
    char line[1000];

    fseek(table->anime_data, tree->value.position, SEEK_SET);

    fgets(line, sizeof(line), table->anime_data);

    char *token;
    char rotulos[5][20] = {"NOME", "AUTOR", "PRODUTOR", "DESCRIÇÃO", "ANO"};
    token = strtok(line, "|");
    int i = 0;
    while (token != NULL && i < 5) {
        printf("%s: %s\n", rotulos[i], token);
        token = strtok(NULL, "|");
        i++;
    }
}

void print_by_author(anime_table *table, tree_bst tree, int option){
    if(option == 1){
        if(tree != NULL){
            print_anime_info_bst(table, tree);
            print_by_author(table, tree->left,1);
            print_by_author(table, tree->right,1);
        }
    } else if(option == 2){
        if(tree != NULL){
            print_by_author(table, tree->left,2);
            print_anime_info_bst(table, tree);
            print_by_author(table, tree->right,2);
        }
    }
}

void print_anime_info_bst(anime_table *table, tree_bst tree){
    char line[1000];

    fseek(table->anime_data, tree->value.position, SEEK_SET);

    fgets(line, sizeof(line), table->anime_data);

    char *token;
    char rotulos[5][20] = {"NOME", "AUTOR", "PRODUTOR", "DESCRIÇÃO", "ANO"};
    token = strtok(line, "|");
    int i = 0;
    while (token != NULL && i < 5) {
        printf("%s: %s\n", rotulos[i], token);
        token = strtok(NULL, "|");
        i++;
    }
}

void print_bst_search(anime_table *table){
    char *buffer = (char *) malloc(512 * sizeof(char));

    getchar();

    printf("Nome do autor: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    printf("\n");

    bst_search(table->author_tree, buffer, table->anime_data);
}

void print_anime_info_avl(anime_table *table, tree_avl temp){
    char line[1000];

    fseek(table->anime_data, temp->value->position, SEEK_SET);

    fgets(line, sizeof(line), table->anime_data);

    char *token;
    char rotulos[5][20] = {"NOME", "AUTOR", "PRODUTOR", "DESCRIÇÃO", "ANO"};
    token = strtok(line, "|");
    int i = 0;
    while (token != NULL && i < 5) {
        printf("%s: %s\n", rotulos[i], token);
        token = strtok(NULL, "|");
        i++;
    }
}

void print_by_name(anime_table *table, tree_avl temp, int option){
    if(option == 1){
        if(temp != NULL){
            print_anime_info_avl(table, temp);
            print_by_name(table, temp->left,1);
            print_by_name(table, temp->right,1);
        }
    } else if(option == 2){
        if(temp != NULL){
            print_by_name(table, temp->left,2);
            print_anime_info_avl(table, temp);
            print_by_name(table, temp->right,2);
        }
    }
}

tree_bst search_bst_by_name(anime_table *table, char *string){
    char line[1000];
    char *token;

    tree_avl avl_temp = table->name_tree;
    int cmp = strcmp(string, avl_temp->value->string);

    while(avl_temp != NULL){
        cmp = strcmp(string, avl_temp->value->string);

        if(cmp < 0){
            avl_temp = avl_temp->left;
        } else if(cmp > 0){
            avl_temp = avl_temp->right;
        } else {
            break;
        }
    }

    fseek(table->anime_data, avl_temp->value->position, SEEK_SET);
    fgets(line, sizeof(line), table->anime_data);

    token = strtok(line, "|");
    token = strtok(NULL, "|");

    tree_bst bst_temp = table->author_tree;

    while(bst_temp != NULL){
        cmp = strcmp(token, bst_temp->value.author);
        if(cmp == 0 && avl_temp->value->position == bst_temp->value.position){
            break;
        } else if(cmp < 0){
            bst_temp = bst_temp->left;
        } else{
            bst_temp = bst_temp->right;
        }
    }

    return bst_temp;

}

void get_producer(anime_table *table, char *buffer, char **output, int option){
    char line[1000];
    char *token;
        if(option == 1){
            tree_avl avl_temp = table->name_tree;
            int cmp = strcmp(buffer, avl_temp->value->string);

            while(avl_temp != NULL){
                cmp = strcmp(buffer, avl_temp->value->string);

                if(cmp < 0){
                    avl_temp = avl_temp->left;
                } else if(cmp > 0){
                    avl_temp = avl_temp->right;
                } else {
                    break;
                }
            }

        fseek(table->anime_data, avl_temp->value->position, SEEK_SET);
        fgets(line, sizeof(line), table->anime_data);

        token = strtok(line, "|");
        token = strtok(NULL, "|");
        token = strtok(NULL, "|");

        *output = token;

        } else{
            tree_bst bst_temp = table->author_tree;
            int cmp = strcmp(buffer, bst_temp->value.author);

            while(bst_temp != NULL){
                cmp = strcmp(buffer, bst_temp->value.author);

                if(cmp < 0){
                    bst_temp = bst_temp->left;
                } else if(cmp > 0){
                    bst_temp = bst_temp->right;
                } else {
                    break;
                }
            }

        fseek(table->anime_data, bst_temp->value.position, SEEK_SET);
        fgets(line, sizeof(line), table->anime_data);

        token = strtok(line, "|");
        token = strtok(NULL, "|");
        token = strtok(NULL, "|");

        *output = token;

        }
}

void remove_avl_index(anime_table *table){
    char *buffer = (char *) malloc(512 * sizeof(char));
    int avl_factor;
    char *producer = (char *) malloc(512 * sizeof(char));

    getchar();

    printf("Nome do anime: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    printf("\n");

    if(buffer[0] == '\n' || buffer[0] == '\0' || buffer[0] == ' ')
        return;

    if(verify_existance_in_data(table,buffer,2)){
        get_producer(table, buffer, &producer, 1);
        rb_search_remove(table->producer_tree, producer);
        
        tree_avl tree_temp = avl_search_remove(table->name_tree, buffer);
        tree_bst bst_temp = search_bst_by_name(table, tree_temp->value->string);
        if(tree_temp != NULL)
            table->name_tree = remove_avl(table->name_tree, *tree_temp->value, &avl_factor);
        if(bst_temp != NULL)
            table->author_tree = remove_bst(table->author_tree, bst_temp->value);
    }

}

void remove_bst_index(anime_table *table){
    char *buffer = (char *) malloc(512 * sizeof(char));
    int avl_factor;
    char *producer = (char *) malloc(512 * sizeof(char));

    getchar();

    printf("Nome do autor: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    printf("\n");

    if(buffer[0] == '\n' || buffer[0] == '\0' || buffer[0] == ' ')
        return;

    if(verify_existance_in_data(table,buffer,3)){
        get_producer(table, buffer, &producer, 1);
        rb_search_remove(table->producer_tree, producer);

        tree_bst tree_temp = bst_search_remove(table->author_tree, buffer);
        tree_avl avl_temp = search_avl_by_author(table, tree_temp->value.author);

        if(tree_temp != NULL)
            table->author_tree = remove_bst(table->author_tree, tree_temp->value);
        if(avl_temp != NULL)
            table->name_tree = remove_avl(table->name_tree, *avl_temp->value, &avl_factor);
    }
}

tree_avl search_avl_by_author(anime_table *table, char *string){
    char line[1000];
    char *token;

    tree_bst bst_temp = table->author_tree;
    int cmp = strcmp(string, bst_temp->value.author);

    while(bst_temp != NULL){
        cmp = strcmp(string, bst_temp->value.author);

        if(cmp < 0){
            bst_temp = bst_temp->left;
        } else if(cmp > 0){
            bst_temp = bst_temp->right;
        } else {
            break;
        }
    }

    fseek(table->anime_data, bst_temp->value.position, SEEK_SET);
    fgets(line, sizeof(line), table->anime_data);

    token = strtok(line, "|");

    tree_avl avl_temp = table->name_tree;

    while(avl_temp != NULL){
        cmp = strcmp(token, avl_temp->value->string);
        if(cmp == 0 && avl_temp->value->position == bst_temp->value.position){
            break;
        } else if(cmp < 0){
            avl_temp = avl_temp->left;
        } else{
            avl_temp = avl_temp->right;
        }
    }

    return avl_temp;
}

void print_avl_search(anime_table *table){
    char *buffer = (char *) malloc(512 * sizeof(char));

    getchar();

    printf("Nome do anime: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    printf("\n");

    avl_search(table->name_tree, buffer, table->anime_data);

}

anime *read_anime_data(){
    anime *new = (anime *) malloc(sizeof(anime));
    char *buffer = (char *) malloc(512 * sizeof(char));

    int temp;

    getchar();

    printf("Nome: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    new->name = strdup(buffer);

    printf("Autor: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    new->author = strdup(buffer);    

    printf("Produtora: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    new->producer = strdup(buffer);  

    printf("Descrição: ");
    fgets(buffer, 511, stdin);
    format_buffer(buffer);
    new->description = strdup(buffer); 

    printf("Ano: ");
    fgets(buffer, 511, stdin);
    new->year = (buffer[0] == '\0') ? -1 : atoi(buffer);

    return new;
}

void format_buffer(char *string) {
    if (string[0] == '\0' || string[0] == '\n') {
        strcpy(string, " ");
        return;
    }
    int i, j = 0;
    while (string[j] == ' ' || string[j] == '\t' || string[j] == '\n') {
        j++;
    }

    for (i = 0; string[i + j] != '\0'; ++i) {
        string[i] = string[i + j];
    }
    string[i] = '\0';

    int capitalize_next = 1;

    for (int i = 0; string[i] != '\0'; i++) {
        if (isspace(string[i])) {
            capitalize_next = 1; 
        } else {
            if (capitalize_next) {
                string[i] = toupper(string[i]);
                capitalize_next = 0;
            } else {
                string[i] = tolower(string[i]);
            }
        }
    }

    string[strlen(string)-1] = '\0';
}

void add_anime_data(anime_table *table, anime *anime_info){
    if(table->anime_data != NULL){
        int avl_factor;
        
        fseek(table->anime_data, 0, SEEK_END);

        index_avl *new = (index_avl *) malloc(sizeof(index_avl));
        new->string = anime_info->name;
        new->position = ftell(table->anime_data);

        index_bst *new_bst = (index_bst *) malloc(sizeof(index_bst));
        new_bst->author = anime_info->author;
        new_bst->position = ftell(table->anime_data);

        index_rb *new_rb = (index_rb *) malloc(sizeof(index_rb));
        new_rb->producer = anime_info->producer;
        new_rb->position = ftell(table->anime_data);

        fprintf(table->anime_data, "%s|%s|%s|%s|%d", anime_info->name, anime_info->author, anime_info->producer, anime_info->description, anime_info->year);
        fprintf(table->anime_data, "\n");
        table->name_tree = insert_avl(table->name_tree, *new, &avl_factor);
        table->author_tree = insert_bst(table->author_tree, *new_bst);
        insert_rb(&(table->producer_tree), *new_rb);
    }
}

