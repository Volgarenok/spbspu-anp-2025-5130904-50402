#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>

namespace pozdnyakov
{
  const size_t MAX_ELEMENTS = 10000;
  const size_t MAX_ROWS = 100;
  const size_t MAX_COLS = 100;

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

int main(int argc, char* argv[])
{
  using namespace pozdnyakov;

  if (argc != 4)
  {
    std::cerr << (argc < 4 ? "Not enough arguments\n" : "Too many arguments\n");
    return 1;
  }

  if (!validateArgs(argv[1]))
  {
    std::cerr << "Invalid first argument\n";
    return 1;
  }

  long mode = std::strtol(argv[1], nullptr, 10);
  const char* inputFile = argv[2];
  const char* outputFile = argv[3];

  std::ifstream in(inputFile);
  if (!in.is_open())
  {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;

  if (!readDimensions(in, rows, cols))
  {
    std::cerr << "Invalid matrix dimensions\n";
    return 2;
  }

  if (rows == 0 || cols == 0)
  {
    std::ofstream out(outputFile);
    if (!out.is_open())
    {
      std::cerr << "Cannot open output file\n";
      return 3;
    }
    out << 0 << '\n';
    out << 0 << ' ' << 0 << '\n';
    return 0;
  }

  if (mode == 1)
  {
    if (rows * cols > MAX_ELEMENTS || rows > MAX_ROWS || cols > MAX_COLS)
    {
      std::cerr << "Matrix size exceeds limits\n";
      return 2;
    }
  }

  int fixedData[MAX_ROWS * MAX_COLS] = {0};
  int* dataPtr = nullptr;

  if (mode == 1)
  {
    dataPtr = fixedData;
  }
  else
  {
    dataPtr = reinterpret_cast< int* >(std::malloc(rows * cols * sizeof(int)));
    if (dataPtr == nullptr)
    {
      std::cerr << "Memory allocation failed\n";
      return 2;
    }
  }

  if (!readMatrix(in, dataPtr, rows, cols))
  {
    std::cerr << "Invalid matrix data\n";
    if (mode == 2)
    {
      std::free(dataPtr);
    }
    return 2;
  }

  size_t diagCount = countDiagonalsWithoutZero(dataPtr, rows, cols);
  transformMatrixLayers(dataPtr, rows, cols);

  std::ofstream out(outputFile);
  if (!out.is_open())
  {
    std::cerr << "Cannot open output file\n";
    if (mode == 2)
    {
      std::free(dataPtr);
    }
    return 3;
  }

  out << diagCount << '\n';
  writeMatrix(out, dataPtr, rows, cols);

  if (mode == 2)
  {
    std::free(dataPtr);
  }

  return 0;
}
