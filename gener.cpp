#include "np.h"
#include <vector>
#include <fstream>

int main()
{
  std::vector<char> bytes(1024*1024*65, 'b');
  std::ofstream ofs("big.txt");
  ofs.write(bytes.data(), bytes.size());
  return 0;
  
}
