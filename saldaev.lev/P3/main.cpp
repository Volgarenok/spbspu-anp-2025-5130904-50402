#include <iostream>
#include <fstream>

namespace saldaev
{
  const size_t Max_size = 10000;
  size_t doCntRowNsm(const long long *matrix, size_t rows, size_t cols)
  {
    size_t count = 0;
    for (size_t r = 0; r < rows; ++r)
    {
      bool flag = true;
      for (size_t c = 1; c < cols; ++c)
      {
        if (matrix[r * cols + c] == matrix[r * cols + c - 1])
        {
          flag = false;
          break;
        }
      }
      if (flag)
      {
        ++count;
      }
    }
    return count;
  }

  void doLftBotClk(long long *matrix, size_t rows, size_t cols)
  {
    if (cols * rows == 0)
    {
      return;
    }
    size_t x = 0;
    size_t y = rows - 1;
    size_t c = 0;
    size_t top = 0;
    size_t bottom = rows - 1;
    size_t left = 0;
    size_t right = cols - 1;
    bool flag = true;
    while (flag)
    {
      flag = false;
      for (; y > top; --y)
      {
        flag = true;
        c++;
        matrix[y * cols + x] -= c;
      }
      left++;

      for (; x < right; ++x)
      {
        flag = true;
        c++;
        matrix[y * cols + x] -= c;
      }
      top++;

      for (; y < bottom; ++y)
      {
        flag = true;
        c++;
        matrix[y * cols + x] -= c;
      }
      right--;

      for (; x > left; --x)
      {
        flag = true;
        c++;
        matrix[y * cols + x] -= c;
      }
      bottom--;
    }
    c++;
    matrix[y * cols + x] -= c;
  }
}

int main(int argc, char **argv)
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }

  size_t mode = 0;
  if (argv[1][0] == '1' && argv[1][1] == '\0')
  {
    mode = 1;
  }
  else if (argv[1][0] == '2' && argv[1][1] == '\0')
  {
    mode = 2;
  }
  else
  {
    std::cerr << "First parameter is unacceptable\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open())
  {
    std::cerr << "Cant open input file\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;
  input >> rows >> cols;
  if (input.fail())
  {
    std::cerr << "data from file is unacceptable\n";
    return 2;
  }

  long long fixed_length_Matrix[saldaev::Max_size] = {};
  long long *matrix = nullptr;
  if (mode == 1)
  {
    matrix = fixed_length_Matrix;
  }
  else
  {
    matrix = new long long[rows * cols];
  }

  for (size_t i = 0; i < rows * cols; ++i)
  {
    input >> matrix[i];
  }
  if (input.fail())
  {
    std::cerr << "data from file is unacceptable\n";
    if (mode == 2)
    {
      delete[] matrix;
    }
    return 2;
  }
  input.close();

  size_t res1 = saldaev::doCntRowNsm(matrix, rows, cols);
  saldaev::doLftBotClk(matrix, rows, cols);

  std::ofstream output(argv[3]);
  if (!output.is_open())
  {
    std::cerr << "Cant open output file\n";
    return 2;
  }

  output << res1 << '\n' << rows << ' ' << cols;
  for (size_t i = 0; i < rows * cols; ++i)
  {
    output << ' ' << matrix[i];
  }

  if (mode == 2)
  {
    delete[] matrix;
  }
}
