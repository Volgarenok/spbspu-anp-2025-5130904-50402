#include <iostream>
#include <fstream>

namespace dirko
{
  void staticInput(std::istream &input, int *matrix, const size_t rows, const size_t cols)
  {
    for (size_t i = 0; i < rows * cols; ++i)
    {
      input >> matrix[i];
      if (input.eof())
      {
        throw std::logic_error("Not enougth data\n");
      }
      else if (input.fail())
      {
        throw std::logic_error("Cant read\n");
      }
    }
  }
  int *dinamicInput(std::istream &input, const size_t rows, const size_t cols)
  {
    try
    {
      int *matrix = new int[rows * cols];
      for (size_t i = 0; i < rows * cols; ++i)
      {
        input >> matrix[i];
        if (input.eof())
        {
          delete[] matrix;
          throw std::logic_error("Not enougth data\n");
        }
        else if (input.fail())
        {
          delete[] matrix;
          throw std::logic_error("Cant read");
        }
      }
      return matrix;
    }
    catch (std::bad_alloc &e)
    {
      throw;
    }
  }
  int *LFT_BOT_CLK(const int *matrix, const size_t rows, const size_t cols)
  {
    const size_t elements = rows * cols;
    if (elements == 0)
    {
      return nullptr;
    }
    int *result = nullptr;
    try
    {
      result = new int[elements];
    }
    catch (const std::bad_alloc &e)
    {
      throw;
    }
    for (size_t i = 0; i < elements; ++i)
    {
      result[i] = matrix[i];
    }
    size_t decrement = 1;
    size_t left = 0, right = cols - 1;
    size_t top = 0, bottom = rows - 1;
    while (top <= bottom && left <= right)
    {
      if (left <= right)
      {
        for (size_t i = bottom + 1; i > top; --i)
        {
          result[(i - 1) * cols + left] -= decrement++;
        }
        left++;
      }
      for (size_t j = left; j <= right; ++j)
      {
        result[top * cols + j] -= decrement++;
      }
      top++;
      for (size_t i = top; i <= bottom; ++i)
      {
        result[i * cols + right] -= decrement++;
      }
      right--;
      if (top <= bottom)
      {
        for (size_t j = right + 1; j > left; --j)
        {
          result[bottom * cols + j - 1] -= decrement++;
        }
        bottom--;
      }
    }
    return result;
  }
  bool LWR_TRI_MTX(const int *matrix, const size_t rows, const size_t cols)
  {
    const size_t min = (rows > cols) ? cols : rows;
    if (min < 2)
    {
      return false;
    }
    size_t line = 0;
    for (size_t i = 0; i < min; ++i)
    {
      for (size_t j = line + i + 1; j < line + min; j++)
      {
        if (matrix[i + j] != 0)
        {
          return false;
        }
      }
      line += min - 1;
    }
    return true;
  }
  std::ostream &output(std::ostream &output, const int *matrix, const size_t rows, const size_t cols, const bool result2)
  {
    output << rows << ' ' << cols << ' ';
    for (size_t i = 0; i < rows * cols; ++i)
    {
      output << matrix[i] << ' ';
    }
    output << '\n';
    return output << std::boolalpha << result2;
  }
}

int main(int argc, char const **argv)
{
  int mode = 0;
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
  try
  {
    mode = std::stoi(argv[1]);
  }
  catch (std::out_of_range &e)
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  if (mode < 1 || mode > 2)
  {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }
  std::ifstream fin(argv[2]);
  if (!fin.is_open())
  {
    std::cerr << "Cant open input file\n";
    return 2;
  }
  size_t rows = 0, cols = 0;
  fin >> rows >> cols;
  if (fin.eof())
  {
    std::cerr << "Not enougth data\n";
    return 2;
  }
  else if (fin.fail())
  {
    std::cerr << "Cant read\n";
    return 2;
  }
  int *result1 = nullptr;
  bool result2 = false;
  if (mode == 1)
  {
    int matrix[1000]{};
    try
    {
      dirko::staticInput(fin, matrix, rows, cols);
      fin.close();
      result1 = dirko::LFT_BOT_CLK(matrix, rows, cols);
      result2 = dirko::LWR_TRI_MTX(matrix, rows, cols);
    }
    catch (std::logic_error &e)
    {
      std::cerr << e.what();
      delete[] result1;
      return 2;
    }
    std::ofstream fout(argv[3]);
    dirko::output(fout, result1, rows, cols, result2) << '\n';
  }
  else
  {
    int *matrix = nullptr;
    try
    {
      matrix = dirko::dinamicInput(fin, rows, cols);
    }
    catch (const std::bad_alloc &e)
    {
      std::cerr << "Cant alloc\n";
      return 3;
    }
    catch (std::logic_error &e)
    {
      delete[] matrix;
      std::cerr << e.what();
      return 2;
    }
    try
    {
      fin.close();
      result1 = dirko::LFT_BOT_CLK(matrix, rows, cols);
      result2 = dirko::LWR_TRI_MTX(matrix, rows, cols);
    }
    catch (std::bad_alloc &e)
    {
      delete[] matrix;
      std::cerr << "Cant alloc\n";
      return 3;
    }
    std::ofstream fout(argv[3]);
    dirko::output(fout, result1, rows, cols, result2) << '\n';
    delete[] matrix;
  }
  delete[] result1;
}
