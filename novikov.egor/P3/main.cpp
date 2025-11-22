#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

namespace novikov
{
  const size_t MAX_FIXED_SIZE = 10000;
  const size_t MAX_LINE_LENGTH = 10000;
  const size_t MAX_NUMBERS = 10002;

  bool isNumber(const char* str)
  {
    if (str == nullptr || str[0] == '\0')
    {
      return false;
    }
    size_t start = 0;
    if (str[0] == '-')
    {
      if (str[1] == '\0')
      {
        return false;
      }
      start = 1;
    }
    for (size_t i = start; str[i] != '\0'; i++)
    {
      if (!std::isdigit(str[i]))
      {
        return false;
      }
    }
    return true;
  }

  size_t getIndex(size_t i, size_t j, size_t cols)
  {
    return i * cols + j;
  }

  bool parseLine(const char* line, int* numbers, size_t* numbers_count)
  {
    *numbers_count = 0;
    if (line == nullptr || line[0] == '\0')
    {
      return false;
    }
    size_t start_pos = 0;
    size_t end_pos = 0;
    while (line[start_pos] != '\0' && *numbers_count < MAX_NUMBERS)
    {
      while (line[start_pos] == ' ')
      {
        start_pos++;
      }
      if (line[start_pos] == '\0')
      {
        break;
      }
      end_pos = start_pos;
      while (line[end_pos] != ' ' && line[end_pos] != '\0')
      {
        end_pos++;
      }
      size_t length = end_pos - start_pos;
      char buffer[32];
      if (length >= 32)
      {
        return false;
      }
      for (size_t i = 0; i < length; i++)
      {
        buffer[i] = line[start_pos + i];
      }
      buffer[length] = '\0';
      if (!isNumber(buffer))
      {
        return false;
      }
      numbers[*numbers_count] = std::atoi(buffer);
      (*numbers_count)++;
      start_pos = end_pos;
    }
    return true;
  }

  bool readMatrixFromFile(std::ifstream& file, int* matrix_data, size_t* rows, size_t* cols)
  {
    char line[MAX_LINE_LENGTH];
    if (!file.getline(line, MAX_LINE_LENGTH))
    {
      return false;
    }
    *rows = 0;
    *cols = 0;
    int numbers[MAX_NUMBERS];
    size_t numbers_count = 0;
    if (!parseLine(line, numbers, &numbers_count))
    {
      return false;
    }
    if (numbers_count < 2)
    {
      return false;
    }
    if (numbers[0] < 0 || numbers[1] < 0)
    {
      return false;
    }
    *rows = static_cast<size_t>(numbers[0]);
    *cols = static_cast<size_t>(numbers[1]);
    size_t expected_elements = *rows * *cols;
    if (numbers_count != 2 + expected_elements)
    {
      return false;
    }

    if (matrix_data == nullptr)
    {
      return false;
    }

    size_t index = 0;
    for (size_t i = 0; i < *rows; i++)
    {
      for (size_t j = 0; j < *cols; j++)
      {
        matrix_data[getIndex(i, j, *cols)] = numbers[2 + index];
        index++;
      }
    }
    return true;
  }

  void freeMatrix(int* matrix_data, bool use_fixed)
  {
    if (matrix_data == nullptr)
    {
      return;
    }
    if (use_fixed)
    {
      delete[] matrix_data;
    }
    else
    {
      std::free(matrix_data);
    }
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
            int neighbor_i = static_cast<int>(i) + di;
            int neighbor_j = static_cast<int>(j) + dj;
            if (neighbor_i >= 0 && neighbor_i < static_cast<int>(rows) &&
              neighbor_j >= 0 && neighbor_j < static_cast<int>(cols))
            {
              int neighbor = matrix_data[getIndex(static_cast<size_t>(neighbor_i),
                static_cast<size_t>(neighbor_j), cols)];
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
    while (current_value <= static_cast<int>(total_elements))
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
    file << local_maxima_count << std::endl;
    file << rows << " " << cols;
    for (size_t i = 0; i < rows; i++)
    {
      for (size_t j = 0; j < cols; j++)
      {
        file << " " << matrix_data[getIndex(i, j, cols)];
      }
    }
    file << std::endl;
    return file.good();
  }
}

int main(int argc, char* argv[])
{
  using namespace novikov;
  if (argc != 4)
  {
    std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << std::endl;
    return 1;
  }
  const char* num_str = argv[1];
  const char* input_file = argv[2];
  const char* output_file = argv[3];
  if (!isNumber(num_str))
  {
    std::cerr << "First parameter is not a number" << std::endl;
    return 1;
  }
  int num = std::atoi(num_str);
  if (num != 1 && num != 2)
  {
    std::cerr << "First parameter is out of range" << std::endl;
    return 1;
  }
  bool use_fixed = (num == 1);
  size_t rows = 0;
  size_t cols = 0;
  std::ifstream input_stream(input_file);
  if (!input_stream.is_open())
  {
    std::cerr << "Cannot open input file" << std::endl;
    return 2;
  }
  char line[MAX_LINE_LENGTH];
  if (!input_stream.getline(line, MAX_LINE_LENGTH))
  {
    std::cerr << "Invalid input file content" << std::endl;
    input_stream.close();
    return 2;
  }
  int numbers[MAX_NUMBERS];
  size_t numbers_count = 0;
  if (!parseLine(line, numbers, &numbers_count) || numbers_count < 2)
  {
    std::cerr << "Invalid input file content" << std::endl;
    input_stream.close();
    return 2;
  }
  if (numbers[0] < 0 || numbers[1] < 0)
  {
    std::cerr << "Invalid input file content" << std::endl;
    input_stream.close();
    return 2;
  }
  rows = static_cast<size_t>(numbers[0]);
  cols = static_cast<size_t>(numbers[1]);
  size_t expected_elements = rows * cols;
  if (numbers_count != 2 + expected_elements)
  {
    std::cerr << "Invalid input file content" << std::endl;
    input_stream.close();
    return 2;
  }
  if (use_fixed && expected_elements > MAX_FIXED_SIZE)
  {
    std::cerr << "Matrix too large for fixed array" << std::endl;
    input_stream.close();
    return 2;
  }
  int* matrix_data = nullptr;
  if (use_fixed)
  {
    matrix_data = new int[rows * cols];
  }
  else
  {
    matrix_data = static_cast<int*>(std::malloc(rows * cols * sizeof(int)));
    if (matrix_data == nullptr)
    {
      std::cerr << "Memory allocation failed" << std::endl;
      input_stream.close();
      return 2;
    }
  }
  input_stream.clear();
  input_stream.seekg(0);
  if (!readMatrixFromFile(input_stream, matrix_data, &rows, &cols))
  {
    std::cerr << "Invalid input file content" << std::endl;
    input_stream.close();
    freeMatrix(matrix_data, use_fixed);
    return 2;
  }
  input_stream.close();
  int local_maxima_count = countLocalMaxima(matrix_data, rows, cols);
  spiralTransform(matrix_data, rows, cols);
  std::ofstream output_stream(output_file);
  if (!output_stream.is_open())
  {
    std::cerr << "Cannot open output file" << std::endl;
    freeMatrix(matrix_data, use_fixed);
    return 3;
  }
  if (!writeResultsToFile(output_stream, matrix_data, rows, cols, local_maxima_count))
  {
    std::cerr << "Error writing to output file" << std::endl;
    output_stream.close();
    freeMatrix(matrix_data, use_fixed);
    return 3;
  }
  output_stream.close();
  freeMatrix(matrix_data, use_fixed);
  return 0;
}
