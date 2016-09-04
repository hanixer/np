#ifndef __NP_H__
#define __NP_H__
#include        <stdlib.h>
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */
#include	<time.h>		/* timespec{} for pselect() */
#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */
#include	<sys/select.h>	/* for convenience */
#include	<sys/param.h>	/* OpenBSD prereq for sysctl.h */
#include	<sys/sysctl.h>
#include	<poll.h>		/* for convenience */
//#include	<sys/event.h>	/* for kqueue */
#include	<strings.h>		/* for convenience */
#include	<sys/ioctl.h>
//#include	<sys/filio.h>
//#include	<sys/sockio.h>
#include	<pthread.h>
//#include	<net/if_dl.h>
//#include	<netinet/sctp.h>

const char *sock_ntop(const struct sockaddr *sockaddr, socklen_t addrlen);
int sock_bind_wild(int sockfd, int family);


ssize_t readn(int fd, void *buf, size_t nbytes);
ssize_t writen(int fd, const void *buf, size_t nbytes);
ssize_t readline(int fd, void *buf, size_t maxlen);
ssize_t readline2(int fd, void *buf, size_t maxlen);

#endif
