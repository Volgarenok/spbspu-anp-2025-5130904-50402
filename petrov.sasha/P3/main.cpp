#include <iostream>
#include <fstream>

namespace petrov
{
  void LFT_BOT_CNT(int** a, size_t r, size_t c);
  void FLL_INC_WAV(int** a, size_t rows, size_t cols);
  void rm(int** mtx, size_t rows);

  void rm(int** mtx, size_t rows)
  {
    if (!mtx) return;
    for (size_t i = 0; i < rows; i++){
      delete[] mtx[i];
    }
    delete[] mtx;
  }

  bool readMTX(std::ifstream& in, int** a, size_t rows, size_t cols)
  {
    size_t elements = 0;
    for (size_t i = 0; i < rows; ++i)
    {
      for (size_t j = 0; j < cols; ++j)
      {
        if (!(in >> a[i][j]))
        {
          if (elements < rows * cols)
          {
            std::cerr << "Invalid matrix data\n";
          }
          return false;
        }
        elements++;
      }
    }
    if (elements < rows * cols)
    {
      std::cerr << "Invalid matrix data\n";
      return false;
    }
    int temp;
    if (in >> temp)
    {
      std::cerr << "Too many arguments for array\n";
      return false;
    }
    return true;
  }

  bool writeMTX(std::ofstream& out, int** a, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows; i++)
    {
      for (size_t j = 0; j < cols; j++)
      {
        out << " " << a[i][j];
      }
    }
    return true;
  }

  bool stat(std::ifstream& in, std::ofstream& out, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return true;
    }
    int arr[100][100] = {0};
    int* arr_ptr[100];
    for (size_t i = 0; i < rows; ++i)
    {
      arr_ptr[i] = arr[i];
    }
    if (!readMTX(in, arr_ptr, rows, cols)) return false;
    LFT_BOT_CNT(arr_ptr, rows, cols);
    return writeMTX(out, arr_ptr, rows, cols);
  }

  bool dyn(std::ifstream& in, std::ofstream& out, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return true;
    }
    int** a = nullptr;
    try
    {
      a = new int*[rows];
      for (size_t i = 0; i < rows; ++i) {
        a[i] = new int[cols]();
      }
    } catch (...) {
      rm(a, rows);
      std::cerr << "Memory allocation failed\n";
      return false;
    }
    if (!readMTX(in, a, rows, cols)) {
      rm(a, rows);
      return false;
    }
    FLL_INC_WAV(a, rows, cols);
    bool result = writeMTX(out, a, rows, cols);
    rm(a, rows);
    return result;
  }

  void LFT_BOT_CNT(int** a, size_t r, size_t c)
  {
    size_t counter = 1;
    size_t up = 0, down = r - 1, left = 0, right = c - 1;
    while (up <= down && left <= right) {
        for (size_t i = down; i >= up && i < r; --i) {
            a[i][left] += counter++;
            if (i == 0) {
              break;
            }
        }
        left++;
        if (left > right) {
          break;
        }
        for (size_t j = left; j <= right && j < c; ++j) {
            a[up][j] += counter++;
        }
        up++;
        if (up > down) {
          break;
        }
        for (size_t i = up; i <= down && i < r; ++i) {
            a[i][right] += counter++;
        }
        right--;
        if (left > right) {
          break;
        }
        for (size_t j = right; j >= left && j < c; --j) {
            a[down][j] += counter++;
            if (j == 0) {
              break;
            }
        }
        down--;
    }
}
  void FLL_INC_WAV(int** a, size_t rows, size_t cols)
  {
    size_t min_data = (rows < cols) ? rows : cols;
    size_t borders = (min_data + 1) / 2;
    for (size_t border =  0; border < borders; ++border) {
      int counter = border + 1;
      for (size_t j = border; j < cols - border; ++j) {
        a[border][j] += counter;
      }
      for (size_t i = border + 1; i < rows - border; ++i) {
        a[i][cols - 1 - border] += counter;
      }
      if (border < rows - 1 - border) {
        for (size_t j = cols - 2 - border; j >= border; j--) {
          a[rows - 1 - border][j] += counter;
          if (j == 0) {
            break;
          }
        }
      }
      if (border < cols - 1 - border) {
        for (size_t i = rows - 2 - border; i > border; i--) {
          a[i][border] += counter;
          if (i == 0) {
            break;
          }
        }
      }
    }
  }
}

int main(int argc, char const** argv)
{
  if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments\n" : "Too many arguments\n");
    return 1;
  }
  int var = 0;
  try {
    var = std::stoi(argv[1]);
  }
  catch (...) {
    std::cerr << "First parameter is not a number\n";
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
  int rows = 0, cols = 0;
  if (!(input >> rows >> cols)) {
    std::cerr << "Invalid matrix data\n";
    return 2;
  }
  output << rows << " " << cols;
  bool success = true;
  if (var == 1) {
    success = petrov::stat(input, output, rows, cols);
  } else {
    success = petrov::dyn(input, output, rows, cols);
  }
  input.close();
  return success ? 0 : 2;
}
