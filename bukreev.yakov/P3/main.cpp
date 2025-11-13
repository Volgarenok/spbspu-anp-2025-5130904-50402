#include <iostream>
#include <string>
#include <fstream>
#include <limits>

namespace bukreev
{
  int mode = 0;

  int* inputMatrix(std::istream& in, int* stackMatrix, size_t* rows, size_t* cols);
  void deleteMatrix(const int* matrix);

  size_t cntSdlPnt(const int* matrix, size_t rows, size_t cols);
  void lftTopClk(int* matrix, size_t rows, size_t cols);

  void output(std::ostream& out, size_t sdl, const int* matrix, size_t rows, size_t cols);
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

  int arg1 = 0;
  try
  {
    arg1 = std::stoi(argv[1]);
  }
  catch(const std::invalid_argument& e)
  {
    std::cerr << "First parameter is not a number.\n";
    return 1;
  }
  catch(const std::out_of_range& e)
  {
    std::cerr << "First parameter is out of range.\n";
    return 1;
  }

  if (arg1 != 1 && arg1 != 2)
  {
    std::cerr << "First parameter is out of range.\n";
    return 1;
  }

  bukreev::mode = arg1;

  int* matrix = nullptr;
  size_t rows = 0, cols = 0;
  std::ifstream input(argv[2]);
  if (arg1 == 1)
  {
    const size_t MTX_MAX_SIZE = 10000;
    int stackMatrix[MTX_MAX_SIZE] = {};
    matrix = stackMatrix;
  }

  try
  {
    matrix = bukreev::inputMatrix(input, matrix, &rows, &cols);
  }
  catch(const std::invalid_argument& e)
  {
    std::cerr << "Content of the input file is invalid: ";
    std::cerr << e.what() << ".\n";
    return 2;
  }
  catch(const std::bad_alloc& e)
  {
    return 3;
  }

  input.close();

  size_t sdl = 0;
  try
  {
    sdl = bukreev::cntSdlPnt(matrix, rows, cols);
  }
  catch(const std::bad_alloc& e)
  {
    bukreev::deleteMatrix(matrix);
    return 3;
  }

  bukreev::lftTopClk(matrix, rows, cols);

  std::ofstream output(argv[3]);

  bukreev::output(output, sdl, matrix, rows, cols);

  output.close();

  bukreev::deleteMatrix(matrix);

  return 0;
}

int* bukreev::inputMatrix(std::istream& in, int* stackMatrix, size_t* rows, size_t* cols)
{
  int* matrix = nullptr;

  size_t n = 0, m = 0;
  in >> n >> m;

  if (in.fail())
  {
    throw std::invalid_argument("No width or height of matrix");
  }

  if (mode == 1)
  {
    matrix = stackMatrix;
  }
  else
  {
    matrix = new int[n * m];
  }

  for (size_t i = 0; i < n * m; i++)
  {
    in >> matrix[i];
  }

  if (in.fail())
  {
    deleteMatrix(matrix);
    throw std::invalid_argument("Not enough elements of matrix");
  }

  *rows = n;
  *cols = m;

  return matrix;
}

void bukreev::deleteMatrix(const int* matrix)
{
  if (mode == 2)
  {
    delete[] matrix;
  }
}

size_t bukreev::cntSdlPnt(const int* matrix, size_t rows, size_t cols)
{
  size_t res = 0;

  size_t* colsMaximums = new size_t[cols];

  for (size_t i = 0; i < cols; i++)
  {
    int maxElem = std::numeric_limits<int>::min();
    size_t maxIndex = 0;

    for (size_t j = 0; j < rows; j++)
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
    int minElem = std::numeric_limits<int>::max();
    size_t minIndex = 0;

    for (size_t j = 0; j < cols; j++)
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

    start+=1;
    n -= 2;
    m -= 2;
  }
}

void bukreev::output(std::ostream& out, size_t sdl, const int* matrix, size_t rows, size_t cols)
{
  out << sdl << '\n';
  out << rows << ' ' << cols << ' ';
  for (size_t i = 0; i < rows * cols; i++)
  {
    out << matrix[i] << ' ';
  }
  out << '\n';
}
