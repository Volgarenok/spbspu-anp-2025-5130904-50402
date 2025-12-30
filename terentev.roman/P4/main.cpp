#include <iostream>
#include <cstddef>
#include <cctype>

namespace terentev
{
  char* readLine(std::istream& in, std::size_t& cap, std::size_t& size);
  char* transformUppLow(const char* src, char* dst, int dst_sz);
  char* transformRmvVow(const char* src, char* dst, int dst_sz);
}
char* terentev::readLine(std::istream& in, std::size_t& cap, std::size_t& size)
{
  size = 0;
  if (cap == 0)
  {
    std::cerr << "Error: buffer capacity must be > 0\n";
    return nullptr;
  }
  char* buf = nullptr;
  try
  {
    buf = new char[cap];
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory for input buffer\n";
    return nullptr;
  }
  char ch;
  while (in.get(ch) && ch != '\n')
  {
    if (size + 1 >= cap)
    {
      std::size_t new_cap = cap * 2;
      if (new_cap < size + 2)
      {
        new_cap = size + 2;
      }
      char* new_buf = 0;
      try
      {
        new_buf = new char[new_cap];
      }
      catch (const std::bad_alloc&)
      {
        delete[] buf;
        std::cerr << "Error: cannot allocate memory for input buffer\n";
        return nullptr;
      }
      for (std::size_t i = 0; i < size; ++i)
      {
        new_buf[i] = buf[i];
      }
      delete[] buf;
      buf = new_buf;
      cap = new_cap;
    }
    buf[size++] = ch;
  }
  if (size == 0 && !in)
  {
    delete[] buf;
    return nullptr;
  }
  buf[size] = '\0';
  return buf;
}
char* terentev::transformUppLow(const char* src, char* dst, int dst_sz)
{
  if (src == 0 || dst == 0 || dst_sz <= 0)
  {
    return nullptr;
  }
  int i = 0;
  while (src[i] != '\0')
  {
    unsigned char c = static_cast <unsigned char> (src[i]);
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
  return dst;
}
char* terentev::transformRmvVow(const char* src, char* dst, int dst_sz)
{
  if (src == 0 || dst == 0 || dst_sz <= 0) {
    return nullptr;
  }
  int i = 0;
  int j = 0;
  while (src[i] != '\0') {
    unsigned char c = static_cast <unsigned char> (src[i]);
    char lc = static_cast <char> (std::tolower(c));
    bool is_vowel = (lc == 'a') || (lc == 'e') || (lc == 'i') ||
      (lc == 'o') || (lc == 'u') || (lc == 'y');
    if (!is_vowel)
    {
      dst[j++] = src[i];
    }
    ++i;
  }
  dst[j] = '\0';
  return dst;
}

int main()
{
  const std::size_t inite_cape = 128;
  std::size_t cap = inite_cape;
  std::size_t size = 0;
  char* buf = terentev::readLine(std::cin, cap, size);
  if (!buf)
  {
    std::cerr << "no input\n"
    return 1;
  }
  std::size_t res_cap = size + 1;
  char* res = nullptr;
  try
  {
    res = new char[res_cap];
  }
  catch (const std::bad_alloc&)
  {
    delete[] buf;
    std::cerr << "Error: cannot allocate memory for result buffer\n";
    return 1;
  }
  if (!terentev::transformUppLow(buf, res, static_cast<int>(res_cap)) && size != 0)
  {
    delete[] res;
    delete[] buf;
    std::cerr << "Error: result buffer too small\n";
    return 1;
  }
  std::cout << res << '\n';
  if (!terentev::transformRmvVow(buf, res, static_cast<int>(res_cap)) && size != 0)
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
