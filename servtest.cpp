#include "np.h"

int main()
{
  addrinfo hints, *servaddr;
  int rt;
  int servfd, clientfd;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  rt = getaddrinfo(NULL, "12000", &hints, &servaddr);
  if (rt != 0) {
    puts(gai_strerror(rt));
    exit(EXIT_FAILURE);
  }

  servfd = socket(AF_INET, SOCK_STREAM, 0);
  if (servfd == -1) {
    switch (errno) {
    case EACCES: puts("EACCES"); break;
          case EAFNOSUPPORT: puts("EAFNOSUPPORT"); break;
	        case EINVAL: puts("EINVAL"); break;
		      case EMFILE: puts("EMFILE"); break;
			    case ENOBUFS: puts("ENOMEM"); break;
    default:
      puts("Don'tknow");
    }
    puts("socket() failed");
    exit(EXIT_FAILURE);
  }

  rt = bind(servfd, (sockaddr*)servaddr->ai_addr, servaddr->ai_addrlen);
  if (rt != 0) {
    puts("bind() failed");
    exit(EXIT_FAILURE);
  } 

  freeaddrinfo(servaddr);

  rt = listen(servfd, 5);
  if (rt != 0) {
    puts("listen() failed");
    exit(EXIT_FAILURE);
  }

  //  for (;;) {
    if (fork() == 0) {
      clientfd = accept(servfd, NULL, NULL);
      const char *str = "Child received your request";
      write(clientfd, str, strlen(str));
    }
    else {
      clientfd = accept(servfd, NULL, NULL);
      const char *str = "Parent received your request";
      write(clientfd, str, strlen(str));
    }   //  }
      

  
  
  return 0;
}
