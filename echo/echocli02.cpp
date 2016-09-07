#include "echo_header.h"

const char ADDRESS[] = "127.0.0.1";

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

    linger ling;
    ling.l_onoff = 1;
    ling.l_linger = 0;
    socklen_t optlen;

    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER,  &ling, optlen) == -1) {
        perror("setsockopt");
        exit(1);
    }

    return 0;
}
