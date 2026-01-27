#include <iostream>
#include <cstring>
#include <ios>
#include <new>

namespace lavrentev
{
  size_t difLat(char* s1, char* buf1);
  void uniTwo(char* s1, char* s2, size_t ex, size_t ex2, char* result);
  char* getline(std::istream& in, size_t& n);
}

int main()
{
  char* s1 = nullptr;
  size_t ex = 0;
  try
  {
    s1 = lavrentev::getline(std::cin, ex);
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation fail" << '\n';
    return 1;
  }

  if (ex == 0)
  {
    std::cerr << "Invalid string" << '\n';
    delete[] s1;
    return 1;
  }

  char* buf1 = nullptr;
  try
  {
    buf1 = new char[ex + 1];
    buf1[ex] = '\0';
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation fail" << '\n';
    delete[] s1;
    return 1;
  }

  char s2[] = "def ";
  size_t ex2 = 4;

  char* result = nullptr;
  try
  {
    result = new char[ex + ex2 + 1];
    result[ex + ex2] = '\0';
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation fail" << '\n';
    delete[] buf1;
    delete[] s1;
    return 1;
  }

  int ans_7 = lavrentev::difLat(s1, buf1);
  std::cout << "Ans 7: " << ans_7 << '\n';

  lavrentev::uniTwo(s1, s2, ex, ex2, result);
  std::cout << "Ans 12: " << result << '\n';

  delete[] buf1;
  delete[] s1;
  delete[] result;
}

size_t lavrentev::difLat(char* s1, char* buf1)
{
  size_t answer = 0;
  size_t i = 0;

  buf1[0] = '\0';

  while (s1[i] != '\0')
  {
    bool flag = false;

    for (size_t j = 0; j < answer; ++j)
    {
      if (buf1[j] == s1[i])
      {
        flag = true;
        break;
      }
    }

    if (!flag && std::isalpha(s1[i]))
    {
      buf1[answer] = s1[i];
      ++answer;
    }
    ++i;
  }

  buf1[answer] = '\0';

  return answer;
}

void lavrentev::uniTwo(char* s1, char* s2, size_t ex, size_t ex2, char* result)
{
  size_t min = 0;
  char* max_s_ptr;
  if (ex < ex2)
  {
    min = ex;
    max_s_ptr = s2;
  }
  else
  {
    min = ex2;
    max_s_ptr = s1;
  }

  size_t t = 0;

  for (size_t i = 0; i < min; ++i)
  {
    result[t] =  s1[i];
    result[t + 1] =  s2[i];
    t += 2;
  }

  t = min;

  for (size_t i = min * 2; i < ex + ex2; ++i)
  {
    result[i] = max_s_ptr[t];
    ++t;
  }
  result[ex + ex2] = '\0';
}

char* lavrentev::getline(std::istream& in, size_t& n)
{
  size_t cap = 10;

  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws)
  {
    in >> std::noskipws;
  }

  char* s = nullptr;

  try
  {
    s = new char[cap];
  }
  catch (const std::bad_alloc&)
  {
    n = 0;

    if (is_skipws)
    {
      in >> std::skipws;
    }

    throw;
  }

  char st;
  while (in >> st && st != '\n')
  {
    if (n == cap - 1)
    {
      size_t new_cap = cap + cap / 2;
      char* new_s = nullptr;

      try
      {
        new_s = new char[new_cap];
      }
      catch (const std::bad_alloc&)
      {
        delete[] s;
        n = 0;

        if (is_skipws)
        {
          in >> std::skipws;
        }

        throw;
      }

      for (size_t j = 0; j < n; ++j)
      {
        new_s[j] = s[j];
      }

      delete[] s;
      s = new_s;
      cap = new_cap;
    }

    s[n] = st;
    ++n;
  }

  s[n] = '\0';

  if (is_skipws)
  {
    in >> std::skipws;
  }

  return s;
}
