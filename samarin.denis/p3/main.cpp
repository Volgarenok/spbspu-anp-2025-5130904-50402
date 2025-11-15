#include <fstream>
#include <iostream>
#include <cstdlib>

namespace samarin {
  bool check_max(int* a, int i, int j, int n, int m);
  bool is_lower_triangular(int* a, int size, int n);
}

int main(int argc, char** argv) {
  // Проверка количества аргументов
  if (argc != 4) {
    return 1;
  }

  if (atoi(argv[1]) != 1 && atoi(argv[1]) != 2) {
    std::cerr << "Unexpected first parameter. \n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file \n";
    return 2;
  }

  int m = 0, n = 0;
  input >> m >> n;
  if (input.fail() || m <= 0 || n <= 0) {
    std::cerr << "Invalid matrix dimensions \n";
    return 2;
  }

  int* a = nullptr;
  if (atoi(argv[1]) == 2) {
    a = static_cast<int*>(malloc(sizeof(int) * m * n));
    if (a == nullptr) {
      std::cerr << "Memory allocation failed \n";
      return 2;
    }
  } else {
    int static_array[10000] = {};
    a = static_array;
  }

  for (int i = 0; i < m * n; ++i) {
    input >> a[i];
    if (input.fail()) {
      std::cerr << "Invalid matrix element \n";
      if (atoi(argv[1]) == 2) {
        free(a);
      }
      return 2;
    }
  }
  input.close();

  size_t counter = 0;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i != 0 && j !=0 && i != m-1 && j != n-1) {
        if (samarin::check_max(a, i, j, n, m)) {
          counter++;
        }
      }
    }
  }

  int size = std::min(m, n);
  bool is_lower_left = samarin::is_lower_triangular(a, size, n);

  // Вывод результатов в файл
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file \n";
    if (atoi(argv[1]) == 2) {
      free(a);
    }
    return 2;
  }

  output << "Локальных максимумов: " << counter << "\n";

  if (is_lower_left) {
    output << "Матрица нижняя треугольная\n";
  } else {
    output << "Матрица не нижняя треугольная\n";
  }

  output.close();

  if (atoi(argv[1]) == 2) {
    free(a);
  }

  return 0;
}

bool samarin::check_max(int* a, int i, int j, int n, int m) {
  int * current = a + j + (i*n);

  for (int di = -1; di <= 1; ++di) {
    for (int dj = -1; dj <= 1; ++dj) {
      if (di == 0 && dj == 0) {
        continue;
      }

      if (current[dj + di*n] >= current[0]) {
        return false;
      }
    }
  }
  return true;
}

bool samarin::is_lower_triangular(int* a, int size, int n) {
  for (int i = 0; i < size; ++i) {
    for (int j = i + 1; j < size; ++j) {
      if (a[j + i * n] != 0) {
        return false;
      }
    }
  }
  return true;
}
