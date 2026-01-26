#include <fstream>
#include <iostream>

namespace lavrentev
{
  size_t cntLocMin(const int* arr, size_t x, size_t y);
  size_t numColLsr(const int* arr, size_t x, size_t y, int** mas);
  std::istream& inputFile(std::istream& in, int* m, size_t lng);
}

int main(int argc, char** argv)
{
  const size_t staticSize = 10000;

  if (argc < 4)
  {
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }

  char* endPtr = nullptr;

  int num = std::strtol(argv[1], std::addressof(endPtr), 10);
  if (*endPtr != '\0')
  {
    std::cerr << "First parameter is not a number" << '\n';
    return 1;
  }
  if (num != 1 && num != 2)
  {
    std::cerr << "First parameter is out of range" << '\n';
    return 1;
  }

  std::ifstream input(argv[2]);

  if (!input.is_open())
  {
    std::cerr << "Uncorrect file" << '\n';
    return 2;
  }

  size_t x, y;

  if (!(input >> x >> y))
  {
    std::cerr << "Uncorrect file" << '\n';
    return 2;
  }

  size_t total = x * y;
  int arr2[staticSize];
  int* arr = nullptr;
  int* matrix = nullptr;

  if (num == 1)
  {
    matrix = arr2;
  }
  else
  {
    int total_for_memory = x * y;
    arr = reinterpret_cast< int* >(malloc(total_for_memory * sizeof(int)));
    if (arr == nullptr)
    {
      std::cerr << "Memory allocation fail" << "\n";
      return 3;
    }
    matrix = arr;
  }

  if (lavrentev::inputFile(input, matrix, total).fail())
  {
    std::cerr << "Couldn't read the matrix" << '\n';
    free(arr);
    return 2;
  }

  int** mas = reinterpret_cast< int** >(malloc(y * sizeof(int*)));
  if (mas == nullptr)
  {
    std::cerr << "Memory allocation fail for mas" << "\n";
    free(arr);
    return 3;
  }

  for (size_t i = 0; i < y; ++i)
  {
    mas[i] = reinterpret_cast< int* >(malloc(2 * sizeof(int)));
    if (mas[i] == nullptr)
    {
      std::cerr << "Memory allocation fail for mas[" << i << "]" << "\n";
      for (size_t j = 0; j < i; ++j)
      {
        free(mas[j]);
      }
      free(mas);
      free(arr);
      return 3;
    }
    mas[i][0] = -1;
    mas[i][1] = 0;
  }

  size_t ans2 = lavrentev::cntLocMin(matrix, x, y);
  size_t ans11 = lavrentev::numColLsr(matrix, x, y, mas);

  std::ofstream output(argv[3]);

  if (!output.is_open())
  {
    std::cerr << "Couldn't open output file" << '\n';
    for (size_t i = 0; i < y; ++i)
    {
      free(mas[i]);
    }
    free(mas);
    free(arr);
    return 4;
  }

  output << "Answer for var_2: " << ans2 << '\n';
  output << "Answer for var_11: " << ans11 << '\n';

  for (size_t i = 0; i < y; ++i)
  {
    free(mas[i]);
  }
  free(mas);
  free(arr);
}

size_t lavrentev::cntLocMin(const int* arr, size_t x, size_t y)
{
  size_t ans2 = 0;
  size_t total = x * y;

  for (size_t i = y + 1; i < total - y; ++i)
  {
    if ((i % y != 0) && (i % y != y - 1))
    {
      bool b1 = (arr[i] < arr[i + 1]) && (arr[i] < arr[i - 1]);
      b1 = b1 && (arr[i] < arr[i + y]) && (arr[i] < arr[i - y]);
      b1 = b1 && (arr[i] < arr[i + y + 1]) && (arr[i] < arr[i - y - 1]);
      b1 = b1 && (arr[i] < arr[i + y - 1]) && (arr[i] < arr[i - y + 1]);
      if (b1)
      {
        ++ans2;
      }
    }
  }

  return ans2;
}

size_t lavrentev::numColLsr(const int* arr, size_t x, size_t y, int** mas)
{
  int ans11 = 0;
  size_t total = x * y;
  int max_length = 0;

  for (size_t i = 0; i < y; ++i)
  {
    mas[i][0] = -1;
    mas[i][1] = 0;
  }

  for (size_t i = 0; i < total; ++i)
  {
    size_t j = i % y;
    if (mas[j][0] == -1)
    {
      mas[j][0] = j + 1;
      mas[j][1] = 1;
    }
    else
    {
      if (arr[i] == arr[i - y])
      {
        ++mas[j][1];
      }
      else
      {
        if (mas[j][1] > max_length)
        {
          max_length = mas[j][1];
        }
        mas[j][1] = 1;
      }
    }
  }

  for (size_t i = 0; i < y; ++i)
  {
    if (max_length < mas[i][1])
    {
      ans11 = mas[i][0];
      max_length = mas[i][1];
    }
  }

  return ans11;
}

std::istream& lavrentev::inputFile(std::istream& in, int* m, size_t lng)
{
  for (size_t i = 0; i < lng; i++)
  {
    in >> m[i];
    if (in.fail())
    {
      return in;
    }
  }
  return in;
}
