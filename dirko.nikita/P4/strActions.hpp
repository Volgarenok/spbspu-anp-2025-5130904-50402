#ifndef STR_ACTIONS
#define STR_ACTIONS
#include <cstddef>

namespace dirko
{
  const size_t alphaSize = 26;
  const char *find(const char *start, const char *end, char target);
  size_t doDifLat(const char *str);
  char *doUppLow(const char *source, char *distention);
}
#endif
