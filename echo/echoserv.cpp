#include "echo_header.h"

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

    for (;;) {
        sockaddr_storage clientaddr;
        socklen_t len;
        memset(&clientaddr, 0, sizeof(clientaddr));
        //clientfd = accept(servfd, (sockaddr*)&clientaddr, &len);
        clientfd = accept(servfd, NULL, NULL);

        printf("accepted--");
        printf("Client addr<%s> \n", sock_ntop((sockaddr*)&clientaddr, len));

        if (clientfd != -1) {
            if (fork() == 0) {
                close(servfd);
                client_handler(clientfd);
                close(clientfd);
            }
        }
        else
            perror("accept() failed");

        close(clientfd);
    }

    return 0;
}
