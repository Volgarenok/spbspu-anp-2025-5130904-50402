#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>

namespace matveev
{
  std::istream & rMatrix(std::istream & in, int * matrix, size_t rows, size_t cols);
  std::ostream & wMatrix(std::ostream & out, const int * matrix, size_t rows, size_t cols);
  void spiral(int * matrix, size_t rows, size_t cols);
  size_t find(const int * matrix, size_t rows, size_t cols);
}

int main(int argc, char * argv[])
{
  if (argc != 4)
  {
    std::cerr << "Error ./lab num input output\n";
    return 1;
  }

  char * ePtr = nullptr;
  long num = std::strtol(argv[1], std::addressof(ePtr), 10);
  if (*ePtr != '\0' || (num != 1 && num != 2))
  {
    std::cerr << "Error first argument\n";
    return 1;
  }

  const char * in  = argv[2];
  const char * out = argv[3];

  std::ifstream file(in);
  if (!file.is_open())
  {
    std::cerr << "Error cannot open file\n";
    return 1;
  }

  size_t rows = 0;
  size_t cols = 0;
  if (!(file >> rows >> cols))
  {
    std::cerr << "Error empty or invalid file\n";
    return 1;
  }
  if (rows == 0 || cols == 0)
  {
    return 0;
  }

  const size_t max_rows = 100;
  const size_t max_cols = 100;
  int * matrix = nullptr;
  int fixed_matrix[max_rows * max_cols] = {0};
  bool isDynamic = false;

  if (num == 1)
  {
    if (rows > max_rows || cols > max_cols)
    {
      std::cerr << "Error: matrix size exceeds fixed array limits\n";
      return 1;
    }
    matrix = fixed_matrix;
    isDynamic = false;
  }
  else if (num == 2)
  {
    try
    {
      matrix = new int[rows * cols];
      isDynamic = true;
    }
    catch (const std::bad_alloc &)
    {
      std::cerr << "Error memory allocation failed\n";
      return 1;
    }
  }

  if (!matveev::rMatrix(file, matrix, rows, cols))
  {
    std::cerr << "Error not enough data\n";
    if (isDynamic)
    {
      delete[] matrix;
    }
    return 1;
  }

  matveev::spiral(matrix, rows, cols);
  size_t MaxSeq = matveev::find(matrix, rows, cols);

  std::ofstream oFile(out);
  if (!oFile.is_open())
  {
    std::cerr << "Error cannot open output file\n";
    if (isDynamic)
    {
      delete[] matrix;
    }
    return 1;
  }

  matveev::wMatrix(oFile, matrix, rows, cols);
  oFile << '\n' << MaxSeq << '\n';

  if (isDynamic)
  {
    delete[] matrix;
  }
  return 0;
}

std::istream & matveev::rMatrix(std::istream & in, int * matrix, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return in;
  }

  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = 0; j < cols; ++j)
    {
      if (!(in >> matrix[i * cols + j]))
      {
        return in;
      }
    }
  }
  return in;
}

std::ostream & matveev::wMatrix(std::ostream & out, const int * matrix, size_t rows, size_t cols)
{
  out << rows << ' ' << cols;
  for (size_t i = 0; i < rows * cols; ++i)
  {
    out << ' ' << matrix[i];
  }
  return out;
}

void matveev::spiral(int * matrix, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return;
  }

  size_t upR = 0;
  size_t lC = 0;
  size_t dR = rows - 1;
  size_t rC = cols - 1;
  int cnt = 1;

  while (upR <= dR && lC <= rC)
  {
    for (size_t i = lC; i <= rC; ++i)
    {
      matrix[dR * cols + i] += cnt++;
    }
    if (dR == 0)
    {
      break;
    }
    dR--;
    if (lC <= rC)
    {
      for (size_t i = dR; i >= upR && i < rows; --i)
      {
        matrix[i * cols + rC] += cnt++;
        if (i == 0)
        {
          break;
        }
      }
      if (rC == 0)
      {
        break;
      }
      rC--;
    }

    if (upR <= dR)
    {
      for (size_t i = rC; i >= lC && i < cols; --i)
      {
        matrix[upR * cols + i] += cnt++;
        if (i == 0)
        {
          break;
        }
      }
      upR++;
    }
    if (lC <= rC)
    {
      for (size_t i = upR; i <= dR; ++i)
      {
        matrix[i * cols + lC] += cnt++;
      }
      lC++;
    }
  }
}

size_t matveev::find(const int * matrix, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return 0;
  }

  size_t maxLen = 0;
  size_t res = 0;

  for (size_t col = 0; col < cols; ++col)
  {
    size_t ser = 1;
    for (size_t row = 1; row < rows; ++row)
    {
      if (matrix[row * cols + col] == matrix[(row - 1) * cols + col])
      {
        ser++;
      }
      else
      {
        if (ser > maxLen)
        {
          maxLen = ser;
          res = col;
        }
        ser = 1;
      }
    }
    if (ser > maxLen)
    {
      maxLen = ser;
      res = col;
    }
  }
  return res;
}
