#ifndef ANIME_WORLD
#define ANIME_WORLD
#include "avl.h"
#include "bst.h"
#include "red-black.h"

typedef struct anime{
    char *name;
    char *author;
    char *producer;
    char *description;
    int year;
} anime;

typedef struct anime_table{
    FILE *anime_data;
    tree_avl name_tree;
    tree_bst author_tree;
    tree_rb producer_tree;

} anime_table;

void print_menu(anime_table *table);
void add_anime_data(anime_table *table, anime *anime_info);
void initialize_tables(anime_table *animes);
void format_buffer(char *string);
void finalize_tables(anime_table *animes);
void remove_avl_index(anime_table *table);
void remove_bst_index(anime_table *table);
tree_bst search_bst_by_name(anime_table *table, char *string);
tree_avl search_avl_by_author(anime_table *table, char *string);
void print_bst_search(anime_table *table);
void print_avl_search(anime_table *table);
tree_avl load_name_tree(char *archive);
tree_bst load_author_tree(char *archive);
void print_by_name(anime_table *table, tree_avl temp, int option);
void print_by_author(anime_table *table, tree_bst tree, int option);
void print_anime_info_avl(anime_table *table, tree_avl temp);
void print_anime_info_bst(anime_table *table, tree_bst temp);
void save_avl_tree(char *archive, tree_avl tree);
void save_avl_tree_recursion(tree_avl tree, FILE *avl);
void save_author_tree(char *archive, tree_bst tree);
void save_author_tree_recursion(tree_bst tree, FILE *bst);
anime *read_anime_data();
void load_producer_tree(char *archive, tree_rb *tree);
void save_producer_tree(char *archive, tree_rb tree);
void save_producer_tree_recursion(tree_rb tree, FILE *rb);
void print_rb_search(anime_table *table);
void print_anime_info_rb(anime_table *table, tree_rb tree);
void print_by_producer(anime_table *table, tree_rb tree, int option);
void remove_rb_index(anime_table *table);
void get_name_and_author_by_producer(anime_table *table, char *producer, char **name, char **author);
void get_producer(anime_table *table, char *buffer, char **output, int option);
int verify_existance_in_data(anime_table *table, char *string, int option);

#endif