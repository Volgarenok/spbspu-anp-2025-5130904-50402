#ifndef MASSIVE_HPP
#define MASSIVE_HPP
#include <cstddef>
#include <istream>

namespace shirokov
{
  std::istream &input(std::istream &in, int *m, size_t lng);
  size_t transformIndexes(size_t i, size_t j, size_t n);
  std::ostream &outputMatrix(std::ostream &out, const int *matrix, size_t m, size_t n);
  int stoi(const char *n);
}

#endif
