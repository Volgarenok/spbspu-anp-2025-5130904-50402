#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace alisov
{
  const size_t matrix_size = 10000;

  void input(std::istream &in, int *m, size_t lng)
  {
    for (size_t i = 0; i < lng; i++)
    {
      in >> m[i];
      if (in.fail())
      {
        throw std::logic_error("Cant read the matrix");
      }
    }
  }

  size_t lengthInput(char *file)
  {
    std::ifstream in(file);
    size_t a, b;
    in >> a >> b;
    if (in.fail())
    {
      throw std::logic_error("Couldn't read the size of matrix");
    }
    return a * b;
  }

  int myPow(int a, int b)
  {
    if (a == 0 || a == 1)
    {
      return a;
    }
    int res = 1;
    for (int i = 0; i < b; i++)
    {
      if (res > std::numeric_limits< int >::max() / a)
      {
        throw std::overflow_error("First parameter is out of range");
      }
      res *= a;
    }
    return res;
  }
  int sti(const char *str)
  {
    char *end = nullptr;
    long val = std::strtol(str, std::addressof(end), 10);

    if (*end != '\0')
    {
      throw std::logic_error("");
    }

    int out = static_cast< int >(val);
    return out;
  }

  int minSum(int *mtr, size_t m, size_t n)
  {
    int sum;
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

    int mas[n][2];
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

    return ans1;
  }
} // namespace alisov

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
  } catch (const std::logic_error &)
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
  }
  size_t lng = m * n;

  if (in.peek() == std::ifstream::traits_type::eof())
  {
    std::cerr << "File is empty";
    return 2;
  }

  try
  {
    lng = alisov::lengthInput(argv[2]);
  } catch (const std::logic_error &e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }

  int *matr = nullptr;

  try
  {
    if (num == 1)
    {
      int a[alisov::matrix_size] = {};
      matr = a;
    }
    if (num == 2)
    {
      int *b = new int[lng];
      matr = b;
    }
    alisov::input(in, matr, lng);
  } catch (std::logic_error &e)
  {
    std::cerr << e.what() << "\n";
    return 2;
  }

  std::ofstream out(argv[3]);
  size_t res1 = alisov::ncl(matr, m, n);
  int res2 = alisov::minSum(matr, m, n);
  out << "Answer for var-1: " << res1 << "\n";
  out << "Answer for var-2: " << res2 << "\n";

  if (num == 2)
  {
    delete[] matr;
  }
}
