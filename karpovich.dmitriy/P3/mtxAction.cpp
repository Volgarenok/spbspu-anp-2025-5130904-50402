#include "mtxAction.hpp"
#include <iostream>
std::istream& karpovich::inputFunc(std::istream& input, int* arr, size_t size)
{
  for (size_t i = 0; i < size; ++i) {
    if (!(input >> arr[i])) {
      return input;
    }
  }
  return input;
}
void karpovich::printArray(std::ostream& out, const int* arr, size_t rows, size_t cols)
{
  out << rows << ' ' << cols;
  for (size_t i = 0; i < rows * cols; ++i) {
    out << ' ' << arr[i];
  }
}
