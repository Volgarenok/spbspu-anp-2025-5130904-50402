#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

namespace pozdnyakov {

  const int MAX_ELEMENTS = 10000;
  const int MAX_ROWS = 100;
  const int MAX_COLS = 100;

  struct Matrix {
    int** data;
    int rows;
    int cols;
  };

  // create matrix using malloc
  Matrix* create_matrix(int rows, int cols) {
    Matrix* matrix = static_cast<Matrix*>(std::malloc(sizeof(Matrix)));
    if (matrix == nullptr) return nullptr;

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = static_cast<int**>(std::malloc(rows * sizeof(int*)));

    if (matrix->data == nullptr) {
      std::free(matrix);
      return nullptr;
    }

    for (int i = 0; i < rows; i++) {
      matrix->data[i] = static_cast<int*>(std::malloc(cols * sizeof(int)));
      if (matrix->data[i] == nullptr) {
        for (int j = 0; j < i; j++) {
          std::free(matrix->data[j]);
        }
        std::free(matrix->data);
        std::free(matrix);
        return nullptr;
      }
    }
    return matrix;
  }

  // free matrix memory
  void free_matrix(Matrix* matrix) {
    if (matrix == nullptr) return;
    if (matrix->data != nullptr) {
      for (int i = 0; i < matrix->rows; i++) {
        std::free(matrix->data[i]);
      }
      std::free(matrix->data);
    }
    std::free(matrix);
  }

  // read matrix from file
  Matrix* read_matrix_from_file(const char* filename, bool use_fixed_size) {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Cannot open input file" << std::endl;
      return nullptr;
    }

    int rows, cols;
    if (!(file >> rows >> cols)) {
      std::cerr << "Invalid matrix dimensions" << std::endl;
      return nullptr;
    }

    if (use_fixed_size && (rows * cols > MAX_ELEMENTS)) {
      std::cerr << "Matrix size exceeds limits" << std::endl;
      return nullptr;
    }

    Matrix* matrix = create_matrix(rows, cols);
    if (matrix == nullptr) {
      std::cerr << "Memory allocation failed" << std::endl;
      return nullptr;
    }

    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < cols; j++) {
        if (!(file >> matrix->data[i][j])) {
          std::cerr << "Invalid matrix data" << std::endl;
          free_matrix(matrix);
          return nullptr;
        }
      }
    }
    return matrix;
  }

  // task 18
  int process_task_18(const Matrix* input) {
    int count = 0;
    int rows = input->rows;
    int cols = input->cols;

    for (int k = 1 - rows; k < cols; k++) {
      bool has_zero = false;
      bool diagonal_exists = false;

      for (int i = 0; i < rows; i++) {
        int j = i + k;
        if (j >= 0 && j < cols) {
          diagonal_exists = true;
          if (input->data[i][j] == 0) {
            has_zero = true;
            break;
          }
        }
      }

      if (diagonal_exists && !has_zero) {
        count++;
      }
    }
    return count;
  }

  // task 8
  int process_task_8(const Matrix* input) {
    int total = input->rows * input->cols;
    int* elements = static_cast<int*>(std::malloc(total * sizeof(int)));

    if (elements == nullptr) {
      return -1;
    }

    int index = 0;
    for (int i = 0; i < input->rows; i++) {
      for (int j = 0; j < input->cols; j++) {
        elements[index++] = input->data[i][j];
      }
    }

    for (int i = 0; i < total - 1; i++) {
      for (int j = 0; j < total - i - 1; j++) {
        if (elements[j] > elements[j + 1]) {
          int temp = elements[j];
          elements[j] = elements[j + 1];
          elements[j + 1] = temp;
        }
      }
    }

    int sum = 0;
    for (int i = 0; i < total; i++) {
      sum += elements[i];
    }

    std::free(elements);
    return sum;
  }

  // both results to file
  void write_results_to_file(const char* filename, int result18, int result8) {
    std::ofstream file(filename);
    if (file.is_open()) {
      file << result18 << " " << result8;
    }
  }

  // error handling
  bool validate_arguments(int argc, char* argv[]) {
    if (argc != 4) {
      std::cerr << (argc < 4 ? "Not enough arguments" : "Too many arguments") << std::endl;
      return false;
    }

    char* endptr;
    long num = std::strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || endptr == argv[1]) {
      std::cerr << "First parameter is not a number" << std::endl;
      return false;
    }

    if (num != 1 && num != 2) {
      std::cerr << "First parameter is out of range" << std::endl;
      return false;
    }

    return true;
  }

}

int main(int argc, char* argv[]) {
  using namespace pozdnyakov;

  if (!validate_arguments(argc, argv)) {
    return 1;  // code 1 for argument errors
  }

  int task_num = std::atoi(argv[1]);
  const char* input_file = argv[2];
  const char* output_file = argv[3];

  Matrix* input_matrix = read_matrix_from_file(input_file, task_num == 1);
  if (input_matrix == nullptr) {
    return 2;  // code 2 for file/matrix errors
  }

  int result18 = process_task_18(input_matrix);
  int result8 = process_task_8(input_matrix);

  write_results_to_file(output_file, result18, result8);

  // output results
  std::cout << result18 << " " << result8 << std::endl;

  // cleanup
  free_matrix(input_matrix);

  return 0;
}

