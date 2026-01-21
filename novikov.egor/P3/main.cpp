#include <iostream>
#include <fstream>

namespace novikov
{
  constexpr size_t MAX_STATIC_SIZE = 10000;

  bool isNumber(const char* s)
  {
    if (!s || *s == '\0')
    {
      return false;
    }

    for (size_t i = 0; s[i]; ++i)
    {
      if (s[i] < '0' || s[i] > '9')
      {
        return false;
      }
    }

    return true;
  }

  size_t countLocalMax(const int* const matrix, const size_t rows, const size_t cols)
  {
    if (rows < 3 || cols < 3)
    {
      return 0;
    }

    size_t count = 0;

    for (size_t i = 1; i < rows - 1; ++i)
    {
      for (size_t j = 1; j < cols - 1; ++j)
      {
        const int current = matrix[i * cols + j];
        bool isMax = true;

        for (int di = -1; di <= 1; ++di)
        {
          for (int dj = -1; dj <= 1; ++dj)
          {
            if (di || dj)
            {
              if (current <= matrix[(i + di) * cols + (j + dj)])
              {
                isMax = false;
              }
            }
          }
        }

        if (isMax)
        {
          ++count;
        }
      }
    }

    return count;
  }

  void spiralLeftBottomClockwise(int* const matrix, const size_t rows, const size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return;
    }

    size_t top = 0;
    size_t bottom = rows - 1;
    size_t left = 0;
    size_t right = cols - 1;

    int decrement = 1;

    while (left <= right && top <= bottom)
    {
      for (size_t j = left; j <= right; ++j)
      {
        matrix[bottom * cols + j] -= decrement++;
      }

      if (bottom == 0)
      {
        break;
      }

      --bottom;

      for (size_t i = bottom + 1; i-- > top;)
      {
        matrix[i * cols + right] -= decrement++;
      }

      if (right == 0)
      {
        break;
      }

      --right;

      for (size_t j = right + 1; j-- > left;)
      {
        matrix[top * cols + j] -= decrement++;
      }

      ++top;

      for (size_t i = top; i <= bottom; ++i)
      {
        matrix[i * cols + left] -= decrement++;
      }

      ++left;
    }
  }

  int mainLogic(const int argc, char* argv[])
  {
    if (argc != 4)
    {
      std::cerr << "Invalid number of arguments\n";
      return 1;
    }

    if (!isNumber(argv[1]))
    {
      std::cerr << "First parameter is not a number\n";
      return 1;
    }

    const int mode = std::atoi(argv[1]);
    if (mode != 1 && mode != 2)
    {
      std::cerr << "First parameter is out of range\n";
      return 1;
    }

    std::ifstream fin(argv[2]);
    if (!fin)
    {
      std::cerr << "Cannot open input file\n";
      return 2;
    }

    size_t rows = 0;
    size_t cols = 0;
    if (!(fin >> rows >> cols))
    {
      std::cerr << "Invalid matrix format\n";
      return 2;
    }

    const size_t totalElements = rows * cols;
    if (totalElements > MAX_STATIC_SIZE && mode == 1)
    {
      std::cerr << "Matrix too large for static storage\n";
      return 2;
    }

    int* matrixData = nullptr;
    int staticMatrixData[MAX_STATIC_SIZE] = { 0 };

    if (mode == 1)
    {
      matrixData = staticMatrixData;
    }
    else
    {
      matrixData = static_cast<int*>(std::malloc(sizeof(int) * totalElements));
      if (!matrixData)
      {
        std::cerr << "Memory allocation failed\n";
        return 2;
      }
    }

    for (size_t i = 0; i < totalElements; ++i)
    {
      if (!(fin >> matrixData[i]))
      {
        std::cerr << "Invalid matrix data\n";
        if (mode == 2)
        {
          std::free(matrixData);
        }

        return 2;
      }
    }

    std::ofstream fout(argv[3]);
    if (!fout)
    {
      std::cerr << "Cannot open output file\n";
      if (mode == 2)
      {
        std::free(matrixData);
      }

      return 2;
    }

    const size_t localMaxCount = countLocalMax(matrixData, rows, cols);
    fout << localMaxCount << "\n";

    spiralLeftBottomClockwise(matrixData, rows, cols);

    fout << rows << " " << cols;
    for (size_t i = 0; i < totalElements; ++i)
    {
      fout << " " << matrixData[i];
    }
    fout << "\n";

    if (mode == 2)
    {
      std::free(matrixData);
    }

    return 0;
  }
} 

int main(int argc, char* argv[])
{
  return novikov::mainLogic(argc, argv);
}
