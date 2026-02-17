#include "matrixOps.hpp"
#include <cstdlib>

namespace pozdnyakov
{
  std::istream &readDimensions(std::istream &in, size_t &rows, size_t &cols)
  {
    if (!(in >> rows)) {
      return in;
    }
    if (!(in >> cols)) {
      return in;
    }
    return in;
  }

  std::istream &readMatrix(std::istream &in, int *data, size_t rows, size_t cols)
  {
    size_t total = rows * cols;
    for (size_t i = 0; i < total; i++) {
      if (!(in >> data[i])) {
        return in;
      }
    }
    return in;
  }

  size_t countDiagonalsWithoutZero(const int *data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }

    size_t count = 0;

    for (size_t startRow = 0; startRow < rows; ++startRow) {
      bool hasZero = false;
      for (size_t r = startRow, c = 0; r < rows && c < cols; ++r, ++c) {
        if (data[r * cols + c] == 0) {
          hasZero = true;
          break;
        }
      }
      if (!hasZero) {
        count++;
      }
    }

    for (size_t startCol = 1; startCol < cols; ++startCol) {
      bool hasZero = false;
      for (size_t r = 0, c = startCol; r < rows && c < cols; ++r, ++c) {
        if (data[r * cols + c] == 0) {
          hasZero = true;
          break;
        }
      }
      if (!hasZero) {
        count++;
      }
    }

    return count;
  }

  void transformMatrixLayers(int *data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return;
    }

    size_t minDim = (rows < cols) ? rows : cols;
    size_t layers = (minDim + 1) / 2;

    for (size_t layer = 0; layer < layers; layer++) {
      int inc = 1;

      size_t lastRow = rows - 1 - layer;
      size_t lastCol = cols - 1 - layer;

      for (size_t r = layer; r <= lastRow; r++) {
        for (size_t c = layer; c <= lastCol; c++) {
          data[r * cols + c] += inc;
        }
      }
    }
  }

  std::ostream &writeMatrix(std::ostream &out, const int *data, size_t rows, size_t cols)
  {
    out << rows << ' ' << cols;
    size_t total = rows * cols;
    for (size_t i = 0; i < total; i++) {
      out << ' ' << data[i];
    }
    out << '\n';
    return out;
  }

  bool validateArgs(const char *s)
  {
    char *endptr = nullptr;
    long num = std::strtol(s, std::addressof(endptr), 10);
    if (endptr == s || *endptr != '\0') {
      return false;
    }
    if (num != 1 && num != 2) {
      return false;
    }
    return true;
  }
}
