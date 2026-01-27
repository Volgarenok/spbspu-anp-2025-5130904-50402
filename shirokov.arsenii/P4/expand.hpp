#ifndef EXPAND_HPP
#define EXPAND_HPP
#include <cstddef>

namespace shirokov
{
  void expand(char **str, size_t size, size_t &capacity);
  void expand(char ***str, size_t size, size_t &capacity);
}

#endif
