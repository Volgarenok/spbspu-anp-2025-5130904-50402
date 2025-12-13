#include <iostream>
#include <iomanip>

namespace alisov
{
  void expand(char **str, size_t size, size_t &cap);
  char *getline(std::istream &in, size_t &k);
  void RmvVow(const char *str, size_t s, char *res);
  void DgtSnd(const char *str1, size_t s1, const char *str2, size_t s2, char *res);
}
char *alisov::getline(std::istream &in, size_t &k)
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws)
  {
    in >> std::noskipws;
  }
  size_t cap = 1;
  k = 0;
  char *str = static_cast< char * >(malloc(cap * sizeof(char)));
  if (str == nullptr)
  {
    return nullptr;
  }
  while (in)
  {
    alisov::expand(&str, k, cap);
    if (str == nullptr)
    {
      return nullptr;
    }
    in >> str[k];
    if (in.eof())
    {
      break;
    }
    k += 1;
  }
  if (in.bad() || k == 0)
  {
    free(str);
    return nullptr;
  }
  str[k] = '\0';
  if (is_skipws)
  {
    in >> std::skipws;
  }
  return str;
}

void expand(char **str, size_t size, size_t &cap)
{
  char *tmp = nullptr;
  if (size == cap)
  {
    cap *= 2;
    tmp = static_cast< char * >(malloc(cap));

    if (tmp == nullptr)
    {
      *str = nullptr;
      return;
    }
    for (size_t i = 0; i < cap; ++i)
    {
      tmp[i] = *str[i];
    }
    free(str);
    *str = tmp;
  }
}
int main()
{
}