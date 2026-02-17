#ifndef MTX_CHANGE_HPP
#define MTX_CHANGE_HPP
#include <iosfwd>
namespace karpovich
{
  size_t locMin(const int* arrdyn, size_t rows, size_t cols);
  void lftTopClk(int* arrdyn, size_t rows, size_t cols);
}
#endif
