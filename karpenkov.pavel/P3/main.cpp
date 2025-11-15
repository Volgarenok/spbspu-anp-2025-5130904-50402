#include <climits>
#include <fstream>
#include <iostream>
#include <stdexcept>
int *createDynamicMassive(size_t n, size_t m) {
  return static_cast<int *>(malloc(n * m * sizeof(int)));
}
int CNT_COL_NSM(int *array, int m, int n) {
  bool flag = false;
  int num = 0, cntCol = n;
  for (int i = 0; i < m * n - 3; ++i) {
    if (num == n) {
      flag = false;
      num = 0;
    }
    if (array[i] == array[i + 3] && flag != true) {
      --cntCol;
      flag = true;
    }
    ++num;
  }
  return cntCol;
}
int MAX_SUM_SDG(int *array, int m, int n) {
  int max_sum = INT_MIN;
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
  return max_sum;
}

int main(int argc, char **argv) {
  if (argc < 4) {
    std::cout << "Not enough arguments";
    return 1;
  }
  if (argc > 4) {
    std::cout << "Too many arguments";
    return 1;
  }
  try {
    int num = std::stoi(argv[1]);
    if (num < 1 || num > 2) {
      std::cout << "First parametr is out of range";
    }
  } catch (std::invalid_argument) {
    std::cout << "First parametr is not a number";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cout << "Cannot open file";
    return 2;
  }
  if (input.peek() == std::ifstream::traits_type::eof()) {
    std::cout << "File is empty";
    return 2;
  }

  int m, n;
  if (!(input >> m >> n)) {
    std::cout << "Cannot read matrix dimension";
    return 2;
  }

  int *array = nullptr;

  int variant = std::stoi(argv[1]);
  int index = 0, tmp;
  if (variant == 1) {
    array = static_cast<int *>(alloca(n * m * sizeof(int)));
  } else {
    array = createDynamicMassive(n, m);
    if (array == nullptr) {
      std::cout << "Cannot allocate memory";
      return 3;
    }
  }
  while (input >> tmp) {
    array[index++] = tmp;
  }

  std::ofstream output("output.txt");
  for (int i = 0; i < n * m; ++i) {
    output << array[i] << " ";
  }

  std::cout << "CNT-COL-NSM - " << CNT_COL_NSM(array, m, n) << "\n";
  std::cout << "MAX-SUM-SDG - " << MAX_SUM_SDG(array, m, n) << "\n";

  if (variant == 2) {
    free(array);
  }
}
