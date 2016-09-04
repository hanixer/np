#include "np.h"
#include "buffered_reader.h"
#include <time.h>

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

void test_readn()
{
  FILE *f = fopen("news","w");
  if (f == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  
  fwrite("0123456789", 1, 10, f);
  fclose(f);

  int fd = open("news", O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  char buf[5];
  buf[sizeof(buf) - 1] = 0;
  
  ssize_t read_bytes = readn(fd, buf, sizeof(buf) - 1);
  printf("string <%s>\n", buf);
  if (read_bytes == sizeof(buf) - 1) {
    puts("readn from file OK");
  }
}

void test_writen()
{
  const int times = 10000;
  const char part[] = "back to clock, silent the lock... ";
  const size_t part_len = strlen(part);
  size_t len = times * sizeof(part);
  char *str = (char*) malloc(len + 1);
  if (str == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < len; i++) {
    str[i] = part[i % part_len];
  }

  int fd = open("news", O_WRONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
    free(str);
  }

  ssize_t written_bytes = writen(fd, str, len + 1);
  close(fd);

  FILE *f = fopen("news", "r");
  if (f == NULL) {
    perror("fopen");
    free(str);
    exit(EXIT_FAILURE);
  }

  if (fseek(f, 0, SEEK_END) == 0) {
    ssize_t pos = ftell(f);
    printf("written<%ld>, file size<%ld>\n", written_bytes, pos);
  }

  fclose(f);
  

  free(str);
}
const int MAX = 1024*1024*65;
char buf[MAX + 1];

void test_readline()
{
  const char filename[] = "/media/sf_virtualb/e.tar";
  clock_t t;
  buf[MAX - 1] = 0;

  int fd = open(filename, O_RDONLY);
  if (fd != -1) {
    t = clock();
    readline(fd, buf, sizeof(buf));
    printf("readlinesed for <%f>\n", ((double)(clock() - t) / CLOCKS_PER_SEC));
    close(fd);
  }

  fd = open(filename, O_RDONLY);
  if (fd != -1) {
      buffered_reader rdr;
      t = clock();
      rdr.readline(fd, buf, sizeof(buf));
      printf("rdr.readlinesed for <%f>\n", ((double)(clock() - t) / CLOCKS_PER_SEC));
      close(fd);
  }

  puts("Enter something:");
  readline(STDIN_FILENO, buf, MAX);
  printf("You've entered <%s>", buf);
}

int main()
{
  testSockntop();
  test_readn();
  test_writen();
  test_readline();

  
  return 0;
}
