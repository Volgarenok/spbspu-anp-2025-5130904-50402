#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <cstdlib>

namespace shirokov
{
  const size_t MATRIX_SIZE = 10000;
  std::istream &input(std::istream &in, int *m, size_t lng);
  std::ostream &outputMatrix(std::ostream &out, const int *matrix, size_t m, size_t n);
  void spiral(int *matrix, size_t m, size_t n);
  bool isTriangularMatrix(const int *matrix, size_t m, size_t n);
  size_t transformIndexes(size_t i, size_t j, size_t n);
  int stoi(const char *n);
}

int main(int argc, char **argv)
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
  int num = 0;
  try
  {
    num = shirokov::stoi(argv[1]);
  }
  catch (const std::logic_error &)
  {
    std::cerr << "First parameter is not a number\n";
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
    std::cerr << "Couldn't read the size of matrix" << '\n';
    return 2;
  }
  size_t lng = m * n;

  int *matrix = nullptr;
  int static_buffer[shirokov::MATRIX_SIZE] = {};
  if (num == 1)
  {
    matrix = static_buffer;
  }
  else
  {
    matrix = new int[lng];
  }
  if (shirokov::input(in, matrix, lng).fail())
  {
    std::cerr << "Couldn't read the matrix" << '\n';
    if (num == 2)
    {
      delete[] matrix;
    }
    return 2;
  }
  std::ofstream out(argv[3]);
  if (!out.is_open())
  {
    std::cerr << "Couldn't open the output file\n";
    if (num == 2)
    {
      delete[] matrix;
    }
    return 2;
  }
  bool res2 = shirokov::isTriangularMatrix(matrix, m, n);
  shirokov::spiral(matrix, m, n);
  out << "Решение варианта 1:\n";
  shirokov::outputMatrix(out, matrix, m, n) << '\n';
  out << "Решение варианта 2:\n" << std::boolalpha << res2 << '\n';
  if (num == 2)
  {
    delete[] matrix;
  }
}

int shirokov::stoi(const char *n)
{
  char *end = nullptr;
  long val = std::strtol(n, std::addressof(end), 10);

  if (*end != '\0')
  {
    throw std::logic_error("");
  }

  int out = static_cast< int >(val);
  return out;
}

std::istream &shirokov::input(std::istream &in, int *m, size_t lng)
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

std::ostream &shirokov::outputMatrix(std::ostream &out, const int *matrix, size_t m, size_t n)
{
  out << m << ' ' << n;
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      out << ' ' << matrix[transformIndexes(i, j, n)];
    }
  }
  return out;
}

size_t shirokov::transformIndexes(size_t i, size_t j, size_t n)
{
  return i * n + j;
}

void shirokov::spiral(int *matrix, size_t m, size_t n)
{
  if (m == 0 || n == 0)
  {
    return;
  }
  size_t ptr = matrix[transformIndexes(m - 1, 0, n)];
  size_t leftBorder = 0;
  size_t rightBorder = n - 1;
  size_t upperBorder = 0;
  size_t lowerBorder = m - 1;

  size_t deductible = 1;
  while (leftBorder < n || upperBorder < m || rightBorder > 0 || lowerBorder > 0)
  {
    for (size_t i = lowerBorder; i + 1 >= upperBorder + 1; i--)
    {
      ptr = transformIndexes(i, leftBorder, n);
      matrix[ptr] -= deductible++;
    }
    if (leftBorder < n)
    {
      leftBorder++;
    }

    for (size_t j = leftBorder; j <= rightBorder; j++)
    {
      ptr = transformIndexes(upperBorder, j, n);
      matrix[ptr] -= deductible++;
    }
    if (upperBorder < m)
    {
      upperBorder++;
    }

    for (size_t i = upperBorder; i <= lowerBorder; i++)
    {
      ptr = transformIndexes(i, rightBorder, n);
      matrix[ptr] -= deductible++;
    }
    if (rightBorder > 0)
    {
      rightBorder--;
    }

    for (size_t j = rightBorder; j + 1 >= leftBorder + 1; j--)
    {
      ptr = transformIndexes(lowerBorder, j, n);
      matrix[ptr] -= deductible++;
    }
    if (lowerBorder > 0)
    {
      lowerBorder--;
    }
  }
}

bool shirokov::isTriangularMatrix(const int *matrix, size_t m, size_t n)
{
  if (m == 0 || n == 0)
  {
    return false;
  }
  size_t minn = m < n ? m : n;
  for (size_t i = 0; i < minn - 1; ++i)
  {
    for (size_t j = i + 1; j < minn; ++j)
    {
      if (matrix[transformIndexes(i, j, n)] != 0)
      {
        return false;
      }
    }
  }
  return true;
}
