#include "procedure.h"

char* token_separater(char *source, char *delimiter, char **last) {
    char *begin, *next_token;
    char *sbegin; 
    int count = 0;

    /*Get the start of the token */
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
    /* If we have not reached at the end of the string */
    if (*next_token != '\0') {
        *last = begin + 2;
        *next_token = '\0';
        printf("%d\n", count);
        return sbegin;
    }
}

char* parse_msg(char* msg_rcvd) {
    // char* msg_rcvd;
    char* token = NULL;
    char delim[10] = "|";
    char* last = "";
    char code[4];
    // char second_field[];
    int len;
    char* response;

    token = token_separater(msg_rcvd, delim, &last);
    printf("%s\n", token);
    for (int i = 0; i < 3; i++) {
            token  = token_separater(NULL, delim, &last);
            printf("%s\n", token);
    }
    return token;
}
