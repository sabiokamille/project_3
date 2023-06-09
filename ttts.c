#define _POSIX_C_SOURCE 200809L
#include "procedure.h"
#define LISTEN_Q 8


volatile int active = 1;
/*struct player_list {
    int num_players;
    char* players = (char *) malloc(num_players * sizeof(char));
    pthread_mutex_t lock;
};

void new_player (struct player_list *players_list) {
    pthread_mutex_lock(players_list->lock);
    // reallocate players_list array to include new player if the name is valid 
    players_list->num_players++;
};
*/ 

// data to be sent to worker threads
struct connection_data {
    struct sockaddr_storage addr;
    socklen_t addr_len;
    int fd;
};

int create_listener(char* service, int listen_q) {
    struct addrinfo hints, *info, *info_list;
    int error, sock;

    //initialize hints struct
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //stream socket
    hints.ai_flags = AI_PASSIVE; //for wildcard IP addresses

    //get information on listening socket
    error = getaddrinfo(NULL, service, &hints, &info_list);
    if (error) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return -1;
    }

    // attempt to create socket
    for (info = info_list; info != NULL; info = info->ai_next) {
        sock = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
        if (sock == -1) { //if failed to create socket, try next method
            continue;
        }

        // bind socket to requested port
        error = bind(sock, info->ai_addr, info->ai_addrlen);
        if (error) {
            close(sock);
        continue;
        }

        //enable listening for incoming connection requests from clients
        error = listen(sock,listen_q);
        if (error) {
            close(sock);
            continue;
        }
        //socket has been opened
        break;
    }

    freeaddrinfo(info_list);
    //info will be NULL if no method succeeded
    if (info == NULL) {
        fprintf(stderr, "Could not bind\n");
        return -1;
    }
    return sock;
}

void *read_response() {

}

int main(int argc, char **argv) {
    // sigset_t mask;
    struct connection_data *con;
    struct connection_data *con2;
    // int error, bytes;
    // pthread_t tid;
    // char buf[BUFLEN + 1];
    // pthread_t tid2;
    char *service = argc == 2 ? argv[1] : "15213";
    // install_handlers(&mask);
    int listener = create_listener(service, LISTEN_Q);
    if (listener < 0) {
        exit(EXIT_FAILURE);
    }
    printf("Listening for incoming connections on %s\n", service);
    while (active) {
        con = (struct connection_data *)malloc(sizeof(struct connection_data));
        con->addr_len = sizeof(struct sockaddr_storage);
        con->fd = accept(listener,
        (struct sockaddr *)&con->addr,
        &con->addr_len);
        if (con->fd < 0) {
            perror("accept");
            free(con);
            // TODO check for specific error conditions
            continue;
        }
        printf("Connected to player 1\n");

        //create second connection to second player client
        con2 = (struct connection_data *)malloc(sizeof(struct connection_data));
        con2->addr_len = sizeof(struct sockaddr_storage);
        con2->fd = accept(listener,
        (struct sockaddr *)&con->addr,
        &con->addr_len);
        if (con->fd < 0) {
            perror("accept");
            free(con);
            // TODO check for specific error conditions
            continue;
        }
        printf("Connected to player 2\n");

        play_game(con->fd, con2->fd);
        // temporarily disable signals
        // (the worker thread will inherit this mask, ensuring that SIGINT is
        // only delivered to this thread)
        /* 
        error = pthread_sigmask(SIG_BLOCK, &mask, NULL);
        if (error != 0) {
            fprintf(stderr, "sigmask: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        } 
        
        error = pthread_create(&tid, NULL, read_response, con);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            close(con->fd);
            free(con);
            continue;
        }
        // automatically clean up child threads once they terminate
        pthread_detach(tid);

        error = pthread_create(&tid2, NULL, read_response, con2);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            close(con2->fd);
            free(con2);
            continue;
        }
        // automatically clean up child threads once they terminate
        pthread_detach(tid);
        // unblock handled signals
        error = pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
        if (error != 0) {
            fprintf(stderr, "sigmask: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        } */
    }
    puts("Shutting down");
    close(listener);
    // returning from main() (or calling exit()) immediately terminates all
    // remaining threads
    // to allow threads to run to completion, we can terminate the primary thread
    // without calling exit() or returning from main:
    // pthread_exit(NULL);
    // child threads will terminate once they check the value of active, but
    // there is a risk that read() will block indefinitely, preventing the
    // thread (and process) from terminating
    // to get a timely shut-down of all threads, including those blocked by
    // read(), we will could maintain a global list of all active thread IDs
    // and use pthread_cancel() or pthread_kill() to wake each one
    return EXIT_SUCCESS;
}