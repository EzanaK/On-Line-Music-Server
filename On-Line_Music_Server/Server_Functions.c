/*
 * Music Server Functions
 */

#include <stdio.h>
#include <stdlib.h>
#include "Music_Server.h"

void Initialization() {
    // Creates an array of music album structs using parsed information
    int num_albums;
    fscanf(album_file, "%d", &num_albums);
    music_albums = (Album_t *)malloc(sizeof(Album_t)*num_albums);
    int album_ID;
    for (album_ID = 0; album_ID < num_albums; album_ID++) { // Goes through every album
        int amount_tracks;
        fscanf(album_file, "%d", &amount_tracks);
        music_albums[album_ID].num_tracks = amount_tracks;
        music_albums[album_ID].playlist_hits = (int *) malloc(sizeof(int) * amount_tracks);
        music_albums[album_ID].tracks = (char**)malloc(sizeof(char*) * amount_tracks);
        int track_num;
        for (track_num = 0; track_num < amount_tracks; track_num++) { // Goes through every track
            music_albums[album_ID].playlist_hits[track_num] = 0;
            int num_characters;
            fscanf(album_file, "%d", &num_characters);
            music_albums[album_ID].tracks[track_num] = malloc(sizeof(char) * (num_characters + 1));
            char skip;
            fscanf(album_file, "%c", &skip); // Skips over space in the file
            int c;
            for (c = 0; c < num_characters; c++) {
                // Scans each character in the track title
                fscanf(album_file, "%c", &music_albums[album_ID].tracks[track_num][c]);
            }
            music_albums[album_ID].tracks[track_num][c] = '\0';
        }
    }
}

void Transaction() {
    // Reads transaction request and calls other functions to complete the request
    int transaction_ID;
    fscanf(transaction_file, "%d", &transaction_ID);
    switch (transaction_ID) {
        case 1:
            print_album();
            break;
        case 2:
            open_account();
            break;
        case 3:
            print_account();
            break;
        case 4:
            add_playlist();
            break;
        case 5:
            close_account();
            break;
        case 6:
            fclose(album_file);
            fclose(transaction_file);
            exit(0);
    }
}

void print_album() {
    int album_ID;
    fscanf(transaction_file, "%d", &album_ID);
    printf("ALBUM %d\n", album_ID);
    int track_num;
    for (track_num = 0; track_num < music_albums[album_ID].num_tracks; track_num++) {
        printf("  %d: %s\n", music_albums[album_ID].playlist_hits[track_num], music_albums[album_ID].tracks[track_num]);
    }
    printf("\n");
}

void open_account() {
    Account_t *new_account;
    new_account = (Account_t *)malloc(sizeof(Account_t));
    fscanf(transaction_file, "%d", &new_account -> user_ID);
    new_account -> playlist_head = NULL;
    // Head insertion
    new_account -> next = account_head;
    account_head = new_account;
}

void print_account() {
    int user_ID;
    fscanf(transaction_file, "%d", &user_ID);
    Account_t *account_cur;
    account_cur = account_head;
    while (account_cur) { // Traverses until it finds matching user_ID
        if (account_cur -> user_ID  == user_ID) {
            break;
        }
        account_cur = account_cur -> next;
    }
    printf("ACCOUNT %d\n", user_ID);
    Playlist_t *playlist_cur;
    playlist_cur = account_cur -> playlist_head;
    if (playlist_cur == NULL) {
        printf("EMPTY PLAYLIST\n");
    }
    else{
        while(playlist_cur) {
            // Traverses and prints each track
            printf("  ALBUM %d:  %s\n", playlist_cur -> album_ID, music_albums[playlist_cur -> album_ID].tracks[playlist_cur -> track_num]);
            playlist_cur = playlist_cur -> next;
        }
    }
    printf("\n");
}

void add_playlist() {
    int user_ID;
    fscanf(transaction_file, "%d", &user_ID);
    Account_t *account_cur;
    account_cur = account_head;
    while (account_cur) { // Traverses until it finds matching user_ID
        if (account_cur -> user_ID  == user_ID) {
            break;
        }
        account_cur = account_cur -> next;
    }
    Playlist_t *new_playlist;
    new_playlist = (Playlist_t *)malloc(sizeof(Playlist_t));
    int album_ID;
    fscanf(transaction_file, "%d", &album_ID);
    new_playlist -> album_ID = album_ID;
    int track_num;
    fscanf(transaction_file, "%d", &track_num);
    new_playlist -> track_num = track_num;
    if (!account_cur -> playlist_head) { // Head insertion
        new_playlist -> next = account_cur -> playlist_head;
        account_cur -> playlist_head = new_playlist;
    }
    else { // Tail insertion
        Playlist_t *playlist_cur;
        for (playlist_cur = account_cur -> playlist_head; playlist_cur -> next; playlist_cur = playlist_cur -> next); // Traverses until it reaches the last linked node
        playlist_cur -> next = new_playlist;
        new_playlist -> next = NULL;
    }
    music_albums[album_ID].playlist_hits[track_num]++;
}

void close_account() {
    int user_ID;
    fscanf(transaction_file, "%d", &user_ID);
    Account_t *account_cur;
    Account_t *account_prev;
    account_cur = account_head;
    while (account_cur) { // Traverses until it finds matching user_ID
        if (account_cur -> user_ID == user_ID) {
            break;
        }
        account_prev = account_cur;
        account_cur = account_cur -> next;
    }
    if (account_cur == account_head) { // Head deletion
        account_head = account_cur -> next;
    }
    else if (!account_cur -> next) { // Tail deletion
        account_prev = account_cur -> next;
    }
    else { // Middle deletion
        account_prev -> next = account_cur -> next;
    }
    Playlist_t *playlist_cur = account_cur -> playlist_head;
    Playlist_t *playlist_prev;
    while (playlist_cur) { // Traverses the entire linked list
        // Decrements playlist_hits and frees each playlist structure
        music_albums[playlist_cur -> album_ID].playlist_hits[playlist_cur -> track_num]--;
        playlist_prev = playlist_cur;
        playlist_cur = playlist_cur -> next;
        free(playlist_prev);
    }
    free(account_cur);
}