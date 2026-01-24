#include <iostream>
#include <fstream>
#include <cstdlib>
namespace sogdanov
{
  std::istream & readMatrix(std::ifstream & input, int * mtx, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows * cols ; i++) {
      if (!(input >> mtx[i])) {
        return input;
      }
    }
    return input;
  }
  int cntSdlPnt(const int * mtx, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    int * rowMin = reinterpret_cast< int * >(malloc(rows * sizeof(int)));
    if (rowMin == nullptr) {
      return -1;
    }
    for (size_t i = 0; i < rows; ++i) {
      int minValue = mtx[i * cols];
      for (size_t j = 1; j < cols; ++j) {
        if (mtx[i * cols + j] < minValue) {
          minValue = mtx[i * cols +j];
        }
      }
      rowMin[i] = minValue;
    }
    size_t count = 0;
    for (size_t i = 0; i < cols; ++i) {
      int maxVal = mtx[i];
      for (size_t j = 1; j < rows; ++j) {
        if (mtx[j * cols + i] > maxVal) {
          maxVal = mtx[j * cols + i];
        }
      }
      for (size_t j = 0; j < rows; ++j) {
        if (mtx[j * cols + i] == maxVal && mtx[j * cols + i] == rowMin[j]) {
          count++;
        }
      }
    }
    free(rowMin);
    return count;
  }
  int maxSumSdg(const int * mtx, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    size_t n = rows < cols ? rows : cols;
    long long maxSum = 0;
    for (size_t i = 1; i < n; ++i) {
      long long sum = 0;
      for (size_t j = 0; j < n - i; ++j) {
        sum += mtx[j * cols + j + i];
      }
      if (sum > maxSum) {
        maxSum = sum;
      }
    }
    for (size_t i = 1; i < n; i++) {
      long long sum = 0;
      for (size_t j = 0; j < n - i; j++) {
        sum += mtx[cols * (j + i) + j];
      }
      if (sum > maxSum) {
        maxSum = sum;
      }
    }
    return maxSum;
  }
  int * createMatrix(size_t rows, size_t cols)
  {
    return reinterpret_cast< int * >(malloc(rows * cols * sizeof(int)));
  }
  void rm(long num, int * mtx)
  {
    if (num == 2 && mtx != nullptr) {
      free(mtx);
    }
  }
}

int main(int argc, char ** argv)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  char * end = nullptr;
  long num = std::strtol(argv[1], std::addressof(end), 10);
  if (num != 1 && num != 2) {
    std::cerr << "First argument is out of range\n";
    return 1;
  }
  if (*end != '\0' || end == argv[1]) {
    std::cerr << "First argument is not a number\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }
  size_t rows = 0;
  size_t cols = 0;
  input >> rows >> cols;
  if (rows > 10000 || cols > 10000 || rows * cols > 10000) {
    std::cerr << "Matrix sizes are too large\n";
    return 2;
  }
  if (input.fail()) {
    std::cerr << "Incorrect Matrix Sizes\n";
    return 2;
  }
  const int SIZE = 10000;
  int mtx_on_stack[SIZE] = {};
  int * mtx = nullptr;
  if (num == 1) {
    mtx = mtx_on_stack;
  } else if (num == 2) {
    mtx = sogdanov::createMatrix(rows, cols);
  }
  if (mtx == nullptr) {
    std::cerr << "Memory allocation failed\n";
    return 2;
  }
  sogdanov::readMatrix(input, mtx, rows, cols);
  if (input.fail()) {
    std::cerr << "Input error\n";
    sogdanov::rm(num, mtx);
    return 2;
  }
  int extra;
  if (input >> extra) {
    std::cerr << "Too many elements in input file\n";
    sogdanov::rm(num, mtx);
    return 2;
  }
  int res1 = sogdanov::maxSumSdg(mtx, rows, cols);
  int res2 = sogdanov::cntSdlPnt(mtx, rows, cols);
  if (res2 < 0) {
    std::cerr << "Memory allocation failed\n";
    sogdanov::rm(num,mtx);
    return 2;
  }
  sogdanov::rm(num, mtx);
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file\n";
    return 2;
  }
  output << res1 << ' ' << res2 << '\n';
}
