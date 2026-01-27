#include <fstream>
#include <iostream>
#include <cstdlib>

namespace samarin {
  bool checkMax(const int * a, size_t i, size_t j, size_t n);
  bool isLowerTriangular(const int * a, size_t size, size_t n);
  size_t localMax(const int * a, size_t n, size_t m);
}

int main(int argc, char ** argv)
{
  if (argc != 4) {
    return 1;
  }

  char memory_type = argv[1][0];
  if ((memory_type != '1' && memory_type != '2') || argv[1][1] != '\0') {
    std::cerr << "Unexpected first parameter. \n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file \n";
    return 2;
  }

  size_t m = 0, n = 0;
  input >> m >> n;

  if (input.fail()) {
    std::cerr << "Invalid matrix dimensions \n";
    return 2;
  }

  const size_t ARRAY_SIZE = 10000;
  int * matrix = nullptr;
  int fixed_array[ARRAY_SIZE] = {};

  if (memory_type == '2') {
    matrix = reinterpret_cast< int* >(malloc(sizeof(int) * m * n));
    if (matrix == nullptr) {
      std::cerr << "Memory allocation failed \n";
      return 2;
    }
  } else {
    matrix = fixed_array;
  }

  for (size_t i = 0; i < m * n; ++i) {
    input >> matrix[i];
    if (input.fail()) {
      std::cerr << "Invalid matrix element \n";
      if (memory_type == '2') {
        free(matrix);
      }
      return 2;
    }
  }
  input.close();

  size_t counter = samarin::localMax(matrix, n, m);

  size_t size = std::min(m, n);
  bool is_lower_left = samarin::isLowerTriangular(matrix, size, n);

  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file \n";
    if (memory_type == '2') {
      free(matrix);
    }
    return 2;
  }

  output << "Локальных максимумов: " << counter << "\n";

  if (is_lower_left) {
    output << "Матрица нижняя треугольная\n";
  } else {
    output << "Матрица не нижняя треугольная\n";
  }

  if (memory_type == '2') {

    free(matrix);
  }

  return 0;
}

bool samarin::checkMax(const int * a, size_t i, size_t j, size_t n)
{
  const int * upper_left_section = a + j - 1 + ((i - 1) * n);
  const int * current = a + j + (i * n);
  for (size_t di = 0; di <= 2; ++di) {
    for (size_t dj = 0; dj <= 2; ++dj) {
      if (di == 1 && dj == 1) {
        continue;
      }

      if (upper_left_section[dj + di * n] >= current[0]) {
        return false;
      }
    }
  }
  return true;
}

bool samarin::isLowerTriangular(const int * a, size_t size, size_t n)
{
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = i + 1; j < size; ++j) {
      if (a[j + i * n] != 0) {
        return false;
      }
    }
  }
  return true;
}

size_t samarin::localMax(const int * a, size_t n, size_t m)
{
  size_t counter = 0;
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (i != 0 && j != 0 && i != m - 1 && j != n - 1) {
        if (samarin::checkMax(a, i, j, n)) {
          counter++;
        }
      }
    }
  }
  return counter;
}

