#ifndef GETLINE_HPP
#define GETLINE_HPP
#include <istream>

namespace shirokov
{
  char **getline(std::istream &in, size_t &size, bool (*isDelimiter)(char symbol));
}

#endif
