#include "getLine.hpp"
#include <iostream>
#include "extendSize.hpp"

char *dirko::getLine(std::istream &in, size_t &size)
{
  size_t copasity = 1;
  bool isSkipWp = in.flags() & std::ios::skipws;
  if (isSkipWp) {
    in >> std::noskipws;
  }
  char *str = reinterpret_cast< char * >(malloc(sizeof(char) * copasity));
  if (str == nullptr) {
    if (isSkipWp) {
      in >> std::skipws;
    }
    return str;
  }
  char ch = 0;
  while (in >> ch && ch != '\n') {
    if (size == copasity) {
      char *tmp = extendSize(str, size);
      free(str);
      if (tmp == nullptr) {
        str = nullptr;
        if (isSkipWp) {
          in >> std::skipws;
        }
        return str;
      }
      str = tmp;
      copasity *= 2;
    }
    str[size++] = ch;
  }
  if (isSkipWp) {
    in >> std::skipws;
  }
  if (size == copasity) {
    char *tmp = extendSize(str, size);
    free(str);
    if (tmp == nullptr) {
      str = nullptr;
      return str;
    }
    str = tmp;
    copasity *= 2;
  }
  str[size] = '\0';
  return str;
}
