#include "echo_header.h"
#include <algorithm>

const char ADDRESS[] = "127.0.0.1";
const char INPUT_FILE_NAME[] = "/home/hanik/proj/linux-4.3.3/Documentation/workqueue.txt";

void handle_server(int fd, FILE *in_file)
{
    char buf[MAX_MSG_LEN];
    buf[MAX_MSG_LEN - 1] = 0;
    ssize_t rv;
    fd_set fdset;
    int in_fd = fileno(in_file);
    int maxfd = std::max(in_fd, fd);

    for (;;) {
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        FD_SET(in_fd, &fdset);


        if (select(maxfd + 1, &fdset, NULL, NULL, NULL) == -1) {
            perror("select");
            return;
        }

        if (FD_ISSET(in_fd, &fdset)) {
            puts("Client: std is ready");

            if (fgets(buf, MAX_MSG_LEN, in_file) == NULL) {
                puts("Client: stop of input");
                return;
            }

            rv = writen(fd, buf, strlen(buf));
            if (rv == -1) {
                perror("written");
                break;
            }
        }

        if (FD_ISSET(fd, &fdset)) {
            puts("Client: server is ready before stdin");
            rv = readline(fd, buf, MAX_MSG_LEN);

            if (rv == -1) {
                perror("readline");
                break;
            }
            else if (rv == 0) {
                puts("Client: readline: Server ended");
                break;
            }
            else {
                puts(buf);
            }
        }
    }
}

// This function addresses the problem when, all data is
// sent from the client  and client is closed before
// server send response.
void handle_server2(int fd, FILE *in_file)
{
    char buf[MAX_MSG_LEN];
    buf[MAX_MSG_LEN - 1] = 0;
    ssize_t rv;
    fd_set fdset;
    int in_fd = fileno(in_file);
    int maxfd = std::max(in_fd, fd);
    bool eof_occured = false;

    for (;;) {
        FD_ZERO(&fdset);
        FD_SET(fd, &fdset);
        FD_SET(in_fd, &fdset);


        if (select(maxfd + 1, &fdset, NULL, NULL, NULL) == -1) {
            perror("select");
            return;
        }

        if (FD_ISSET(in_fd, &fdset)) {
            if (fgets(buf, MAX_MSG_LEN, in_file) == NULL) {
                if (shutdown(fd, SHUT_WR) == -1) {
                    perror("shutdown");
                    return;
                }
            }
            else {
                rv = writen(fd, buf, strlen(buf));
                if (rv == -1) {
                    perror("written");
                    break;
                }
            }
        }

        if (FD_ISSET(fd, &fdset)) {
            rv = readline(fd, buf, MAX_MSG_LEN);

            if (rv == -1) {
                perror("readline");
                break;
            }
            else if (rv == 0) {
                puts("Client: readline: Server ended");
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

    FILE *file = fopen(INPUT_FILE_NAME, "r");
    if (file) {
        handle_server2(sockfd, file);
        fclose(file);
    }
    else
        fprintf(stderr, "Client: cannot open file %s", INPUT_FILE_NAME);

    return 0;
}
