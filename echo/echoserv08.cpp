#include "echo_header.h"
#include <algorithm>
#include <list>
#include <limits.h>

// This server does not creates child proccesses, it handles
// all incoming connections using poll() and a set of fdescr.

char buf[MAX_MSG_LEN];

void client_handler(int clientfd)
{
    char buf[MAX_MSG_LEN];
    ssize_t rv;

    for (;;) {
        rv = readline(clientfd, buf, MAX_MSG_LEN);
        if (rv == -1)
            break;
        else {
            rv = writen(clientfd, buf, rv);
            if (rv <= 0)
                break;
        }
    }

    puts("Server: finish handle loop");
}

int main()
{
    int rv;
    int servfd, clientfd;
    int maxi;
    pollfd pfd_set[FOPEN_MAX];

    servfd = create_tcp_listen_sock(ECHO_PORT_STR);
    if (servfd ==  -1) {
        puts("socket creation failed");
        return -1;
    }

    for (int i = 0; i < FOPEN_MAX; ++i)
        pfd_set[i].fd = -1;

    pfd_set[0].fd = servfd;
    pfd_set[0].events = POLLIN;

    for (;;) {

        if (poll(pfd_set, FOPEN_MAX, -1) == -1) {
            perror("poll");
            exit(1);
        }

        if (pfd_set[0].revents & POLLIN) {
            clientfd = accept(servfd, NULL, NULL);
            if (clientfd != -1) {
                int i = 1;
                while (i < FOPEN_MAX) {
                    if (pfd_set[i].fd == -1) {
                        pfd_set[i].fd = clientfd;
                        pfd_set[i].events = POLLIN | POLLRDBAND | POLLPRI;
                        break;
                    }
                    ++i;
                }
            }
            else {
                perror("accept() failed");
                break;
            }
        }

        for (int i = 1; i < FOPEN_MAX; ++i) {
            if (pfd_set[i].fd != -1) {
                if (pfd_set[i].revents) {
                    rv = readline(pfd_set[i].fd, &buf, sizeof(buf));
                    if (rv > 0) {
                        rv = writen(pfd_set[i].fd, &buf, rv);
                        if (rv == -1)
                            perror("writen");
                    }
                    else {
                        if (rv == -1)
                            perror("readline");

                        printf("Server: client %d removed", pfd_set[i].fd);

                        close(pfd_set[i].fd);

                        pfd_set[i].fd = -1;
                    }
                }
            }
        }
    }

    std::cin.get();

    return 0;
}
