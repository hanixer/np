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

    puts("Server: finish handle loop");
}

void signchild_handler(int n)
{
    (void) n;
    int stat;

    pid_t pid;
    do {
        pid = waitpid(-1, &stat, WNOHANG);
        if (pid == -1)
            perror("waitpid");
        else
            printf("Server: child <%d> handled\n", pid);
    }
    while (pid > 0);
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

    set_signal(SIGCHLD, &signchild_handler);

    for (;;) {
        sockaddr_storage clientaddr;
        socklen_t len;
        memset(&clientaddr, 0, sizeof(clientaddr));

        do {
            clientfd = accept(servfd, (sockaddr*)&clientaddr, &len);
        } while(clientfd == -1 && errno == EINTR);

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
