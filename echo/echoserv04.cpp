#include "echo_header.h"

void client_handler(int clientfd)
{
    char buf[MAX_MSG_LEN];
    ssize_t rv;

    for (;;) {
        rv = readn(clientfd, buf, sizeof(sum_args));
        if (rv == -1)
            break;
        else {
            sum_args args = { 0 };
            sum_res res = { 0 };
            if (rv == sizeof(args)) {
                args = *((sum_args*)buf);
                res.n = args.n1 + args.n2;
            }

            rv = writen(clientfd, &res, sizeof(res));
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

    for (;;) {
        sockaddr_storage clientaddr;
        socklen_t len;
        memset(&clientaddr, 0, sizeof(clientaddr));
        //clientfd = accept(servfd, (sockaddr*)&clientaddr, &len);
        clientfd = accept(servfd, NULL, NULL);

        if (clientfd != -1) {
            printf("accepted--");
            printf("Client addr<%s> \n", sock_ntop((sockaddr*)&clientaddr, len));
            if (fork() == 0) {
                close(servfd);
                client_handler(clientfd);
                close(clientfd);
                exit(0);
            }
        }
        else {
            perror("accept() failed");
            break;
        }

        close(clientfd);
    }

    return 0;
}
