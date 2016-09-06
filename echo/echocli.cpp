#include "echo_header.h"

const char ADDRESS[] = "127.0.0.1";

void handle_server(int fd)
{
    char buf[MAX_MSG_LEN];
    buf[MAX_MSG_LEN - 1] = 0;
    ssize_t rv;

    while (fgets(buf, MAX_MSG_LEN, stdin) != NULL) {
        rv = writen(fd, buf, strlen(buf));
        if (rv == -1) {
            perror("written");
            break;
        }
        else if (rv == 0) {
            puts("Client: Server ended??????");
            break;
        }
        else {
            rv = readline(fd, buf, MAX_MSG_LEN);

            if (rv == -1) {
                perror("readline");
                break;
            }
            else if (rv == 0) {
                puts("Client: Server ended??????");
                break;
            }
            else {
                puts(buf);
            }
        }
    }
}

int main()
{
    addrinfo hints, *ai;
    int sockfd;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    rv = getaddrinfo(ADDRESS, ECHO_PORT_STR, &hints, &ai);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo failed: %s", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    rv = connect(sockfd, ai->ai_addr, ai->ai_addrlen);
    if (rv == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(ai);

    handle_server(sockfd);

    return 0;
}
