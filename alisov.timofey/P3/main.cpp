#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace alisov
{
  const size_t matrix_size = 10000;

  std::istream &input(std::istream &in, int *m, size_t lng)
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

  long sti(const char *str)
  {
    char *end = nullptr;
    long val = std::strtol(str, std::addressof(end), 10);

    if (*end != '\0')
    {
      throw std::logic_error("");
    }

    return val;
  }

  int minSum(int *mtr, size_t m, size_t n)
  {
    int sum = 0;
    int min = std::numeric_limits< int >::max();
    if (m + n < 2)
    {
      return 0;
    }
    for (size_t k = 0; k <= m + n - 2; ++k)
    {
      sum = 0;
      for (size_t i = 0; i < m; ++i)
      {
        if (k - i < n && i <= k)
        {
          sum += mtr[i * n + (k - i)];
        }
      }
      min = (sum < min) ? sum : min;
    }
    return min;
  }

  size_t ncl(int *mtr, size_t m, size_t n)
  {
    int ans1 = 0;
    size_t total = m * n;
    int max_length = 0;

    int (*mas)[2] = new int[n][2];
    for (size_t i = 0; i < n; ++i)
    {
      mas[i][0] = -1;
      mas[i][1] = 0;
    }

    for (size_t i = 0; i < total; ++i)
    {
      size_t j = i % n;
      if (mas[j][0] == -1)
      {
        mas[j][0] = j + 1;
        mas[j][1] = 1;
      }
      else
      {
        if (mtr[i] == mtr[i - n])
        {
          ++mas[j][1];
        }
        else
        {
          if (mas[j][1] > max_length)
          {
            max_length = mas[j][1];
          }
          mas[j][1] = 1;
        }
      }
    }

    for (size_t i = 0; i < n; ++i)
    {
      if (max_length < mas[i][1])
      {
        ans1 = mas[i][0];
        max_length = mas[i][1];
      }
    }
    delete[] mas;
    return ans1;
  }
}

int main(int argc, char **argv)
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments \n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments \n";
    return 1;
  }
  int num = 0;
  try
  {
    num = alisov::sti(argv[1]);
  }
  catch (const std::logic_error &)
  {
    std::cerr << "First parametr is not a numver\n";
    return 1;
  }
  if (num != 1 && num != 2)
  {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  size_t m, n;
  std::ifstream in(argv[2]);
  in >> m >> n;
  if (in.fail())
  {
    std::cerr << "Couldn't read size of matrix" << '\n';
    return 2;
  }
  size_t lng = m * n;

  int *matr = nullptr;
  int a[alisov::matrix_size] = {};

  if (num == 1)
  {
    matr = a;
  }
  else
  {
    int *b = new int[lng];
    matr = b;
  }
  if (alisov::input(in, matr, lng).fail())
  {
    std::cerr << "Cant read matrix" << '\n';
    if (num == 2)
    {
      delete[] matr;
    }
    return 2;
  }

  std::ofstream out(argv[3]);
  if (!out.is_open())
  {
    std::cerr << "Cant open output file" << '\n';
    if (num == 2)
    {
      delete[] matr;
    }
    return 2;
  }

  size_t res1 = alisov::ncl(matr, m, n);
  int res2 = alisov::minSum(matr, m, n);
  out << "Answer for var-1: " << res1 << '\n';
  out << "Answer for var-2: " << res2 << '\n';

  if (num == 2)
  {
    delete[] matr;
  }
}
