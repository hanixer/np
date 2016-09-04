#include "buffered_reader.h"

ssize_t buffered_reader::readline(int fd, void *buf, size_t maxlen)
{
  char ch;
  char *ptr = (char*) buf;
  ssize_t read_rv = 0;
  size_t i = 1;

  for (; i < maxlen; ++i) {
      read_rv = readbuf(fd, ch);

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


ssize_t buffered_reader::readbuf(int fd, char &ch)
{
  if (m_remaining_bytes <= 0) {
    int rv = read(fd, m_buf, BUF_LEN);
    if (rv == -1)
        return -1;
    else if (rv == 0)
        return 0;
    else {
        m_remaining_bytes = rv;
        m_ptr = &(m_buf[0]);
    }
  }

  int nnn = m_ptr - &(m_buf[0]);
  *m_ptr = 0;
  ch = *m_ptr;
  m_ptr++;
  m_remaining_bytes--;

  return 1;
}
