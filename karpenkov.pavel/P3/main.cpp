#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace karpenkov {
std::ifstream &readToArray(int *array, size_t n, size_t m,std::ifstream &input)
  {
  for (size_t i = 0; i < n * m; i++) {
    if (!(input >> array[i])) {
      break;
    }
  }
  input.clear();
  input.seekg(0, std::ios::beg);
  return input;
}

size_t printCntColNsm(const int *array, size_t m, size_t n)
  {
  bool flag = false;
  size_t num = 0, countColumn = n;
  if (m*n == 0) {
    return num;
  }
  for (size_t i = 0; i < m * n - 3; ++i) {
    if (num == n) {
      flag = false;
      num = 0;
    }
    if (array[i] == array[i + 3] && flag != true) {
      --countColumn;
      flag = true;
    }
    ++num;
  }
  return num;
}

int printMaxSumSdg(int *array, size_t m, size_t n)
  {
  int max_sum = std::numeric_limits<int>::min();
  if (n*m == 0) {
    return 0;
  }
  for (size_t diag = 1; diag < m; ++diag) {
    int sum = 0;
    size_t count = 0;
    for (size_t i = 0; i < n; ++i) {
      size_t j = i + diag;
      if (j < m) {
        sum += array[i * m + j];
        ++count;
      }
    }
    if (count > 0 && sum > max_sum) {
      max_sum = sum;
    }
  }
  for (size_t diag = 1; diag < n; ++diag) {
    int sum = 0;
    size_t count = 0;
    for (size_t i = 0; i < m; ++i) {
      size_t j = i + diag;
      if (j < n) {
        sum += array[j * m + i];
        ++count;
      }
    }
    if (count > 0 && sum > max_sum) {
      max_sum = sum;
    }
  }
  return max_sum;
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
  char *endptr = nullptr;
  char *parameter = argv[1];

  int num = std::strtol(parameter, std::addressof(endptr), 10);

  if (*endptr != '\0') {
    std::cout << "First parameter contains non-numeric symbol" << '\n';
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

  if (input.peek() == EOF) {
    std::cout << "File is empty" << '\n';
    return 2;
  }

  size_t m, n;
  if (!(input >> m >> n)) {
    std::cout << "Cannot read matrix dimension" << '\n';
    return 2;
  }

  int *arrayPtr = nullptr;
  size_t maxSize = 10000;
  int stackArray[maxSize];

  if (num == 1) {
    arrayPtr = stackArray;
  }
  if (num == 2) {
    arrayPtr = reinterpret_cast<int *>(malloc(n * m * sizeof(int)));
    if (arrayPtr == nullptr) {
      std::cout << "Cannot allocate memory" << '\n';
      return 3;
    }
  }

  size_t indexCount = 0, tmp;
  karpenkov::readToArray(arrayPtr, m, n, input);
  while (input >> tmp) {
    ++indexCount;
  }
  if (n*m != 0 && indexCount - 2 < m * n) {
    std::cout << "Not enough data in file" << '\n';
    if (num == 2) {
      free(arrayPtr);
    }
    return 2;
  }
  if (n*m != 0 && indexCount - 2 > m * n) {
    std::cout << "Too much data in file" << '\n';
    if (num == 2) {
      free(arrayPtr);
    }
    return 2;
  }

  std::ofstream output(argv[3]);
  output << n * m;
  for (size_t i = 0; i < n * m; ++i) {
    output << " " << arrayPtr[i];
  }

  std::cout << "CNT_COL_NSM - " << karpenkov::printCntColNsm(arrayPtr, m, n) << '\n';
  std::cout << "MAX_SUM_SDG - " << karpenkov::printMaxSumSdg(arrayPtr, m, n) << '\n';

  if (num == 2) {
    free(arrayPtr);
  }
}
