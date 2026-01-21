#include <iostream>
#include <iomanip>
#include <cstring>

namespace alisov
{
  void expand(char **str, size_t size, size_t &cap);
  char *getline(std::istream &in, size_t &k);
  void rmvVow(const char *str, char *res);
  void dgtSnd(const char *str1, size_t s1, const char *str2, size_t s2, char *res);
}
char *alisov::getline(std::istream &in, size_t &size)
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws)
  {
    in >> std::noskipws;
  }
  size_t cap = 1;
  size = 0;
  char *str = static_cast< char * >(malloc(cap * sizeof(char)));
  if (str == nullptr)
  {
    return nullptr;
  }
  while (in)
  {
    if (size == cap)
    {
      alisov::expand(&str, size, cap);
    }
    if (str == nullptr)
    {
      return nullptr;
    }
    in >> str[size];
    if (in.eof())
    {
      break;
    }
    size++;
  }
  if (!in || size == 0)
  {
    free(str);
    return nullptr;
  }
  str[size] = '\0';
  if (is_skipws)
  {
    in >> std::skipws;
  }
  return str;
}

void rmvVow(const char *str, size_t s, char *res)
{
  if (str == nullptr)
  {
    res = nullptr;
    return;
  }
  const char *vows = "aeiouyAEIOUY";
  size_t i = 0;
  size_t r = 0;
  while (str[i] != '\0')
  {
    if (!std::strchr(vows, str[i]))
    {
      res[r] = str[i];
      r++;
    }
    i++;
  }
  res[r] = '\0';
}

void dgtSnd(const char *str1, size_t s1, const char *str2, size_t s2, char *res)
{
  size_t i = 0;

  while (i < s1 && str1[i] != '\0')
  {
    res[i] = str1[i];
    ++i;
  }

  for (size_t k = 0; k < s2 && str2[k] != '\0'; ++k)
  {
    if (std::isdigit(static_cast< unsigned char >(str2[k])))
    {
      res[i] = str2[k];
      ++i;
    }
  }

  res[i] = '\0';
}

void expand(char **str, size_t size, size_t &cap)
{
  char *tmp = nullptr;
  cap *= 2;
  tmp = static_cast< char * >(malloc(cap));

  if (tmp == nullptr)
  {
    free(*str);
    *str = nullptr;
    return;
  }
  for (size_t i = 0; i < size; ++i)
  {
    tmp[i] = *str[i];
  }
  free(*str);
  *str = tmp;
}
int main()
{
  size_t size = 0;
  char *str = alisov::getline(std::cin, size);
  if (str == nullptr)
  {
    free(str);
    std::cerr << "Cant read line\n";
    return 1;
  }
  const size_t dgt_size = 5;
  char strDgt[dgt_size + 1] = "g1h2k";
  char *res1 = static_cast< char * >(malloc(size + 1));
  char *res2 = static_cast< char * >(malloc(size + dgt_size + 1));
  if (res1 == nullptr || res2 == nullptr)
  {
    free(str);
    free(res1);
    free(res2);
    std::cerr << "Memory error\n";
    return 1;
  }
  alisov::rmvVow(str, res1);
  alisov::dgtSnd(str, size, strDgt, dgt_size, res2);
  if (res1 == nullptr || res2 == nullptr)
  {
    free(str);
    free(res1);
    free(res2);
    std::cerr << "Error during conversion\n";
    return 1;
  }
  std::cout << "1. " << res1 << '\n';
  std::cout << "2. " << res2 << '\n';
  free(str);
  free(res1);
  free(res2);
}
