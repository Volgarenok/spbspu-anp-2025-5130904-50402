#include <iostream>
namespace matveev
{
  void rm(int** matrix, int rows);
  void rMatrix(int** matrix, int& rows, int& cols, const char* filename);
  void wMatrix(int** matrix, int rows, int cols, const char* filename);
  int** allocMatrix(int rows, int cols);
  void rMatrixDin(int matrix[][100], int& rows, int& cols, const char* filename);
  void wMatrixDin(int matrix[][100], int rows, int cols, const char* filename);


}
int main(int argc, char* argv[]){
  int rows = 0, cols = 0;
  int num = argv[1][0] - '0';
  const char* input = argv[2];
  const char* output = argv[3];
  int res = 0;
  if (num == 1) {
    int matrix[100][100] = {0};
  matveev::rMatrixDin(matrix, rows, cols, input);
  } else if (num == 2) {
  int** matrix = new int*[100];
  matveev::rMatrix(matrix, rows, cols, input);
  matveev::rm(matrix, rows);
  }
  std::cin << res << "\n";
return 0;
}

namespace matveev
{
  void rm(int** matrix, int rows)
  {
    for (int i = 0; i < rows; ++i) {
      delete[] matrix[i];
    }
    delete[] matrix;
  }
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
  void rMatrixDin(int matrix[][100], int& rows, int& cols, const char* filename)
  {
  std::ifstream file(filename);
  file >> rows >> cols;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      file >> matrix[i][j];
    }
  }
  file.close();
  }
  void wMatrixDin(int matrix[][100], int rows, int cols, const char* filename)
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

}
