#include "mtxActions.hpp"
#include <iostream>

std::istream &dirko::inputMtx(std::istream &input, int *matrix, size_t rows, size_t cols)
{
  for (size_t i = 0; i < rows * cols; ++i) {
    input >> matrix[i];
  }
  return input;
}
void dirko::copyMtx(const int *matrix, int *result, size_t rows, size_t cols)
{
  for (size_t i = 0; i < rows * cols; ++i) {
    result[i] = matrix[i];
  }
}
std::ostream &dirko::output(std::ostream &output, const int *matrix, size_t rows, size_t cols)
{
  output << rows << ' ' << cols;
  for (size_t i = 0; i < rows * cols; ++i) {
    output << ' ' << matrix[i];
  }
  return output;
}
