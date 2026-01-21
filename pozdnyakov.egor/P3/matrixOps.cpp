#include "matrixOps.hpp"
#include <cstdlib>

namespace pozdnyakov
{
  std::istream& readDimensions(std::istream& in, size_t& rows, size_t& cols)
  {
    long r = 0;
    long c = 0;

    if (!(in >> r))
    {
      return in;
    }
    if (!(in >> c))
    {
      return in;
    }
    if (r < 0 || c < 0)
    {
      in.setstate(std::ios::failbit);
      return in;
    }

    rows = static_cast< size_t >(r);
    cols = static_cast< size_t >(c);
    return in;
  }

  std::istream& readMatrix(std::istream& in, int* data, size_t rows, size_t cols)
  {
    size_t total = rows * cols;

    for (size_t i = 0; i < total; i++)
    {
      if (!(in >> data[i]))
      {
        return in;
      }
    }
    return in;
  }

  size_t countDiagonalsWithoutZero(const int* data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return 0;
    }

    size_t count = 0;
    size_t maxDiag = rows + cols - 2;

    for (size_t k = 0; k <= maxDiag; k++)
    {
      bool exists = false;
      bool hasZero = false;

      for (size_t i = 0; i < rows; i++)
      {
        size_t j = (k >= i) ? (k - i) : MAX_COLS;

        if (j < cols)
        {
          exists = true;
          if (data[i * cols + j] == 0)
          {
            hasZero = true;
            break;
          }
        }
      }

      if (exists && !hasZero)
      {
        count++;
      }
    }

    return count;
  }

  void transformMatrixLayers(int* data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return;
    }

    size_t layers = (rows < cols ? rows : cols);
    layers = (layers + 1) / 2;

    for (size_t layer = 0; layer < layers; layer++)
    {
      int inc = static_cast< int >(layer + 1);

      for (size_t r = layer; r < rows - layer; r++)
      {
        for (size_t c = layer; c < cols - layer; c++)
        {
          data[r * cols + c] += inc;
        }
      }
    }
  }

  std::ostream& writeMatrix(std::ostream& out, const int* data, size_t rows, size_t cols)
  {
    out << rows << ' ' << cols;

    size_t total = rows * cols;

    for (size_t i = 0; i < total; i++)
    {
      out << ' ' << data[i];
    }

    return out;
  }

  bool validateArgs(const char* s)
  {
    char* endptr = nullptr;

    long num = std::strtol(s, std::addressof(endptr), 10);
    if (endptr == s || *endptr != '\0')
    {
      return false;
    }

    if (num != 1 && num != 2)
    {
      return false;
    }

    return true;
  }
}
