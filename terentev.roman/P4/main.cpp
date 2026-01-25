#include <iostream>
#include <cstddef>
#include <cctype>
#include <stdexcept>

namespace terentev
{
  char* readLine(std::istream& in, std::size_t& cap, std::size_t& size);
  char* transformUppLow(const char* src, char* dst);
  char* transformRmvVow(const char* src, char* dst);
}

char* terentev::readLine(std::istream& in, std::size_t& cap, std::size_t& size)
{
  size = 0;
  if (cap == 0)
  {
    throw std::runtime_error("cap==0");
  }
  char* buf = new char[cap];
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
      char* new_buf = new char[new_cap];
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

char* terentev::transformUppLow(const char* src, char* dst)
{
  if (!src || !dst)
  {
    throw std::runtime_error("bad arguments");
  }
  std::size_t i = 0;
  while (src[i] != '\0')
  {
    unsigned char c = static_cast<unsigned char>(src[i]);
    if (std::isupper(c))
    {
      dst[i] = static_cast< char >(std::tolower(c));
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

char* terentev::transformRmvVow(const char* src, char* dst)
{
  if (!src || !dst)
  {
    throw std::runtime_error("bad arguments");
  }
  std::size_t i = 0;
  std::size_t j = 0;
  while (src[i] != '\0')
  {
    unsigned char c = static_cast<unsigned char>(src[i]);
    char lc = static_cast<char>(std::tolower(c));
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
  char* buf = nullptr;
  char* res = nullptr;
  try
  {
    const std::size_t initial_cap = 128;
    std::size_t cap = initial_cap;
    std::size_t size = 0;
    buf = terentev::readLine(std::cin, cap, size);
    if (!buf)
    {
      std::cerr << "Error: no input\n";
      return 1;
    }
    std::size_t res_cap = size + 1;
    res = new char[res_cap];

    terentev::transformUppLow(buf, res);
    std::cout << res << '\n';

    terentev::transformRmvVow(buf, res);
    std::cout << res << '\n';

    delete[] res;
    delete[] buf;
    return 0;
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
  }
  catch (...)
  {
    std::cerr << "Error: unknown error\n";
  }
  delete[] res;
  delete[] buf;
  return 1;
}
