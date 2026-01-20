#ifndef MTX_ACTION_HPP
#define MTX_ACTION_HPP
#include <iosfwd>
namespace karpovich
{
  const size_t MAX = 10000;
  std::istream& inputFunc(std::istream& input, int* arr, size_t size);
  void printArray(std::ostream& out, const int* arr, size_t rows, size_t cols);
}
#endif
