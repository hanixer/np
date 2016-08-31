#include "np.h"

void testSockntop()
{
  const char address[] = "127.0.0.1";

  addrinfo hints, *pai;
  int ret;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  ret = getaddrinfo(address, NULL, &hints, &pai);
  if (ret != 0) {
    printf("getaddrinfo failed: %s\n", gai_strerror(ret));
    exit(EXIT_FAILURE);
  }

  const char *result = sock_ntop(pai->ai_addr, pai->ai_addrlen);
  if (strcmp(result, address) == 0) {
    puts("Everything ok");
  }
}

int main()
{
  testSockntop();
  return 0;
}
