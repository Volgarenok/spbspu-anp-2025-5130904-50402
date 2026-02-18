#ifndef MATRIXOPS_HPP
#define MATRIXOPS_HPP

#include <iostream>
#include <cstddef>

namespace pozdnyakov
{
  const size_t MAX_ELEMENTS = 10000;
  const size_t MAX_ROWS = 100;
  const size_t MAX_COLS = 100;

  std::istream &readDimensions(std::istream &in, size_t &rows, size_t &cols);
  std::istream &readMatrix(std::istream &in, int *data, size_t rows, size_t cols);
  size_t countDiagonalsWithoutZero(const int *data, size_t rows, size_t cols);
  void transformMatrixLayers(int *data, size_t rows, size_t cols);
  std::ostream &writeMatrix(std::ostream &out, const int *data, size_t rows, size_t cols);
  bool validateArgs(const char *s);
}

#endif
