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
    return 0;
  }
  
  return inet_ntop(family, data, str, addrlen);
}
