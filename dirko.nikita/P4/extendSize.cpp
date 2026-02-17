#include "extendSize.hpp"
#include <cstdlib>
#include <cstring>

char *dirko::extendSize(const char *str, size_t size)
{
  char *newStr = reinterpret_cast< char * >(malloc(size * 2 * sizeof(char)));
  if (newStr == nullptr) {
    return nullptr;
  }
  std::strncpy(newStr, str, size);
  newStr[size] = '\0';
  return newStr;
}
