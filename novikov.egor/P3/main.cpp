#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

namespace novikov
{
  const size_t MAX_FIXED_SIZE = 10000;

  size_t getIndex(size_t i, size_t j, size_t cols)
  {
    return i * cols + j;
  }

  int countLocalMaxima(const int* matrix_data, size_t rows, size_t cols)
  {
    if (rows < 3 || cols < 3)
    {
      return 0;
    }
    int count = 0;
    for (size_t i = 1; i < rows - 1; i++)
    {
      for (size_t j = 1; j < cols - 1; j++)
      {
        int current = matrix_data[getIndex(i, j, cols)];
        bool is_maxima = true;
        for (int di = -1; di <= 1; di++)
        {
          for (int dj = -1; dj <= 1; dj++)
          {
            if (di == 0 && dj == 0)
            {
              continue;
            }
            int neighbor_i = static_cast < int > (i) + di;
            int neighbor_j = static_cast < int > (j) + dj;
            if (neighbor_i >= 0 && neighbor_i < static_cast < int >(rows) &&
              neighbor_j >= 0 && neighbor_j < static_cast < int >(cols))
            {
              int neighbor = matrix_data[getIndex(static_cast < size_t >(neighbor_i),
                static_cast < size_t >(neighbor_j), cols)];
              if (current <= neighbor)
              {
                is_maxima = false;
                break;
              }
            }
          }
          if (!is_maxima)
          {
            break;
          }
        }
        if (is_maxima)
        {
          count++;
        }
      }
    }
    return count;
  }

  void spiralTransform(int* matrix_data, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return;
    }
    size_t total_elements = rows * cols;
    int current_value = 1;
    size_t top = rows - 1;
    size_t bottom = 0;
    size_t left = 0;
    size_t right = cols - 1;
    int direction = 0;
    size_t i = top;
    size_t j = left;
    while (current_value <= static_cast < int > (total_elements))
    {
      matrix_data[getIndex(i, j, cols)] -= current_value;
      current_value++;
      switch (direction)
      {
      case 0:
        if (j < right)
        {
          j++;
        }
        else
        {
          direction = 1;
          i--;
        }
        break;
      case 1:
        if (i > bottom)
        {
          i--;
        }
        else
        {
          direction = 2;
          j--;
        }
        break;
      case 2:
        if (j > left)
        {
          j--;
        }
        else
        {
          direction = 3;
          i++;
        }
        break;
      case 3:
        if (i < top - 1)
        {
          i++;
        }
        else
        {
          direction = 0;
          top--;
          bottom++;
          left++;
          right--;
          i = top;
          j = left;
        }
        break;
      }
    }
  }

  bool writeResultsToFile(std::ofstream& file, const int* matrix_data, size_t rows, size_t cols, int local_maxima_count)
  {
    file << local_maxima_count;
    file << "\n";
    file << rows;
    file << " ";
    file << cols;
    for (size_t i = 0; i < rows; i++)
    {
      for (size_t j = 0; j < cols; j++)
      {
        file << " ";
        file << matrix_data[getIndex(i, j, cols)];
      }
    }
    file << "\n";
    return file.good();
  }
}

int main(int argc, char* argv[])
{
  using namespace novikov;
  if (argc != 4)
  {
    std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << "\n";
    return 1;
  }
  const char* num_str = argv[1];
  const char* input_file = argv[2];
  const char* output_file = argv[3];
  if (num_str == nullptr || num_str[0] == '\0')
  {
    std::cerr << "First parameter is not a number" << "\n";
    return 1;
  }
  bool is_negative = (num_str[0] == '-');
  size_t start = (is_negative ? 1 : 0);
  if (num_str[start] == '\0')
  {
    std::cerr << "First parameter is not a number" << "\n";
    return 1;
  }
  for (size_t i = start; num_str[i] != '\0'; i++)
  {
    if (!std::isdigit(num_str[i]))
    {
      std::cerr << "First parameter is not a number" << "\n";
      return 1;
    }
  }
  int num = 0;
  for (size_t i = start; num_str[i] != '\0'; i++)
  {
    num = num * 10 + (num_str[i] - '0');
  }
  if (is_negative)
  {
    num = -num;
  }
  if (num != 1 && num != 2)
  {
    std::cerr << "First parameter is out of range" << "\n";
    return 1;
  }
  bool use_fixed = (num == 1);
  std::ifstream input_stream(input_file);
  if (!input_stream.is_open())
  {
    std::cerr << "Cannot open input file" << "\n";
    return 2;
  }
  size_t rows = 0;
  size_t cols = 0;
  input_stream >> rows >> cols;
  if (input_stream.fail())
  {
    std::cerr << "Invalid input file content" << "\n";
    input_stream.close();
    return 2;
  }
  else if (rows == 0 || cols == 0)
  {
    std::cerr << "Invalid input file content" << "\n";
    input_stream.close();
    return 2;
  }
  if (use_fixed && rows * cols > MAX_FIXED_SIZE)
  {
    std::cerr << "Matrix too large for fixed array" << "\n";
    input_stream.close();
    return 2;
  }
  int* matrix_data = nullptr;
  int fixed_matrix[MAX_FIXED_SIZE];
  if (use_fixed)
  {
    matrix_data = fixed_matrix;
  }
  else
  {
    size_t size_bytes = rows * cols * sizeof(int);
    if (rows > 0 && cols > 0 && size_bytes / sizeof(int) != rows * cols)
    {
      std::cerr << "Memory allocation failed" << "\n";
      input_stream.close();
      return 2;
    }
    matrix_data = static_cast < int* > (std::malloc(size_bytes));
    if (matrix_data == nullptr)
    {
      std::cerr << "Memory allocation failed" << "\n";
      input_stream.close();
      return 2;
    }
  }
  for (size_t i = 0; i < rows * cols; i++)
  {
    if (!(input_stream >> matrix_data[i]))
    {
      std::cerr << "Invalid input file content" << "\n";
      input_stream.close();
      if (!use_fixed)
      {
        std::free(matrix_data);
      }
      return 2;
    }
  }
  if (input_stream.peek() != std::ifstream::traits_type::eof())
  {
    std::cerr << "Invalid input file content" << "\n";
    input_stream.close();
    if (!use_fixed)
    {
      std::free(matrix_data);
    }
    return 2;
  }
  input_stream.close();
  int local_maxima_count = countLocalMaxima(matrix_data, rows, cols);
  spiralTransform(matrix_data, rows, cols);
  std::ofstream output_stream(output_file);
  if (!output_stream.is_open())
  {
    std::cerr << "Cannot open output file" << "\n";
    if (!use_fixed)
    {
      std::free(matrix_data);
    }
    return 3;
  }
  if (!writeResultsToFile(output_stream, matrix_data, rows, cols, local_maxima_count))
  {
    std::cerr << "Error writing to output file" << "\n";
    output_stream.close();
    if (!use_fixed)
    {
      std::free(matrix_data);
    }
    return 3;
  }
  output_stream.close();
  if (!use_fixed)
  {
    std::free(matrix_data);
  }
  return 0;
}
