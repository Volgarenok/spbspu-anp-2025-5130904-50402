#include "getline.hpp"
#include "expand.hpp"

char **shirokov::getline(std::istream &in, size_t &size, bool (*isDelimiter)(char symbol))
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws)
  {
    in >> std::noskipws;
  }
  char **massive = reinterpret_cast< char ** >(malloc(sizeof(char *)));
  size_t capacity = 1;
  size = 0;
  char *str = reinterpret_cast< char * >(malloc(sizeof(char)));
  size_t cap = 1;
  size_t s = 0;
  if (massive == nullptr || str == nullptr)
  {
    for (size_t i = 0; i < size; ++i)
    {
      free(massive[i]);
    }
    free(massive);
    free(str);
    if (is_skipws)
    {
      in >> std::skipws;
    }
    return nullptr;
  }
  while (in)
  {
    if (size == capacity)
    {
      expand(&massive, size, capacity);
    }
    if (s == cap)
    {
      expand(&str, s, cap);
    }
    if (massive == nullptr || str == nullptr)
    {
      for (size_t i = 0; i < size; ++i)
      {
        free(massive[i]);
      }
      free(massive);
      free(str);
      if (is_skipws)
      {
        in >> std::skipws;
      }
      return nullptr;
    }
    in >> str[s];
    if (!in || str[s] == '\0')
    {
      if (s > 0)
      {
        massive[size++] = str;
      }
      else
      {
        free(str);
      }
      break;
    }
    bool flag = isDelimiter(str[s]);
    if (flag)
    {
      if (s > 0)
      {
        str[s] = '\0';
        massive[size++] = str;
      }
      else
      {
        free(str);
      }
      str = reinterpret_cast< char * >(malloc(sizeof(char)));
      cap = 1;
      s = 0;
    }
    if (!flag)
    {
      s++;
    }
  }
  if (is_skipws)
  {
    in >> std::skipws;
  }
  return massive;
}
