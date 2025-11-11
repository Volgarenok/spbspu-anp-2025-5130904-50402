#include <iostream>
#include <fstream>
namespace matveev
{
  void rMatrix(int** matrix, int& rows, int& cols, const char* filename);
  void wMatrix(int** matrix, int rows, int cols, const char* filename);
  int** allocMatrix(int rows, int cols);
  void rm(int** matrix, int rows);
  void spiral(int** matrix, int rows, int cols);
}

int main()
{
  int rows = 0, cols = 0;
  int** matrix = new int*[100];
  matveev::rMatrix(matrix, rows, cols, "input.txt");
  matveev::spiral(matrix, rows, cols);
  matveev::wMatrix(matrix, rows, cols, "output.txt");
  matveev::rm(matrix, rows);
return 0;
}

namespace matveev
{
  void rMatrix(int** matrix, int& rows, int& cols, const char* filename)
  {
  std::ifstream file(filename);
  file >> rows >> cols;
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new int[cols];
    for (int j = 0; j < cols; ++j) {
      file >> matrix[i][j];
    }
  }
  file.close();
  }
  void wMatrix(int** matrix, int rows, int cols, const char* filename)
  {
  std::ofstream file(filename);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (j > 0) {
        file << " ";
      }
      file << matrix[i][j];
    }
    file << "\n";
  }
  file.close();
  }
  int** allocMatrix(int rows, int cols)
  {
  int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i) {
      matrix[i] = new int[cols];
      for (int j = 0; j < cols; ++j) {
        std::cin >> matrix[i][j];
      }
    }
    return matrix;
  }
  void rm(int** matrix, int rows)
  {
    for (int i = 0; i < rows; ++i) {
      delete[] matrix[i];
    }
    delete[] matrix;
  }
void spiral(int** matrix, int rows, int cols)
{
  int up_r = 0, left_c = 0;
  int down_r = rows - 1, right_c = cols - 1;
  int counter = 1;
  while (up_r <= down_r && left_c <= right_c) {
    for (int i = left_c; i <= right_c; ++i) {
      matrix[down_r][i] += counter++;
    }
    down_r--;
    if (left_c <= right_c) {
      for (int i = down_r; i >= up_r; --i) {
        matrix[i][right_c] += counter++;
      }
      right_c--;
    }
    if (up_r <= down_r) {
      for (int i = right_c; i >= left_c; --i) {
        matrix[up_r][i] += counter++;
      }
      up_r++;
    }
    if (left_c <= right_c) {
      for (int i = up_r; i <= down_r; ++i) {
        matrix[i][left_c] += counter++;
      }
      left_c++;
    }
  }
}
}
