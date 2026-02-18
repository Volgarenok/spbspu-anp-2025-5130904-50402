#ifndef MTX_ACTIONS
#define MTX_ACTIONS
#include <iosfwd>
namespace dirko
{
  const size_t MAX_SIZE = 10000;
  std::istream &inputMtx(std::istream &input, int *matrix, size_t rows, size_t cols);
  void copyMtx(const int *matrix, int *result, size_t rows, size_t cols);
  std::ostream &output(std::ostream &output, const int *matrix, size_t rows, size_t cols);
}
#endif
