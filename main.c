#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "avl.h"
#include "red-black.h"
#include "table.h"

int main(){

    anime_table data;

    initialize_tables(&data);

    print_menu(&data);

    finalize_tables(&data);

    return 1;
}