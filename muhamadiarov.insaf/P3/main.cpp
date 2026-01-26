#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>

namespace muhamadiarov
{
  int* fllIncWav(int* ptr, size_t rows, size_t colons);
  void outMatric(std::ofstream& output, size_t r, size_t c, const int* res1);
  long long toFindMaxRight(const int* ptr, size_t order);
  long long toFindMaxinLeft(const int* ptr, size_t order);
  long long maxSumMdg(const int* ptr, size_t order);
}

int main(int argc, char* argv[])
{
  namespace muh = muhamadiarov;
  if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  int mode = 0;
  if (std::strcmp(argv[1], "1") == 0)
  {
    mode = 1;
  }
  else if (std::strcmp(argv[1], "2") == 0)
  {
    mode = 2;
  }
  else
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Error is openning file\n";
    return 2;
  }
  size_t rows = 0;
  size_t colons = 0;
  input >> rows >> colons;
  if (input.eof() || input.fail())
  {
    std::cerr << "Error is openning file\n";
    return 2;
  }
  bool c1 = (rows != 0 && colons == 0) || (rows == 0 && colons != 0);
  bool c2 = rows * colons > 10000;
  if (c1 || c2)
  {
    std::cerr << "Wrong size of matric\n";
    return 2;
  }
  const int sizeMatric = 10000;
  int arr[sizeMatric];
  int* ptr = nullptr;
  if (rows == 0 && colons == 0)
  {
    std::ofstream output(argv[3], std::ios::app);
    muh::outMatric(output, rows, colons, ptr);
    output << '\n';
    output << 0 << '\n';
    return 0;
  }
  if (mode == 1)
  {
    ptr = arr;
  }
  else if (mode == 2)
  {
    ptr = reinterpret_cast< int* >(malloc(rows * colons * sizeof(int)));
    if (ptr == nullptr)
    {
      std::cerr << "Failed to allocate memory\n";
      return 1;
    }
  }
  for (size_t i = 0; i < rows * colons; ++i)
  {
    input >> ptr[i];
    if (input.eof())
    {
      std::cerr << "Not enough the numbers of elements\n";
      if (mode == 2)
      {
        free(ptr);
      }
      return 2;
    }
    else if (input.fail())
    {
      std::cerr << "Wrong working read\n";
      if (mode == 2)
      {
        free(ptr);
      }
      return 2;
    }
  }
  input.close();
  size_t order = std::min(rows, colons);
  long long int res2 = muh::maxSumMdg(ptr, order);
  ptr = muh::fllIncWav(ptr, rows, colons);
  std::ofstream output(argv[3], std::ios::app);
  if (!output)
  {
    std::cerr << "Error in opening file\n";
    if (mode == 2)
    {
      free(ptr);
    }
    return 2;
  }
  muh::outMatric(output, rows, colons, ptr);
  output << '\n';
  output << res2;
  output << '\n';
  if (mode == 2)
  {
    free(ptr);
  }
  return 0;
}

int* muhamadiarov::fllIncWav(int* ptr, size_t rows, size_t colons)
{
  for (size_t j = 0; j < rows; ++j)
  {
    for (size_t i = 0; i < colons; ++i)
    {
      size_t increase = 0;
      size_t top = j;
      size_t bottom = rows - 1 - j;
      size_t left = i;
      size_t right = colons - 1 - i;
      increase = std::min(std::min(top, bottom), std::min(left, right)) + 1;
      ptr[j * colons + i] += increase;
    }
  }
  return ptr;
}

long long muhamadiarov::maxSumMdg(const int* ptr, size_t order)
{
  namespace muh = muhamadiarov;
  long long max_result = 0;
  long long par_branch_right = muh::toFindMaxRight(ptr, order);
  long long par_branch_left = muh::toFindMaxinLeft(ptr, order);
  max_result = std::max(par_branch_right, par_branch_left);
  return max_result;
}

long long muhamadiarov::toFindMaxinLeft(const int* ptr, size_t order)
{
  long long max_r = 0;
  long long result = 0;
  size_t diag = 0;
  while (diag < order)
  {
    size_t i = diag;
    for (size_t j = 0; j <= diag; ++j)
    {
      result += ptr[j * order + i];
      if (i == 0)
      {
        break;
      }
      --i;
    }
    ++diag;
    max_r = std::max(result, max_r);
    result = 0;
  }
  return max_r;
}

long long muhamadiarov::toFindMaxRight(const int* ptr, size_t order)
{
  long long int max_r = 0;
  long long int result = 0;
  size_t diag = order - 1;
  while (diag > 0)
  {
    for (size_t i = order - 1, j = diag; j < order && i >= diag; --i, ++j)
    {
      result += ptr[j * order + i];
    }
    --diag;
    max_r = std::max(result, max_r);
    result = 0;
  }
  return max_r;
}

void muhamadiarov::outMatric(std::ofstream& output, size_t r, size_t c, const int* res1)
{
  output << r << ' ' << c;
  for (size_t i = 0; i < r * c; ++i)
  {
    output << ' ' << res1[i];
  }
}
