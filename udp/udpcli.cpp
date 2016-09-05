#include "udp_common.h"

const char ADDRESS[] = "127.0.0.1";
char buf[MAX_MSG_LEN];

int main()
{
    addrinfo hints, *ai;
    int sockfd;
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    rv = getaddrinfo(ADDRESS, UDP_PORT, &hints, &ai);
    if (rv != 0) {
        fprintf(stderr, "getaddrinfo failed: %s", gai_strerror(rv));
        exit(EXIT_FAILURE);
    }

    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(ai);

    puts("Enter something: ");
    if (fgets(buf, sizeof(buf), stdin) != NULL) {
        printf("Client: you've entered: %s", buf);
        if (sendto(sockfd, buf, sizeof(buf), 0, ai->ai_addr, ai->ai_addrlen) != -1) {
            puts("Client: data sent");
        }
        else {
            perror("sendto");
        }
    }

    return 0;
}
