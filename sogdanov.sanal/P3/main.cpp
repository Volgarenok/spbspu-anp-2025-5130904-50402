#include <iostream>
#include <fstream>
namespace sogdanov
{
  int readMatrix(std::ifstream & input, int * mtx, int rows, int cols)
  {
    for (int i = 0; i < rows * cols ; i++) {
      input >> mtx[i];
      if (input.fail()) {
        std::cerr << "Input error\n";
        return 2;
      }
    }
    int extra;
    if (input >> extra) {
      std::cerr << "Too many elements in input file\n";
      return 2;
    }
    return 0;
  }
  int cnt_sdl_pnt(const int * mtx, int rows, int cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    int* rowMin = static_cast<int*>(malloc(rows * sizeof(int)));
    if (rowMin == nullptr) {
      std::cerr << "Memory allocation failed\n";
      return 2;
    }
    for (int i = 0; i < rows; ++i) {
      int minValue = mtx[i * cols];
      for (int j = 1; j < cols; ++j) {
        if (mtx[i * cols + j] < minValue) {
          minValue = mtx[i * cols +j];
        }
      }
      rowMin[i] = minValue;
    }
    int* colMax = static_cast<int*>(malloc(cols * sizeof(int)));
    if (rowMin == nullptr) {
      free(rowMin);
      std::cerr << "Memory allocation failed\n";
      return 2;
    }

    for (int i = 0; i < cols; ++i) {
      int maxVal = mtx[i];
      for (int j = 1; j < rows; ++j) {
        if (mtx[j * cols + i] > maxVal) {
          maxVal = mtx[j * cols + i];
        }
      }
      colMax[i] = maxVal;
    }
    int count = 0;
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        int value = mtx[i * cols + j];
        if ( value == rowMin[i] && value == colMax[j]) {
          count++;
        }
      }
    }
    free(rowMin);
    free(colMax);
    return count;
  }
  int max_sum_sdg(int * a, int rows, int cols)
  {
    if (rows == 0 || cols == 0) {
      return 0;
    }
    int n = rows < cols ? rows : cols;
    long long maxSum = 0;
    for (int i = 1; i < n; ++i) {
      long long sum = 0;
      for (int j = 0; j < n - i; ++j) {
        sum += a[j * cols + j + i];
      }
      if (sum > maxSum) {
        maxSum = sum;
      }
    }
    for (int i = 1; i < n; i++) {
      long long sum = 0;
      for (int j = 0; j < n - i; j++) {
        sum += a[cols * (j + i) + j];
      }
      if (sum > maxSum) {
        maxSum = sum;
      }
    }
    return maxSum;
  }
}
int main(int argc, char ** argv)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  } else if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  char num = *argv[1];
  if (num != '1' && num != '2') {
    if (num <= '9' && num >= '0') {
      std::cerr << "First argument is out of range\n";
    } else {
      std::cerr << "First argument is not a number\n";
    }
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input) {
    std::cerr << "Cannot open input file\n";
    input.close();
    return 2;
  }
  int rows = 0;
  int cols = 0;
  input >> rows >> cols;
  if ( input.fail() || rows < 0 || cols < 0) {
    std::cerr << "Incorrect Matrix Sizes\n";
    input.close();
    return 2;
  }
  int * mtx = nullptr;
  int res1 = 0;
  int res2 = 0;
  if (num  == '1') {
    if (rows > 10000 || cols > 10000 || rows * cols > 10000) {
      std::cerr << "Matrix sizes are too large\n";
      input.close();
      return 2;
    }
    int mtx[10000] = {};
    int r = sogdanov::readMatrix(input, mtx, rows, cols);
    if (r != 0) {
      input.close();
      return r;
    }
    res1 = sogdanov::max_sum_sdg(mtx, rows, cols);
    res2 = sogdanov::cnt_sdl_pnt(mtx, rows, cols);
  }
  if (num == '2') {
    mtx = static_cast<int*>(malloc(rows * cols * sizeof(int)));
    if (mtx == nullptr) {
      std::cerr << "Memory allocation failed\n";
      input.close();
      return 2;
    }
    int r = sogdanov::readMatrix(input, mtx, rows, cols);
    if (r != 0) {
      free(mtx);
      input.close();
      return r;
    }
    res1 = sogdanov::max_sum_sdg(mtx, rows, cols);
    res2 = sogdanov::cnt_sdl_pnt(mtx, rows, cols);
    free(mtx);
  }
  input.close();
  std::ofstream output(argv[3]);
  if (!output) {
    std::cerr << "Cannot open output file\n";
  }
  output << res1 << " " << res2;
}
