#include "massive.hpp"

int shirokov::stoi(const char *n)
{
  char *end = nullptr;
  long val = std::strtol(n, std::addressof(end), 10);

  if (*end != '\0')
  {
    throw std::logic_error("");
  }

  int out = static_cast< int >(val);
  return out;
}

std::istream &shirokov::input(std::istream &in, int *m, size_t lng)
{
  for (size_t i = 0; i < lng; i++)
  {
    in >> m[i];
    if (in.fail())
    {
      return in;
    }
  }
  return in;
}

std::ostream &shirokov::outputMatrix(std::ostream &out, const int *matrix, size_t m, size_t n)
{
  out << m << ' ' << n;
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      out << ' ' << matrix[transformIndexes(i, j, n)];
    }
  }
  return out;
}

size_t shirokov::transformIndexes(size_t i, size_t j, size_t n)
{
  return i * n + j;
}
