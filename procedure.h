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

/* functions for error handling */
int game_lvl_err_handler(int conn_fd);
void appl_lvl_err_handler(int conn_fd);
void pres_lvl_err_handler(int conn_fd);
void sess_lvl_err_handler(int conn_fd);

/* functions to parse message */
char* token_separater(char* source, char* delimiter, char** last);
char* parse_msg(char* msg);

/* function to send message to client */
void send_msg(int conn_fd);

#endif /* __PROCEUDRE_H__ */
/* $end procedure.h */