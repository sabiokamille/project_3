#include "procedure.h"

int game_end = -1;
int *game_over = &game_end;

void *play_game(int conn1, int conn2) {
    char buf[BUFLEN + 1];
    char buf2[BUFLEN + 1];
    int bytes;
    while (*game_over != 1) {
        /* read from buf until valid input provided */
        bytes = read(conn1, buf, BUFLEN);
        player player1;
        player1.fd = conn1;
        player1.role = 'X';
        while(!parse_msg(buf, player1)) {
            bytes = read(conn1, buf, BUFLEN);
                if (bytes > 0) {
                    buf[bytes] = '\0';
                    parse_msg(buf, player1);
                }
        }
        /* read from buf 2 until valid input provided */
        bytes = read(conn2, buf2, BUFLEN);
        player player2;
        player2.fd = conn2;
        player2.role = 'O';
        while(!parse_msg(buf2, player2)) {
            bytes = read(conn2, buf2, BUFLEN);
                if (bytes > 0) {
                    buf2[bytes] = '\0';
                    parse_msg(buf2, player2);
                }
        }
    }
    close(conn1);
    close(conn2);
    // send_msg(conn1, "OVER");
    // send_msg(conn2, "OVER");
}

char* token_separater(char *source, char *delimiter, char **last) {
    char *begin, *next_token;
    char *sbegin; 
    int count = 0;

    /* Get the start of the token */
    if (source) {
        begin = source; 
    }
    else {
        begin = *last; 
    }

    sbegin = begin;

    /* Scan through the string till we find the delimiter */
    while (*begin != '\0' && *begin != *delimiter){
        begin++;
        count++;
    }
    /* Check if we have reached end of the string */
    if (*begin == '\0') {
        /* We dont need to come further, hence return NULL */
        *last = NULL;
        return sbegin;
    }
    /* Scan the string till we find a character which is not in delimiter */
    next_token = begin;
    while(*next_token != '\0' && *next_token != *delimiter) {
        next_token++;
    }
    /* If we have not reached the end of the string */
    if (*next_token != '\0') {
        *last = begin + 1;
        *next_token = '\0';
        // printf("count: %d\n", count);
        return sbegin;
    } if (*(begin + 1) == '\0') {
        *last = NULL;
        *next_token = '\0';
        return sbegin;
    }
}

int parse_msg(char* msg_rcvd, struct player_struct player) {
    // char* msg_rcvd;
    char delim[10] = "|";
    char* last = "";
    char *code = NULL;
    char *second_field;
    char *third_field;
    char *fourth_field;
    int count = 0;

    code = token_separater(msg_rcvd, delim, &last);
    printf("code: %s\n", code);
    while (last != NULL) {
        if (count == 0) {
            second_field = token_separater(NULL, delim, &last);
            count++;
            printf("second field: %s\n", second_field);
        }
        else if (count == 1) {
            third_field = token_separater(NULL, delim, &last);
            count++;
            printf("third field: %s\n", third_field);
        } else if (count == 2) {
            fourth_field = token_separater(NULL, delim, &last);
            count++;
            printf("fourth field: %s\n", fourth_field);
        } else if (count == 3) {
        token_separater(NULL, delim, &last);
        } else {
            /* handle invalid input */
            err_handler("Invalid message structure.");
            break;
        }
    }
    return decode_msg(code, second_field, third_field, player);
}

int decode_msg(char* code, char* second_field, char* third_field, struct player_struct player){
    if(code) {
        /* check if second string correctly gives string length of third field 
        (excluding null character and whitespaces) */
        if (atoi(second_field) != (strlen(third_field) - 2)) {
            err_handler("Incorrect field length given");
            return 0;
        }
        if (strcmp(code, "PLAY ") == 0 && 0) {
            err_handler("Game already started");
            return 0;
        } else if(strcmp(code, "PLAY ") == 0) {
            printf("WAIT\n");
            send_msg(1, &player);
            if (player.role == 'O') {
                printf("BEGN\n");
            }
            return 1;
        } else if (strcmp(code, "MOVE ") == 0) {
            /* check if moved to empty space and if the move was a winning move */
            printf("MOVD\n");
        } else if (strcmp(code, "RSGN ") == 0) {
            /* notify both players and end game */
            *game_over = 1;
            return 0;
            printf("MOVD\n");
        } else if (strcmp(code, "DRAW ") == 0) {
            /* check that proper draw has been sent and 
             * notify other player and */
            printf("DRAW\n");
        } else {
            err_handler("Invalid code");
            return 0;
        }
    } else {
        err_handler("Invalid message structure");
        return 0;
    }
}

void err_handler(char* reason) {
    printf("INVLD | %ld | %s |\n", strlen(reason), reason);
    // exit(-1);
}

void send_msg(int code, struct player_struct *player) {
    char message[BUFLEN];
    switch (code) {
    case 1:
        strcpy(message, "WAIT");
        write(player->fd, message, sizeof(message));
        break;
    /*case 2:
        message = "WAIT";
        break;
    case 3:
        message = "WAIT";
        break;
    case 4:
        message = "WAIT";
        break;
    case 5:
        message = "WAIT";
        break;
    case 6:
        message = "WAIT";
        break;*/
    }
}