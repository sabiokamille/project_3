/* $begin procedure.h */
#ifndef PROCEDURE_H_
#define PROCEDURE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUFLEN 256

/* struct to represent player */
typedef struct player_struct{
    int fd;
    char role;
} player;

/* main function to play game */
void* play_game(int fd_player_1, int fd_player_2);

/* function for error handling */
void err_handler(char* reason);

/* functions to parse message */
char* token_separater(char* source, char* delimiter, char** last);
int parse_msg(char* msg, struct player_struct player);

/* function to decode message */
int decode_msg(char* code, char* second_field, char* third_field, struct player_struct player);

/* function to send message to client */
void send_msg(int code, struct player_struct *player);


#endif /* __PROCEUDRE_H__ */
/* $end procedure.h */