#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "variants.hpp"
#include "massive.hpp"

namespace shirokov
{
  const size_t MATRIX_SIZE = 10000;
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
