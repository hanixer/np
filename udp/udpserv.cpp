#include "udp_common.h"

char buf[MAX_MSG_LEN];

int main()
{
    addrinfo hints, *servaddr;
    int rt;
    int servfd, clientfd;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    rt = getaddrinfo(NULL, UDP_PORT, &hints, &servaddr);
    if (rt != 0) {
        puts(gai_strerror(rt));
        exit(EXIT_FAILURE);
    }

    servfd = socket(servaddr->ai_family, servaddr->ai_socktype, servaddr->ai_protocol);
    if (servfd == -1) {
        perror("socket() failed");
        exit(EXIT_FAILURE);
    }

    rt = bind(servfd, servaddr->ai_addr, servaddr->ai_addrlen);
    if (rt != 0) {
        perror("bind() failed");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(servaddr);


    rt = recvfrom(servfd, buf, sizeof(buf), 0, NULL, NULL);
    if (rt == -1) {
        perror("recvfrom() failed");
        exit(EXIT_FAILURE);
    }

    printf("Server: %s\n", buf);

    return 0;
}
