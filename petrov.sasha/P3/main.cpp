#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>

namespace petrov {
  void copyMTX(const int* a, int* b, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        b[i * cols + j] = a[i * cols + j];
      }
    }
  }

  std::ifstream& readMTX(std::ifstream& in, int* a, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        in >> a[i * cols + j];
        if (in.fail()) {
          return in;
        }
      }
    }
    return in;
  }

  std::ofstream& writeMTX(std::ofstream& out, const int* a, size_t rows, size_t cols)
  {
    out << rows << ' ' << cols;
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        out << ' ' << a[i * cols + j];
      }
    }
    return out;
  }

  void lftBotCnt(int* a, size_t r, size_t c)
  {
    if (r == 0 || c == 0 || !a) {
      return;
    }
    size_t up = 0, down = r - 1, left = 0, right = c - 1;
    int counter = 1;
    while (up <= down && left <= right) {
      for (size_t j = left; j <= right; ++j) {
        a[down * c + j] += counter++;
      }
      for (size_t i = down - 1; i >= up && i < r; --i) {
        a[i * c + right] += counter++;
      }
      if (up < down) {
        for (size_t j = right - 1; j >= left && j < c; --j) {
          a[up * c + j] += counter++;
        }
      }
      if (left < right) {
        for (size_t i = up + 1; i < down; ++i) {
          a[i * c + left] += counter++;
        }
      }
      up++;
      down--;
      left++;
      right--;
    }
  }

  void fllIncWav(int* a, size_t rows, size_t cols)
  {
    size_t min_data = (rows < cols) ? rows : cols;
    size_t borders = (min_data + 1) / 2;
    for (size_t border = 0; border < borders; ++border) {
      int counter = border + 1;
      for (size_t j = border; j < cols - border; ++j) {
        a[border * cols + j] += counter;
      }
      for (size_t i = border + 1; i < rows - border; ++i) {
        a[i * cols + (cols - 1 - border)] += counter;
      }
      if (border < rows - 1 - border) {
        for (size_t j = cols - 1 - border; j-- > border; ) {
          a[(rows - 1 - border) * cols + j] += counter;
        }
      }
      if (border < cols - 1 - border) {
        for (size_t i = rows - 1 - border; i-- > border + 1; ) {
          a[i * cols + border] += counter;
        }
      }
    }
  }
}

int main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments\n" : "Too many arguments\n");
    return 1;
  }
  char* endptr = nullptr;
  long var = std::strtol(argv[1], std::addressof(endptr), 10);
  if (argv[1] == endptr) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  if (*endptr != '\0') {
    std::cerr << "First parameter contains invalid symbols\n";
    return 1;
  }
  if (var != 1 && var != 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  std::ofstream output(argv[3]);
  if (!input.is_open() || !output.is_open()) {
    std::cerr << "Cannot open file\n";
    return 2;
  }
  size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols)) {
    std::cerr << "Invalid matrix data\n";
    return 2;
  }
  if (rows * cols > 10000) {
    std::cerr << "Matrix too big for matrix\n";
    return 2;
  }
  if (rows == 0 || cols == 0) {
    return 0;
  }
  const size_t max_lenght = 10000;
  int mtx_var1[max_lenght] = {0};
  int* mtx1 = nullptr;
  int* mtx2 = nullptr;
  try {
    mtx2 = new int[rows * cols]();
  } catch (const std::bad_alloc& e) {
    std::cerr << "Memory add failed\n";
    return 2;
  }
  if (var == 1) {
    mtx1 = mtx_var1;
  } else {
    try {
      mtx1 = new int[rows * cols]();
    } catch (const std::bad_alloc& e) {
      std::cerr << "Memory add failed\n";
      delete[] mtx2;
      return 2;
    }
  }
  if (petrov::readMTX(input, mtx1, rows, cols).fail()) {
    std::cerr << "Cant read matrix" << '\n';
    delete[] mtx2;
    if (var == 2) {
      delete[] mtx1;
    }
    return 2;
  }
  petrov::copyMTX(mtx1, mtx2, rows, cols);
  petrov::lftBotCnt(mtx1, rows, cols);
  petrov::fllIncWav(mtx2, rows, cols);
  output << "Var-1 ";
  petrov::writeMTX(output, mtx1, rows, cols) << '\n';
  output << "Var-2 ";
  petrov::writeMTX(output, mtx2, rows, cols) << '\n';
  delete[] mtx2;
  if (var == 2) {
    delete[] mtx1;
  }
  return 0;
}
