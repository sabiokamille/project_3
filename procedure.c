#include "procedure.h"

void *play_game(int player1, int player2){
    char buf[BUFLEN + 1];
    int bytes;

    bytes = read(player1, buf, BUFLEN);
        if (bytes > 0) {
            buf[bytes] = '\0';
            // printf(buf);
            parse_msg(buf);
        }
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
        printf("count: %d\n", count);
        return sbegin;
    } if (*(begin + 1) == '\0') {
        *last = NULL;
        *next_token = '\0';
        return sbegin;
    }
}

void parse_msg(char* msg_rcvd) {
    // char* msg_rcvd;
    char delim[10] = "|";
    char* last = "";
    char *code = NULL;
    char *second_field;
    char *third_field;
    int count = 0;

    code = token_separater(msg_rcvd, delim, &last);
    // printf("%s\n", code);
    while (last != NULL) {
        if (count == 0) {
            second_field = token_separater(NULL, delim, &last);
            count++;
            // printf("%s\n", second_field);
        }
        else if (count == 1) {
            third_field = token_separater(NULL, delim, &last);
            count++;
            // printf("%s\n", third_field);
        } else if (count == 2) {
        token_separater(NULL, delim, &last);
        } else {
            /* handle invalid input */
            err_handler("Invalid message structure.");
            break;
        }
    }
    decode_msg(code, second_field, third_field);
}

void decode_msg(char* code, char* second_field, char* third_field){
    if (atoi(second_field) != strlen(third_field)) {
        err_handler("Incorrect third field length.");
    }
    if(code) {
        if(strcmp(code, "PLAY ") == 0) {
            printf("WAIT\n");
        } else if (strcmp(code, "MOVE ") == 0) {
            printf("MOVD\n");
        } else if (strcmp(code, "RSGN ") == 0) {
            printf("MOVD\n");
        } else if (strcmp(code, "DRAW ") == 0) {
            printf("MOVD\n");
        } else {
            err_handler("Invalid code.");
        }
    } else {
        err_handler("Invalid message structure.");
    }

}

void err_handler(char* reason) {
    printf("INVLD | %ld | %s |\n", strlen(reason), reason);
    exit(-1);
}
