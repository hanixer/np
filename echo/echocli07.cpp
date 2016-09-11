#include "echo_header.h"
#include <algorithm>
#include <vector>
#include <iostream>

// This client doesn't do any useful - it just clients connections to server.
size_t CONNECTION_COUNT = 1026;
const char ADDRESS[] = "127.0.0.1";

int main()
{
    addrinfo hints, *ai;
    int sockfd;
    int rv;
    std::vector<int> sockets;

    printf("%d", FD_SETSIZE);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    rv = getaddrinfo(ADDRESS, ECHO_PORT_STR, &hints, &ai);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo failed: %s", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < CONNECTION_COUNT; ++i) {
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

        sockets.push_back(sockfd);
    }

    freeaddrinfo(ai);

    std::cin.get();

    return 0;
}
