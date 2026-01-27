#include "mtxConvertion.hpp"

void dirko::doLftBotClk(int *result, size_t rows, size_t cols)
{
  if (cols * rows == 0) {
    return;
  }
  size_t decrement = 1;
  size_t left = 0, right = cols - 1;
  size_t top = 0, bottom = rows - 1;
  while (top <= bottom && left <= right) {
    if (left <= right) {
      for (size_t i = bottom + 1; i > top; --i) {
        result[(i - 1) * cols + left] -= decrement++;
      }
      left++;
    }
    for (size_t j = left; j <= right; ++j) {
      result[top * cols + j] -= decrement++;
    }
    top++;
    for (size_t i = top; i <= bottom; ++i) {
      result[i * cols + right] -= decrement++;
    }
    right--;
    if (top <= bottom) {
      for (size_t j = right + 1; j > left; --j) {
        result[bottom * cols + j - 1] -= decrement++;
      }
      bottom--;
    }
  }
}
bool dirko::doLwrTriMtx(const int *matrix, size_t rows, size_t cols)
{
  const size_t min = (rows > cols) ? cols : rows;
  if (min < 2) {
    return false;
  }
  size_t line = 0;
  for (size_t i = 0; i < min; ++i) {
    for (size_t j = line + i + 1; j < line + min; j++) {
      if (matrix[i + j] != 0) {
        return false;
      }
    }
    line += min - 1;
  }
  return true;
}
