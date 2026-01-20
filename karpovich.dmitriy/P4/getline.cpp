#include <iostream>
#include "getline.hpp"
#include "extend.hpp"
char* karpovich::getline(std::istream& in, size_t& s)
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }
  char* str = nullptr;
  char ch = '\0';
  while (in >> ch && ch != '\n') {
    char* temp = extend(str, s);
    if (temp == nullptr) {
      free(str);
      if (is_skipws) {
        in >> std::skipws;
      }
      return nullptr;
    }
    free(str);
    str = temp;
    str[s] = ch;
    ++s;
  }
  char* temp = extend(str, s);
  if (temp == nullptr) {
    free(str);
    if (is_skipws) {
      in >> std::skipws;
    }
    return nullptr;
  }
  free(str);
  str = temp;
  str[s] = '\0';
  if (is_skipws) {
    in >> std::skipws;
  }
  return str;
}
