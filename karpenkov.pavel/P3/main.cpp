#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <cstdlib>

namespace karpenkov {
  int readToArray(int *array, size_t n, size_t m, std::ifstream& input, std::ofstream& output)
  {
    size_t index = 0, tmp;
    while (input >> tmp) {
      if (index >= n * m) {
        std::cout << "Too much data in file" << '\n';
        return 2;
      }
      array[index++] = tmp;
    }
    if (index < n * m) {
      std::cout << "Not enough data in file" << '\n';
      return 2;
    }
    output << n*m;
    for (size_t i = 0; i < n * m; ++i) {
      output << " "<< array[i];
    }
    return 0;
  }

  void printCntColNsm(int *array, int m, int n) {
    bool flag = false;
    int num = 0, countCol = n;
    for (int i = 0; i < m * n - 3; ++i) {
      if (num == n) {
        flag = false;
        num = 0;
      }
      if (array[i] == array[i + 3] && flag != true) {
        --countCol;
        flag = true;
      }
      ++num;
    }
    std::cout << "CNT-COL-NSM - " << countCol << '\n';
  }

  void printMaxSumSdg(int *array, int m, int n) {
    int max_sum = std::numeric_limits<int>::min();
    for (int diag = 1; diag < m; ++diag) {
      int sum = 0, count = 0;
      for (int i = 0; i < n; ++i) {
        int j = i + diag;
        if (j < m) {
          sum += array[i * m + j];
          ++count;
        }
      }
      if (count > 0 && sum > max_sum) {
        max_sum = sum;
      }
    }
    for (int diag = 1; diag < n; ++diag) {
      int sum = 0, count = 0;
      for (int i = 0; i < m; ++i) {
        int j = i + diag;
        if (j < n) {
          sum += array[j * m + i];
          ++count;
        }
      }
      if (count > 0 && sum > max_sum) {
        max_sum = sum;
      }
    }
    std::cout << "MAX-SUM-SDG - " << max_sum << '\n';
  }
}

int main(int argc, char **argv)
{
  if (argc < 4) {
    std::cout << "Not enough arguments" << '\n';
    return 1;
  }
  if (argc > 4) {
    std::cout << "Too many arguments" << '\n';
    return 1;
  }
   char* endptr;
   char* param = argv[1];

   int num = std::strtol(param, &endptr, 10);

   if (*endptr != '\0') {
     std::cout << "First parameter contains non-numeric symbol" << '\n';
     return 1;
   }
  if (num > std::numeric_limits<int>::max() || num < std::numeric_limits<int>::min()) {
    std::cout << "First parameter is too large or too small" << '\n';
    return 1;
  }
   if (num < 1 || num > 2) {
     std::cout << "First parametr is out of range" << '\n';
   }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cout << "Cannot open file" << '\n';
    return 2;
  }
  if (input.eof()) {
    std::cout << "File is empty" << '\n';
    return 2;
  }

  size_t m, n;
  if (!(input >> m >> n)) {
    std::cout << "Cannot read matrix dimension" << '\n';
    return 2;
  }

  const size_t fixedSize = m * n;

  std::ofstream output(argv[3]);

  if (num == 1) {
    int array[fixedSize];
    if (karpenkov::readToArray(array, n, m, input, output) > 0){
      return 2;
    }
    karpenkov::printCntColNsm(array, m, n);
    karpenkov::printMaxSumSdg(array, m, n);
  } else {
    int *array = reinterpret_cast <int *> (malloc(n * m * sizeof(int)));
    if (array == nullptr) {
      std::cout << "Cannot allocate memory" << '\n';
      return 3;
    }
    if (karpenkov::readToArray(array, n, m, input, output) > 0){
      free(array);
      return 2;
    }
    karpenkov::printCntColNsm(array, m, n);
    karpenkov::printMaxSumSdg(array, m, n);
    free(array);
  }
}
