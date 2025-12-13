#include <iostream>
#include <cstddef>
#include <cctype>

int read_line(std::istream& in, char*& buf, std::size_t& cap, std::size_t& len)
{
  len = 0;
  if (cap == 0)
  {
    std::cerr << "Error: buffer capacity must be > 0\n";
    return 0;
  }
  if (buf == 0)
  {
    try
    {
      buf = new char[cap];
    }
    catch (...)
    {
      std::cerr << "Error: cannot allocate memory for input buffer\n";
      return 0;
    }
  }
  char ch;
  while (in.get(ch))
  {
    if (ch == '\n')
    {
      break;
    }
    if (len + 1 >= cap)
    {
      std::size_t new_cap = cap * 2;
      char* new_buf = 0;
      try
      {
        new_buf = new char[new_cap];
      }
      catch (...)
      {
        std::cerr << "Error: cannot allocate memory for input buffer\n";
        return 0;
      }
      for (std::size_t i = 0; i < len; ++i)
      {
        new_buf[i] = buf[i];
      }
      delete[] buf;
      buf = new_buf;
      cap = new_cap;
    }
    buf[len++] = ch;
  }
  if (len == 0 && !in)
  {
    return 0;
  }
  buf[len] = '\0';
  return 1;
}
int transform_upp_low(const char* src, char* dst, int dst_sz)
{
  if (src == 0 || dst == 0 || dst_sz == 0)
  {
    return 0u;
  }
  int i = 0;
  while (src[i] != '\0')
  {
    if (i + 1 >= dst_sz)
    {
      dst[0] = '\0';
      return 0u;
    }
    char c = static_cast <char> (src[i]);
    if (std::isupper(c))
    {
      dst[i] = static_cast <char> (std::tolower(c));
    }
    else
    {
      dst[i] = src[i];
    }
    ++i;
  }
  dst[i] = '\0';
  return i;
}
int transform_rmv_vow(const char* src, char* dst, int dst_sz)
{
  if (src == 0 || dst == 0 || dst_sz == 0) {
    return 0;
  }
  int i = 0;
  int j = 0;
  while (src[i] != '\0') {
    char c = static_cast <char> (src[i]);
    char lc = static_cast <char> (std::tolower(c));
    bool is_vowel = (lc == 'a') || (lc == 'e') || (lc == 'i') ||
      (lc == 'o') || (lc == 'u') || (lc == 'y');
    if (is_vowel)
    {
      ++i;
      continue;
    }
    if (j + 1 >= dst_sz)
    {
      dst[0] = '\0';
      return 0;
    }
    dst[j] = src[i];
    ++j;
    ++i;
  }
  dst[j] = '\0';
  return j;
}

int main()
{
  const std::size_t inite_cape = 128;
  char* buf = 0;
  std::size_t cap = inite_cape;
  std::size_t len = 0;
  if (!read_line(std::cin, buf, cap, len))
  {
    delete[] buf;
    return 0;
  }
  std::size_t res_cap = len + 1;
  char* res = 0;
  try
  {
    res = new char[res_cap];
  }
  catch (...)
  {
    delete[] buf;
    std::cerr << "Error: cannot allocate memory for result buffer\n";
    return 1;
  }
  int len1 = transform_upp_low(buf, res, static_cast <int> (res_cap));
  if (len1 == 0 && len != 0)
  {
    delete[] res;
    delete[] buf;
    std::cerr << "Error: result buffer too small\n";
    return 1;
  }
  std::cout << res << '\n';
  int len2 = transform_rmv_vow(buf, res, static_cast <int> (res_cap));
  if (len2 == 0 && len != 0)
  {
    delete[] res;
    delete[] buf;
    std::cerr << "Error: result buffer too small\n";
    return 1;
  }
  std::cout << res << '\n';
  delete[] res;
  delete[] buf;
  return 0;
}
