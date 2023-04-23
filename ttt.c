#define _POSIX_C_SOURCE 200809L
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define BUFLEN 256

int connect_inet(char *host, char *service) {
    struct addrinfo hint, *info_list, *info;
    int sock, error;

    // look up remote host
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC; // in practice, this means give us IPv4 or IPv6
    hint.ai_socktype = SOCK_STREAM; // indicate we want a streaming socket
    error = getaddrinfo(host, service, &hint, &info_list);
    if (error) {
        fprintf(stderr, "error looking up %s:%s: %s\n", host, service,
        gai_strerror(error));
        return -1;
    }
    for (info = info_list; info != NULL; info = info->ai_next) {
        sock = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
        if (sock < 0) continue;
        error = connect(sock, info->ai_addr, info->ai_addrlen);
        if (error) {
        close(sock);
        continue;
        }
        break;
    }
    freeaddrinfo(info_list);
    if (info == NULL) {
        fprintf(stderr, "Unable to connect to %s:%s\n", host, service);
        return -1;
    }
    return sock;
}

int main(int argc, char** argv) {
    int sock, bytes;
    char buf[BUFLEN];
    if (argc != 3) {
        printf("Specify host and service\n");
        exit(EXIT_FAILURE);
    }
    sock = connect_inet(argv[1], argv[2]);
    if (sock < 0) exit(EXIT_FAILURE);
    while ((bytes = read(STDIN_FILENO, buf, BUFLEN)) > 0) {
        printf("writing to buffer\n");
        write(sock, buf, bytes);
        // FIXME: should check whether the write succeeded!
    }
    close(sock);
    return EXIT_SUCCESS;
}