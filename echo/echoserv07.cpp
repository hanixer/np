#include "echo_header.h"
#include <algorithm>
#include <list>

// This server does not creates child proccesses, it handles
// all incoming connections using select() and a set of fdescr.

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
    addrinfo hints, *servaddr;
    int rt;
    int servfd, clientfd;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    rt = getaddrinfo(NULL, ECHO_PORT_STR, &hints, &servaddr);
    if (rt != 0) {
        puts(gai_strerror(rt));
        exit(EXIT_FAILURE);
    }

    servfd = socket(servaddr->ai_family, servaddr->ai_socktype, servaddr->ai_protocol);
    if (servfd == -1) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    puts("Server says: ");
    puts(sock_ntop(servaddr->ai_addr, servaddr->ai_addrlen));
    printf("%d\n", ntohs(((sockaddr_in*)servaddr->ai_addr)->sin_port));


    rt = bind(servfd, (sockaddr*)servaddr->ai_addr, servaddr->ai_addrlen);
    if (rt != 0) {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servaddr);

    rt = listen(servfd, 5);
    if (rt != 0) {
        perror("listen() failed");
        exit(EXIT_FAILURE);
    }

    std::list<int> clients;
    fd_set fds;

    FD_ZERO(&fds);

    int maxfd = 0;

    for (;;) {
        FD_SET(servfd, &fds);
        maxfd = servfd;

        for (int fd : clients) {
            FD_SET(fd, &fds);
            maxfd = std::max(maxfd, fd);
        }

        if (select(maxfd + 1, &fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(1);
        }

        if (FD_ISSET(servfd, &fds)) {
            clientfd = accept(servfd, NULL, NULL);

            if (clientfd != -1) {
                clients.push_back(clientfd);
            }
            else {
                perror("accept() failed");
                break;
            }
        }
        else {
            auto it = clients.begin();
            while (it != clients.end()) {
                if (FD_ISSET(*it, &fds)) {
                    int rv = readline(*it, &buf, sizeof(buf));
                    if (rv > 0) {
                        rv = writen(*it, &buf, rv);
                        if (rv == -1)
                            perror("writen");
                        ++it;
                    }
                    else {
                        if (rv == -1)
                            perror("readline");

                        printf("Server: client %d removed", *it);

                        close(*it);
                        FD_CLR(*it, &fds);

                        it = clients.erase(it);
                    }
                }
                else
                    ++it;
            }
        }
    }

    printf("maxfd %d\n", maxfd);
    std::cin.get();

    return 0;
}
