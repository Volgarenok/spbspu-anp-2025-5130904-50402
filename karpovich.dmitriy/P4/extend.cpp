#include "extend.hpp"
#include <iostream>
char* karpovich::extend(char* old_str, size_t old_size)
{
  char* new_str = reinterpret_cast< char* >(malloc(old_size + 1));
  if (new_str == nullptr) {
    return nullptr;
  }
  for (size_t i = 0; i < old_size; ++i) {
    new_str[i] = old_str[i];
  }
  return new_str;
}
