#include <iostream>
#include <fstream>
#include <cstring>
namespace chernikov
{
  bool isDownTriangleMatrix(const int * a, size_t rows, size_t cols);
  size_t localMaxQuantity(const int * a, size_t rows, size_t cols);
  bool isParNum(const char * a);
}

int main(int argc, char ** argv)
{
  if (argc < 4 || argc > 4)
  {
    std::cerr << "Mismatch in the number of parameters\n";
    return 1;
  }
  if (!chernikov::isParNum(argv[1]))
  {
    std::cerr << "memory parameter is not a number\n";
    return 1;
  }
  size_t rows = 0, cols = 0;
  std::ifstream input(argv[2]);
  if (!input || !(input >> rows >> cols))
  {
    std::cerr << "Array cannot exist\n";
    return 2;
  }
  const int MAX_SIZE = 10000;
  if ((rows * cols) > MAX_SIZE)
  {
    std::cerr << "Array incorect\n";
    return 2;
  }
  if (rows == 0 && cols == 0)
  {
    std::ofstream output(argv[3]);
    if (!output || !(output << '0' << '\n' << '0' << '\n'))
    {
      std::cerr << "Parameters cannot output\n";
      return 2;
    }
    return 0;
  }
  bool isArrayDynamic = 0;
  int * array = nullptr;
  int nums[MAX_SIZE] = {};
  if (std::strcmp(argv[1], "1") == 0)
  {
    array = nums;
    isArrayDynamic = 0;
  }
  else if (std::strcmp(argv[1], "2") == 0)
  {
    array = reinterpret_cast< int * >(malloc(sizeof(int) * (rows * cols)));
    if (array == nullptr)
    {
      std::cerr << "Memory allocation failed\n";
      return 2;
    }
    isArrayDynamic = 1;
  }
  else
  {
    std::cerr << "Parameter 2 is set incorrectly\n";
    return 2;
  }
  for (size_t i = 0; i < (rows * cols); ++i)
  {
    if (!(input >> array[i]))
    {
      std::cerr << "Failed to count element\n";
      if (isArrayDynamic)
      {
        free(array);
      }
      return 2;
    }
  }
  input.close();

  bool lwr_tri_mtx = chernikov::isDownTriangleMatrix(array, rows, cols);
  size_t cnt_loc_max = chernikov::localMaxQuantity(array, rows, cols);
  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Parameters cannot output\n";
    return 2;
  }
  output << "LWR_TRI_MTX = " << lwr_tri_mtx << "\n";
  output << "CNT_LOC_MAX = " << cnt_loc_max << "\n";
  if (isArrayDynamic)
  {
    free(array);
    array = nullptr;
  }
  return 0;
}

bool chernikov::isDownTriangleMatrix(const int * array, size_t rows, size_t cols)
{
  if (rows == 0 && cols == 0)
  {
    return 0;
  }
  if (array == nullptr)
  {
    return 0;
  }
  if (rows != cols)
  {
    return 0;
  }
  for (size_t i = 0; i < rows; ++i)
  {
    for (size_t j = i + 1; j < cols; ++j)
    {
      size_t index = i * cols + j;
      if (array[index] != 0)
      {
        return 0;
      }
    }
  }
  return 1;
}

size_t chernikov::localMaxQuantity(const int * array, size_t rows, size_t cols)
{
  if (rows < 3 || cols < 3)
  {
    return 0;
  }
  if (array == nullptr)
  {
    return 0;
  }
  size_t count = 0;
  for (size_t i = 1; i < (rows - 1); ++i)
  {
    for (size_t j = 1; j < (cols - 1); ++j)
    {
      int t = array[j + i * cols];
      if (t > array[(j - 1) + (i - 1) * cols]
            && t > array[j + (i - 1) * cols]
            && t > array[(j + 1) + (i - 1) * cols]
            && t > array[(j - 1) + i * cols]
            && t > array[(j + 1) + i * cols]
            && t > array[(j - 1) + (i + 1) * cols]
            && t > array[j + (i + 1) * cols]
            && t > array[(j + 1) + (i + 1) * cols])
      {
        count++;
      }
    }
  }
  return count;
}

bool chernikov::isParNum(const char * array)
{
  if (array == nullptr)
  {
    return 0;
  }
  return (array[0] == '1' || array[0] == '2') && array[1] == '\0';
}
