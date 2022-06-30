/*
 * Ezana Kebede
 * 04/12/21
 * ENEE150
 * On-Line Music Server
*/

#include <stdio.h>
#include <stdlib.h>
#include "Music_Server.h"

int main(int argc, char *argv[]) {
    album_file = fopen(argv[1], "r");
    transaction_file = fopen(argv[2], "r");
    if (album_file == NULL || transaction_file == NULL) {
        printf("ERROR OPENING FILES!");
        exit(0);
    }
    Initialization();
    account_head = NULL;
    while (1) {
        Transaction();
    }
}
