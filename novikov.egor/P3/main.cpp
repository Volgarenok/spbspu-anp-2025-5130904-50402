#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

namespace novikov 
{
  const int MAX_FIXED_SIZE = 10000;
  bool isNumber(const std::string& str) 
  {
    if (str.empty()) return false;
    size_t start = 0;
    if (str[0] == '-') {
      if (str.length() == 1) return false;
      start = 1;
    }
    for (size_t i = start; i < str.length(); i++) {
      if (!isdigit(str[i])) {
        return false;
      }
    }
    return true;
  }

  int getIndex(int i, int j, int cols) {
    return i * cols + j;
  }

  bool readMatrixFromFile(const std::string& filename, bool use_fixed, int*& matrix_data, int& rows, int& cols) 
  {
    std::ifstream file(filename);
    if (!file.is_open()) {
      return false;
    }
    std::string line;
    if (!std::getline(file, line)) {
      return false;
    }
    std::istringstream iss(line);
    std::vector<int> all_numbers;
    std::string token;
    while (iss >> token) {
      if (!isNumber(token)) {
        return false;
      }
      try {
        all_numbers.push_back(std::stoi(token));
      }
      catch (const std::exception& e) {
        return false;
      }
    }
    if (all_numbers.size() < 2) {
      return false;
    }
    rows = all_numbers[0];
    cols = all_numbers[1];
    if (rows < 0 || cols < 0) {
      return false;
    }
    int expected_elements = rows * cols;
    if (all_numbers.size() != 2 + expected_elements) {
      return false;
    }
    if (use_fixed && expected_elements > MAX_FIXED_SIZE) {
      return false;
    }
    try {
      if (use_fixed) {
        matrix_data = new int[rows * cols];
      }
      else {
        matrix_data = static_cast<int*>(malloc(rows * cols * sizeof(int)));
        if (!matrix_data) {
          throw std::bad_alloc();
        }
      }
    }
    catch (const std::bad_alloc& e) {
      return false;
    }
    int index = 0;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        matrix_data[getIndex(i, j, cols)] = all_numbers[2 + index];
        index++;
      }
    }
    return true;
  }

  void freeMatrix(int* matrix_data, bool use_fixed) 
  {
    if (!matrix_data) return;

    if (use_fixed) {
      delete[] matrix_data;
    }
    else {
      free(matrix_data);
    }
  }

  int countLocalMaxima(int* matrix_data, int rows, int cols) 
  {
    if (rows < 3 || cols < 3) {
      return 0;
    }
    int count = 0;
    for (int i = 1; i < rows - 1; i++) {
      for (int j = 1; j < cols - 1; j++) {
        int current = matrix_data[getIndex(i, j, cols)];
        bool is_maxima = true;
        for (int di = -1; di <= 1; di++) {
          for (int dj = -1; dj <= 1; dj++) {
            if (di == 0 && dj == 0) continue;
            int neighbor = matrix_data[getIndex(i + di, j + dj, cols)];
            if (current <= neighbor) {
              is_maxima = false;
              break;
            }
          }
          if (!is_maxima) break;
        }
        if (is_maxima) {
          count++;
        }
      }
    }
    return count;
  }

  void spiralTransform(int* matrix_data, int rows, int cols) 
  {
    if (rows == 0 || cols == 0) return;
    int total_elements = rows * cols;
    int current_value = 1;
    int top = rows - 1, bottom = 0, left = 0, right = cols - 1;
    int direction = 0;
    int i = top, j = left;
    while (current_value <= total_elements) {
      matrix_data[getIndex(i, j, cols)] -= current_value;
      current_value++;
      switch (direction) {
      case 0:
        if (j < right) j++;
        else { direction = 1; i--; }
        break;
      case 1:
        if (i > bottom) i--;
        else { direction = 2; j--; }
        break;
      case 2:
        if (j > left) j--;
        else { direction = 3; i++; }
        break;
      case 3:
        if (i < top - 1) i++;
        else {
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

  bool writeResultsToFile(const std::string& filename, int* matrix_data, int rows, int cols, int local_maxima_count) 
  {
    std::ofstream file(filename);
    if (!file.is_open()) {
      return false;
    }
    file << local_maxima_count << std::endl;
    file << rows << " " << cols;
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        file << " " << matrix_data[getIndex(i, j, cols)];
      }
    }
    file << std::endl;
    return true;
  }

}

int main(int argc, char* argv[]) 
{
  using namespace novikov;
  if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << std::endl;
    return 1;
  }
  std::string num_str = argv[1];
  std::string input_file = argv[2];
  std::string output_file = argv[3];
  if (!isNumber(num_str)) {
    std::cerr << "First parameter is not a number" << std::endl;
    return 1;
  }
  int num;
  try {
    num = std::stoi(num_str);
  }
  catch (const std::exception& e) {
    std::cerr << "First parameter is not a number" << std::endl;
    return 1;
  }
  if (num != 1 && num != 2) {
    std::cerr << "First parameter is out of range" << std::endl;
    return 1;
  }
  bool use_fixed = (num == 1);
  int* matrix_data = nullptr;
  int rows = 0, cols = 0;
  if (!readMatrixFromFile(input_file, use_fixed, matrix_data, rows, cols)) {
    std::cerr << "Invalid input file content" << std::endl;
    return 2;
  }
  int local_maxima_count = countLocalMaxima(matrix_data, rows, cols);
  spiralTransform(matrix_data, rows, cols);
  if (!writeResultsToFile(output_file, matrix_data, rows, cols, local_maxima_count)) {
    std::cerr << "Error writing to output file" << std::endl;
    freeMatrix(matrix_data, use_fixed);
    return 3;
  }
  freeMatrix(matrix_data, use_fixed);
  return 0;
}
