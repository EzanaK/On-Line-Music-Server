/*
 * Music Server Header File
 */

typedef struct Album { // Structure of Music Albums
    int num_tracks;
    char **tracks;
    int *playlist_hits;
} Album_t;
Album_t *music_albums;

typedef struct Playlist { // Structure of Playlists
    int album_ID;
    int track_num;
    struct Playlist *next;
} Playlist_t;

typedef struct User_Account { // Structure of a User's Account
    int user_ID;
    Playlist_t *playlist_head;
    struct User_Account *next;
} Account_t;

FILE *album_file, *transaction_file;
Account_t *account_head;

void Initialization(); // Creates an array of music album structs using parsed information
void Transaction(); // Reads transaction request and calls other functions to complete the request
void print_album();
void open_account();
void print_account();
void add_playlist();
void close_account();
