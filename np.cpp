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
  size_t read_count = 0;
  char *ptr = (char*) buf;
  int remains = nbytes;

  while (remains > 0) {
    read_rv = read(fd, ptr, remains);

    if (read_rv == -1) 
      return -1;
    else if (read_rv == 0)
      break;
    else {
      read_count -= remains;
      remains -= read_rv;
      ptr += read_rv;
    }
  }
  
  return read_count;
}
