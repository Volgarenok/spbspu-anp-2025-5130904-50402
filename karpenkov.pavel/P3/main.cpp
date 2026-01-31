#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace karpenkov {
std::ifstream &readToArray(std::ifstream &input, int *array, size_t n, size_t m)
  {
  for (size_t i = 0; i < n * m; i++) {
    if (!(input >> array[i])) {
      break;
    }
  }
  return input;
}

size_t CntColNsm(const int *array, size_t m, size_t n)
  {
  bool flag = false;
  size_t num = 0, countColumn = n;
  if (m * n == 0) {
    return num;
  }
  for (size_t i = 0; i + 3 < m * n ; ++i) {
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

int MaxSumSdg(int *array, size_t m, size_t n)
  {
  int max_sum = std::numeric_limits< int >::min();
  if (n * m == 0) {
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
    std::cerr << "Not enough arguments" << '\n';
    return 1;
  }
  if (argc > 4) {
    std::cerr << "Too many arguments" << '\n';
    return 1;
  }
  char *endptr = nullptr;
  char *parameter = argv[1];

  int num = std::strtol(parameter, std::addressof(endptr), 10);

  if (*endptr != '\0') {
    std::cerr << "First parameter contains non-numeric symbol" << '\n';
    return 1;
  }

  if (num < 1 || num > 2) {
    std::cerr << "First parametr is out of range" << '\n';
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Cannot open input file" << '\n';
    return 2;
  }

  size_t m, n;
  if (!(input >> m >> n)) {
    std::cerr << "Cannot read matrix dimension" << '\n';
    return 2;
  }

  int *arrayPtr = nullptr;
  const size_t maxSize = 10000;
  int stackArray[maxSize];

  if (num == 1) {
    arrayPtr = stackArray;
  }
  if (num == 2) {
    arrayPtr = reinterpret_cast<int *>(malloc(n * m * sizeof(int)));
    if (arrayPtr == nullptr) {
      std::cerr << "Cannot allocate memory" << '\n';
      return 3;
    }
  }

  karpenkov::readToArray(input, arrayPtr, n, m);

  if (!input) {
    std::cerr << "Not enough data in file" << '\n';
    if (num == 2) {
      free(arrayPtr);
    }
    return 2;
  }
  int extra;
  if (input >> extra) {
    std::cerr << "Too much data in file" << "\n";
    return 2;
  }

  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "Cannot open output file" << '\n';
    return 2;
  }
  output << karpenkov::CntColNsm(arrayPtr, m, n) << '\n';
  output << karpenkov::MaxSumSdg(arrayPtr, m, n) << '\n';

  if (num == 2) {
    free(arrayPtr);
  }
}
