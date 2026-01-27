#ifndef MTX_CONVERTION
#define MTX_CONVERTION
#include <cstddef>
namespace dirko
{
  bool doLwrTriMtx(const int *matrix, size_t rows, size_t cols);
  void doLftBotClk(int *result, size_t rows, size_t cols);
}
#endif
