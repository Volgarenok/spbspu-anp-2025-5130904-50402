#include "mtxChange.hpp"
size_t karpovich::locMin(const int* arrdyn, size_t rows, size_t cols)
{
  if (!arrdyn || rows < 3 || cols < 3) {
    return 0;
  }
  size_t minimum = 0;
  for (size_t i = 1; i < rows - 1; ++i) {
    for (size_t j = 1; j < cols - 1; ++j) {
      size_t k = i * cols + j;
      int now = arrdyn[k];
      bool yeslocMin = (now < arrdyn[k - 1]) && (now < arrdyn[k + 1]);
      yeslocMin = yeslocMin && (now < arrdyn[k - cols]) && (now < arrdyn[k + cols]);
      yeslocMin = yeslocMin && (now < arrdyn[k - 1 + cols]);
      yeslocMin = yeslocMin && (now < arrdyn[k + 1 + cols]);
      yeslocMin = yeslocMin && (now < arrdyn[k + 1 - cols]);
      yeslocMin = yeslocMin && (now < arrdyn[k - 1 - cols]);
      if (yeslocMin) {
        minimum++;
      }
    }
  }
  return minimum;
}

void karpovich::lftTopClk(int* arrdyn, size_t rows, size_t cols)
{
  if (!arrdyn) {
    return;
  }
  size_t total = rows * cols;
  if (total == 0) {
    return;
  }
  size_t top = 0, bottom = rows - 1;
  size_t left = 0, right = cols - 1;
  int current_value = 1;
  while (top <= bottom && left <= right) {
    for (size_t j = left; j <= right; ++j) {
      arrdyn[top * cols + j] -= current_value++;
    }
    top++;
    for (size_t i = top; i <= bottom; ++i) {
      arrdyn[i * cols + right] -= current_value++;
    }
    right--;
    if (top <= bottom) {
      for (size_t j = right + 1; j > left; --j) {
        arrdyn[bottom * cols + j - 1] -= current_value++;
      }
      bottom--;
    }
    if (left <= right) {
      for (size_t i = bottom + 1; i > top; --i) {
        arrdyn[(i - 1) * cols + left] -= current_value++;
      }
      left++;
    }
  }
}
