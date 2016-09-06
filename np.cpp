#include "np.h"

const char *sock_ntop(const struct sockaddr *sockaddr, socklen_t addrlen)
{
    static char str[128];
    const void *data = NULL;
    int family = sockaddr->sa_family;

    switch (family) {
    case AF_INET:
        data = &((sockaddr_in*)sockaddr)->sin_addr;
        break;
    case AF_INET6:
        data = &((sockaddr_in6*)sockaddr)->sin6_addr;
        break;
    default:
        return NULL;
    }

    return inet_ntop(family, data, str, addrlen);
}

int sock_bind_wild(int sockfd, int family)
{
    (void)sockfd;
    (void)family;
    fprintf(stderr, "sock_bind_wild() not implemented");
    exit(EXIT_FAILURE);
}

ssize_t readn(int fd, void *buf, size_t nbytes)
{
    ssize_t read_rv = 0;
    ssize_t total = 0;
    char *ptr = (char*) buf;
    int remains = nbytes;

    while (remains > 0) {
        read_rv = read(fd, ptr, remains);

        if (read_rv == -1)
            return -1;
        else if (read_rv == 0)
            break;
        else {
            total += read_rv;
            remains -= read_rv;
            ptr += read_rv;
        }
    }

    return total;
}

ssize_t writen(int fd, const void *buf, size_t nbytes)
{
    ssize_t write_rv = 0;
    ssize_t total = 0;
    char *ptr = (char*) buf;
    size_t remains = nbytes;

    while (remains > 0) {
        write_rv = write(fd, buf, remains);

        if (write_rv == -1)
            return -1;
        else if (write_rv == 0)
            break;
        else {
            ptr += write_rv;
            total += write_rv;
            remains -= write_rv;
        }
    }

    return total;
}

ssize_t readline(int fd, void *buf, size_t maxlen)
{
    char ch;
    char *ptr = (char*) buf;
    ssize_t read_rv = 0;
    size_t i = 1;

    for (; i < maxlen; ++i) {
        read_rv = read(fd, &ch, 1);

        if (read_rv == -1)
            return -1;
        else if (read_rv == 0) {
            *ptr = 0;
            return (i - 1);
        }
        else {
            *ptr = ch;
            ptr++;

            if (ch == '\n')
                break;
        }
    }

    *ptr = 0;

    return i;
}

signal_function_t set_signal(int sig, signal_function_t func)
{
    struct sigaction act, oact;

    memset(&act, 0, sizeof(act));
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);

    if (sigaction(sig, &act, &oact) == -1) {
        perror("sigaction");
        return SIG_ERR;
    }

    return oact.sa_handler;
}
