#include <iostream>
namespace matveev
{
  void rMatrix(int** matrix, int& rows, int& cols);
  void wMatrix(int** matrix, int rows, int cols);
  int** allocMatrix(int rows, int cols);
  void rm(int** matrix, int rows);

}

int main()
{

return 0;
}

namespace matveev
{
  void rMatrix(int** matrix, int& rows, int& cols)
  {
    std::cin >> rows >> cols;
  }
  void wMatrix(int** matrix, int rows, int cols)
  {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < cols; ++j) {
        if (j > 0){
           std::cout << " ";
        }
        std::cout << matrix[i][j];
      }
    }
    std::cout << "\n";
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

}
