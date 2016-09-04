#include "np.h"

class buffered_reader
{
public:
  buffered_reader()
    : m_ptr(0)
    , m_remaining_bytes(0)
    {}
 
  ssize_t readline(int fd, void *buff, size_t maxlen);
  
private:
  ssize_t readbuf(int fd, char &ch);
  
  static const int BUF_LEN = 1024 * 4;
  char m_buf[BUF_LEN];
  char *m_ptr;
  size_t m_remaining_bytes;
  
};
