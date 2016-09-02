#include "np.h"

#define PORT_STR "12001"

int main()
{
  addrinfo hints, *servaddr;
  int rt;
  int servfd, clientfd;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  rt = getaddrinfo(NULL, PORT_STR, &hints, &servaddr);
  if (rt != 0) {
    puts(gai_strerror(rt));
    exit(EXIT_FAILURE);
  }

  servfd = socket(servaddr->ai_family, SOCK_STREAM, 0);
  if (servfd == -1) {
    perror("socket() failed");
    exit(EXIT_FAILURE);
  }

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
    printf("EE we goo'\n");
    sockaddr_storage clientaddr;
    socklen_t len;
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientfd = accept(servfd, (sockaddr*)&clientaddr, &len);
    printf("accepted--");
    printf("Client addr<%s> \n", sock_ntop((sockaddr*)&clientaddr, len));
    
    if (clientfd != -1) {
      const int SIZE = 5;
      char buf[SIZE + 1];
      int count = readn(clientfd, buf, sizeof(buf));      
      printf("buff<%s> count<%d>\n", buf, count);
    }
    else
      perror("accept() failed");

    close(clientfd);
  }
  
  return 0;
}
