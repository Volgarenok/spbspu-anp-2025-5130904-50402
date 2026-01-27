#include <iostream>
#include <fstream>
#include <cstring>

namespace khalikov
{
  void outputMtx(std::ostream & out, const int * a, size_t n, size_t m);
  std::istream & inputMtx(std::istream & in, int * a, size_t n, size_t m);
  size_t countSeddle(const int * a, size_t n, size_t m);
  int * spiral(const int * a, int * res, size_t n, size_t m,
    size_t st_row, size_t st_col, size_t end_row, size_t end_col, size_t & c);
  int * spiral(const int * a, int * res, size_t n, size_t m);
  const size_t MTXSIZE = 10000;
}

void khalikov::outputMtx(std::ostream & out, const int * a, size_t n, size_t m)
{
  out << m << ' ' << n;
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < m; ++j)
    {
      out << ' ' << a[i * m + j];
    }
  }
}

std::istream & khalikov::inputMtx(std::istream & in, int * a, size_t n, size_t m)
{
  for (size_t i = 0; i < n * m; ++i)
  {
    if (!(in >> a[i]))
    {
      return in;
    }
  }
  return in;
}

size_t khalikov::countSeddle(const int * a, size_t n, size_t m)
{
  int min = 0;
  int max = 0;
  size_t count = 0;
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < m; ++j)
    {
      min = a[i * m];
      max = a[j];
      for (size_t k = 1; k < m; ++k)
      {
        for (size_t w = 1; w < n; ++w)
          {
            if (a[i * m + k] < min)
            {
              min = a[i * m + k];
            }
            if (a[w * m + j] > max)
            {
              max = a[w * m + j];
            }
          }
      }
      if (min == max)
      {
        count++;
      }
    }
  }
  return count;
}

int * khalikov::spiral(const int * a, int * res, size_t n, size_t m,
  size_t st_row, size_t st_col, size_t end_row, size_t end_col, size_t & c)
{
  if (st_row > end_row || st_col > end_col)
  {
    return res;
  }

  for (size_t i = st_col; i <= end_col; ++i)
  {
    size_t step = st_row * m + i;
    res[step] = a[step] - c;
    ++c;
  }

  for (size_t i = st_row + 1; i <= end_row; ++i)
  {
    size_t step = end_col + i * m;
    res[step] = a[step] - c;
    ++c;
  }

  if (st_row < end_row)
  {
    for (size_t i = end_col - 1; i >= st_col; --i)
    {
      size_t step = end_row * m + i;
      res[step] = a[step] - c;
      ++c;
      if (i == 0)
      {
        break;
      }
    }
  }

  if (st_col < end_col)
  {
    for (size_t i = end_row - 1; i > st_row; --i)
    {
      size_t step = st_col + i * m;
      res[step] = a[step] - c;
      ++c;
      if (i == 0)
      {
        break;
      }
    }
  }
  return khalikov::spiral(a, res, n, m, st_row + 1, st_col + 1, end_row - 1, end_col - 1, c);
}

int * khalikov::spiral(const int * a, int * res, size_t n, size_t m)
{
  if (n == 0 || m == 0)
  {
    return res;
  }
  size_t c = 1;
  return spiral(a, res, n, m, 0, 0, n - 1, m - 1, c);
}

int main(int argc, char ** argv)
{
  namespace kh = khalikov;
  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }
  if (std::strlen(argv[1]) != 1)
  {
    std::cerr << "First parameter is not a number" << '\n';
    return 1;
  }
  if ((*argv[1] != '1') && (*argv[1] != '2'))
  {
    std::cerr << "First parametr is out of range" << '\n';
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Input file is not opened";
    return 2;
  }
  size_t n = 0;
  size_t m = 0;
  int * default_array = nullptr;
  int const_array[kh::MTXSIZE] = {};
  input >> n >> m;
  if (input.fail())
  {
    std::cerr << "Input error" << '\n';
    return 2;
  }
  if (*argv[1] == '1')
  {
    default_array = const_array;
  }
  else
  {
    try
    {
      default_array = new int[n * m];
    }
    catch (const std::bad_alloc &)
    {
      std::cerr << "bad_alloc" << '\n';
      return 2;
    }
  }
  kh::inputMtx(input, default_array, n, m);
  if (input.fail())
  {
    std::cerr << "Input error" << '\n';
    if (*argv[1] == '2')
    {
      delete[] default_array;
    }
    return 2;
  }
  input.close();
  int * res_array = nullptr;
  try
  {
    res_array = new int[n * m];
  }
  catch (const std::bad_alloc &)
  {
    std::cerr << "bad_alloc" << '\n';
    if (*argv[1] == '2')
    {
      delete[] default_array;
    }
    return 2;
  }
  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Output file is not opened";
    if (*argv[1] == '2')
    {
      delete[] default_array;
    }
    delete[] res_array;
    return 2;
  }
  size_t result_count = kh::countSeddle(default_array, n, m);
  res_array = kh::spiral(default_array, res_array, n, m);
  output << "the first number: " << '\n';
  output << result_count << '\n';
  output << "the second number: " << '\n';
  kh::outputMtx(output, res_array, n, m);
  output << '\n';
  if (*argv[1] == '2')
  {
    delete[] default_array;
  }
  delete[] res_array;
  return 0;
}
