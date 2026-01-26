#include <cstdlib>
#include <fstream>
#include <iostream>
#include "mtxActions.hpp"
#include "mtxConvertion.hpp"

int main(int argc, char **argv)
{
  if (argc < 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  if (argv[1][1] != '\0' || argv[1][0] > '2' || argv[1][0] < '1') {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  int mode = std::atoi(argv[1]);
  std::ifstream fin(argv[2]);
  if (!fin.is_open()) {
    std::cerr << "Cant open input file\n";
    return 2;
  }
  size_t rows = 0, cols = 0;
  fin >> rows >> cols;
  int staticMatrix[dirko::MAX_SIZE]{};
  int *matrix = nullptr;
  if (mode == 1) {
    matrix = staticMatrix;
  } else {
    try {
      matrix = new int[rows * cols];
    } catch (const std::bad_alloc &e) {
      std::cerr << "Cant alloc\n";
      return 3;
    }
  }
  dirko::inputMtx(fin, matrix, rows, cols);
  if (fin.fail()) {
    if (mode == 2) {
      delete[] matrix;
    }
    std::cerr << "Cant read\n";
    return 2;
  }
  fin.close();
  int *result1 = nullptr;
  try {
    result1 = new int[rows * cols];
  } catch (const std::bad_alloc &e) {
    if (mode == 2) {
      delete[] matrix;
    }
    std::cerr << "Cant alloc\n";
    return 3;
  }
  dirko::copyMtx(matrix, result1, rows, cols);
  dirko::doLftBotClk(result1, rows, cols);
  bool result2 = dirko::doLwrTriMtx(matrix, rows, cols);
  std::ofstream fout(argv[3]);
  if (!fout.is_open()) {
    std::cerr << "Cant open output file\n";
    return 2;
  }
  dirko::output(fout, result1, rows, cols) << '\n';
  fout << std::boolalpha << result2 << '\n';
  delete[] result1;
  if (mode == 2) {
    delete[] matrix;
  }
}
