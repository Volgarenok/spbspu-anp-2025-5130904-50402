#ifndef GET_LINE
#define GET_LINE
#include <iosfwd>

namespace dirko
{
  char *getLine(std::istream &in, size_t &size);
}
#endif
