#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
namespace terentev
{
  void minmax(const int* a, size_t n, size_t m, size_t& kolmin, size_t& kolmax);
  const size_t MAXOS = 10000;
}
int main(int argc, char** argv)
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  int bruh;
  if (std::strcmp(argv[1], "1") == 0)
  {
    bruh = 1;
  }
  else if (std::strcmp(argv[1], "2") == 0)
  {
    bruh = 2;
  }
  else
  {
    std::cerr << "First parameter is not a number or out of range\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Failed to open input file\n";
    return 2;
  }
  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Failed to open output file\n";
    return 2;
  }
  size_t n = 0, m = 0;
  if (!(input >> n >> m))
  {
    std::cerr << "Failed to read matrix dimensions\n";
    return 2;
  }
  if ((n * m) > terentev::MAXOS)
  {
    std::cerr << "Array cannot exist\n";
    return 2;
  }
  if (n <= 2 || m <= 2)
  {
    output << "0\n";
    return 0;
  }
  int buf[terentev::MAXOS] = {};
  int* a = nullptr;
  bool сhecker = false;
  if (bruh == 1)
  {
    a = buf;
  }
  else
  {
    a = reinterpret_cast< int* >(malloc(n * m * sizeof(int)));
    if (!a)
    {
      std::cerr << "bad_alloc\n";
      return 3;
    }
    сhecker = true;
  }
  for (size_t i = 0; i < n * m; ++i)
  {
    input >> a[i];
    if (!input)
    {
      std::cerr << "Failed to read array element\n";
      if (сhecker)
      {
        free(a);
      }
      return 2;
    }
  }
  size_t kolmin = 0, kolmax = 0;
  terentev::minmax(a, n, m, kolmin, kolmax);
  output << kolmin << '\n' << kolmax << '\n';
  if (сhecker)
  {
    free(a);
  }
}
void terentev::minmax(const int* a, size_t n, size_t m, size_t& kolmin, size_t& kolmax)
{
  size_t ix = m + 1;
  int min = a[m + 1];
  int max = a[m + 1];
  while (ix < n * m - 1 - m)
  {
    if (ix % m == 0)
    {
      ix++;
    }
    else if (ix % m == m - 1)
    {
      ix = ix + 2;
    }
    else
    {
      if (a[ix] < a[ix - m - 1] && a[ix] < a[ix - m]
        && a[ix] < a[ix - m + 1] && a[ix] < a[ix - 1]
        && a[ix] < a[ix + 1] && a[ix] < a[ix + m - 1]
        && a[ix] < a[ix + m] && a[ix] < a[ix + m + 1])
      {
        min = a[ix];
      }
      if (a[ix] > a[ix - m - 1] && a[ix] > a[ix - m]
        && a[ix] > a[ix - m + 1] && a[ix] > a[ix - 1]
        && a[ix] > a[ix + 1] && a[ix] > a[ix + m - 1]
        && a[ix] > a[ix + m] && a[ix] > a[ix + m + 1])
      {
        max = a[ix];
      }
      ix++;
    }
  }
  for (size_t p = m; p < n * m - m; ++p)
  {
    if (a[p] == min && p % m != 0 && p % m != m - 1)
    {
      kolmin++;
    }
    if (a[p] == max && p % m != 0 && p % m != m - 1)
    {
      kolmax++;
    }
  }
}
