#include <iostream>
#include <cstring>
#include <fstream>

namespace bukreev
{
  std::istream& inputMatrix(std::istream& in, int* matrix, size_t rows, size_t cols);

  size_t cntSdlPnt(const int* matrix, size_t rows, size_t cols);
  void lftTopClk(int* matrix, size_t rows, size_t cols);

  void outputMatrix(std::ostream& out, const int* matrix, size_t rows, size_t cols);
}

int main(int argc, char* argv[])
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments.\n";
    return 1;
  }

  if (argc > 4)
  {
    std::cerr << "Too many arguments.\n";
    return 1;
  }

  if (std::strlen(argv[1]) != 1)
  {
    std::cerr << "First parameter is out of range.\n";
    return 1;
  }

  int mode = std::atoi(argv[1]);
  if (mode == 0)
  {
    if (argv[1][0] != '0')
    {
      std::cerr << "First parameter is not a number.\n";
    }
    else
    {
      std::cerr << "First parameter is out of range.\n";
    }
    return 1;
  }

  if (mode != 1 && mode != 2)
  {
    std::cerr << "First parameter is out of range.\n";
    return 1;
  }

  std::ifstream input(argv[2]);

  size_t rows = 0, cols = 0;
  input >> rows >> cols;

  if (input.fail())
  {
    std::cerr << "No width or height of matrix.\n";
    return 2;
  }

  const size_t MTX_MAX_SIZE = 10000;
  int stackMatrix[MTX_MAX_SIZE] = {};
  int* matrix = nullptr;

  if (mode == 1)
  {
    matrix = stackMatrix;
  }
  else
  {
    matrix = new int[rows * cols];
  }

  bukreev::inputMatrix(input, matrix, rows, cols);
  if (input.fail())
  {
    if (mode == 2)
    {
      delete[] matrix;
    }
    std::cerr << "Content of the input file is invalid.\n";
    return 2;
  }

  input.close();

  size_t sdl = 0;
  try
  {
    sdl = bukreev::cntSdlPnt(matrix, rows, cols);
  }
  catch (const std::bad_alloc& e)
  {
    if (mode == 2)
    {
      delete[] matrix;
    }
    return 3;
  }

  bukreev::lftTopClk(matrix, rows, cols);

  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Failed to open output file.\n";
    return 2;
  }

  output << sdl << '\n';
  bukreev::outputMatrix(output, matrix, rows, cols);
  output << '\n';

  if (mode == 2)
  {
    delete[] matrix;
  }

  return 0;
}

std::istream& bukreev::inputMatrix(std::istream& in, int* matrix, size_t rows, size_t cols)
{
  for (size_t i = 0; i < rows * cols; i++)
  {
    in >> matrix[i];
  }
  return in;
}

size_t bukreev::cntSdlPnt(const int* matrix, size_t rows, size_t cols)
{
  size_t res = 0;

  size_t* colsMaximums = new size_t[cols];

  for (size_t i = 0; i < cols; i++)
  {
    int maxElem = matrix[i];
    size_t maxIndex = 0;

    for (size_t j = 1; j < rows; j++)
    {
      int elem = matrix[j * cols + i];
      if (elem > maxElem)
      {
        maxElem = elem;
        maxIndex = j;
      }
    }

    colsMaximums[i] = maxIndex;
  }

  for (size_t i = 0; i < rows; i++)
  {
    int minElem = matrix[i * cols];
    size_t minIndex = 0;

    for (size_t j = 1; j < cols; j++)
    {
      int elem = matrix[i * cols + j];
      if (elem < minElem)
      {
        minElem = elem;
        minIndex = j;
      }
    }

    if (colsMaximums[minIndex] == i)
    {
      res++;
    }
  }

  delete[] colsMaximums;

  return res;
}

void bukreev::lftTopClk(int* matrix, size_t rows, size_t cols)
{
  size_t start = 0;
  size_t n = rows, m = cols;

  size_t i = 0, j = 0;

  int sub = 1;

  while (n != 0 && m != 0)
  {
    i = start;
    j = start;

    if (n == 1)
    {
      for (; j < start + m; j++)
      {
        matrix[i * cols + j] -= sub;
        sub++;
      }
      break;
    }
    else if (m == 1)
    {
      for (; i < start + n; i++)
      {
        matrix[i * cols + j] -= sub;
        sub++;
      }
      break;
    }

    for(; j < start + m - 1; j++)
    {
      matrix[i * cols + j] -= sub;
      sub++;
    }
    for (; i < start + n - 1; i++)
    {
      matrix[i * cols + j] -= sub;
      sub++;
    }
    for(; j > start; j--)
    {
      matrix[i * cols + j] -= sub;
      sub++;
    }
    for (; i > start; i--)
    {
      matrix[i * cols + j] -= sub;
      sub++;
    }

    start++;
    n -= 2;
    m -= 2;
  }
}

void bukreev::outputMatrix(std::ostream& out, const int* matrix, size_t rows, size_t cols)
{
  out << rows << ' ' << cols;
  for (size_t i = 0; i < rows * cols; i++)
  {
    out << ' ' << matrix[i];
  }
}
