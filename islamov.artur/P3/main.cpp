#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <new>
namespace islamov
{
  int colsdiffnumbers(const int* arr, size_t rows, size_t cols);
  int zeroChecker(const int* arr, size_t rows, size_t cols);
  std::istream& matrixReader(std::istream& in, int* arr, size_t totalElements);
}
int main(int argc, char** argv)
{
  if (argc != 4)
  {
    std::cerr << "Error: Incorrect arguments\n";
    return 1;
  }
  char* endptr = nullptr;
  long mode = std::strtol(argv[1], std::addressof(endptr), 10);
  if (endptr == argv[1] || *endptr != '\0' || mode < 1 || mode > 2)
  {
    std::cerr << "Error: first parameter is not a valid number (must be 1 or 2)\n";
    return 1;
  }
  const char* inputName = argv[2];
  const char* outputName = argv[3];
  std::ifstream fin(inputName, std::ios::binary);
  if (!fin)
  {
    std::cerr << "Error: cannot open input file: " << inputName << "\n";
    return 2;
  }
  size_t rows = 0;
  size_t cols = 0;
  if (!(fin >> rows >> cols))
  {
    std::cerr << "Error: input file content is not a valid matrix\n";
    return 2;
  }
  if (rows != 0 && cols > std::numeric_limits< size_t >::max() / rows)
  {
    std::cerr << "Error: matrix too large\n";
    return 2;
  }
  const size_t totalElements = rows * cols;
  const size_t max_size = 10000;
  if (mode == 1 && totalElements > max_size)
  {
    std::cerr << "Error: matrix too large for static mode\n";
    return 1;
  }
  int* arr = nullptr;
  int* dynDelete = nullptr;
  int stackArr[max_size] = {0};
  try
  {
    if (mode == 2)
    {
      arr = new int[totalElements]();
      dynDelete = arr;
    }
    else
    {
      arr = stackArr;
    }
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Error: cannot allocate memory for matrix\n";
    return 2;
  }
  islamov::matrixReader(fin, arr, totalElements);
  if (fin.fail())
  {
    std::cerr << "Error: input file content is not a valid matrix\n";
    delete[] dynDelete;
    return 2;
  }
  const int res1 = islamov::colsdiffnumbers(arr, rows, cols);
  const int res2 = islamov::zeroChecker(arr, rows, cols);
  std::ofstream fout(outputName, std::ios::binary);
  if (!fout)
  {
    std::cerr << "Error: cannot open output file: " << outputName << "\n";
    delete[] dynDelete;
    return 2;
  }
  fout << res1 << '\n' << res2 << '\n';
  delete[] dynDelete;
  return 0;
}
int islamov::colsdiffnumbers(const int* arr, size_t rows, size_t cols)
{
  int count = 0;
  for (size_t j = 0; j < cols; ++j)
  {
    bool equal = false;
    for (size_t i = 0; i + 1 < rows; ++i)
    {
      if (arr[i * cols + j] == arr[(i + 1) * cols + j])
      {
        equal = true;
        break;
      }
    }
    if (!equal)
    {
      ++count;
    }
  }
  return count;
}
int islamov::zeroChecker(const int* arr, size_t rows, size_t cols)
{
  if (rows == 0 || cols == 0)
  {
    return 0;
  }
  int count = 0;
  const size_t totalDiagonals = rows + cols - 1;
  for (size_t diagIndex = 0; diagIndex < totalDiagonals; ++diagIndex)
  {
    bool zeroFound = false;
    size_t startRow = 0;
    size_t startCol = 0;
    if (diagIndex < rows)
    {
      startRow = rows - 1 - diagIndex;
      startCol = 0;
    }
    else
    {
      startRow = 0;
      startCol = diagIndex - rows + 1;
    }
    size_t i = startRow;
    size_t j = startCol;
    while (i < rows && j < cols)
    {
      if (arr[i * cols + j] == 0)
      {
        zeroFound = true;
        break;
      }
      ++i;
      ++j;
    }
    if (!zeroFound)
    {
      ++count;
    }
  }
  return count;
}
std::istream& islamov::matrixReader(std::istream& in, int* arr, size_t totalElements)
{
  for (size_t i = 0; i < totalElements; ++i)
  {
    if (!(in >> arr[i]))
    {
      return in;
    }
  }
  return in;
}
