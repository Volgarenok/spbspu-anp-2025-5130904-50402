#include <iostream>
#include <fstream>
#include <cstdlib>
#include "matrixOps.hpp"

int main(int argc, char *argv[])
{
  using namespace pozdnyakov;

  if (argc != 4) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }

  if (!validateArgs(argv[1])) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  long mode = std::strtol(argv[1], nullptr, 10);
  const char *inputFile = argv[2];
  const char *outputFile = argv[3];

  std::ifstream in(inputFile);
  if (!in.is_open()) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;

  if (!readDimensions(in, rows, cols)) {
    std::cerr << "Invalid matrix dimensions\n";
    return 2;
  }

  if (rows == 0 && cols == 0) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
      std::cerr << "Cannot open output file\n";
      return 2;
    }
    out << 0 << '\n' << 0 << ' ' << 0 << '\n';
    return 0;
  }

  if (mode == 1) {
    if (rows * cols > MAX_ELEMENTS || rows > MAX_ROWS || cols > MAX_COLS) {
      std::cerr << "Matrix size exceeds limits\n";
      return 2;
    }
  }

  int fixedData[MAX_ROWS * MAX_COLS] = {0};
  int *dataPtr = nullptr;

  if (mode == 1) {
    dataPtr = fixedData;
  } else {
    dataPtr = reinterpret_cast< int * >(std::malloc(rows * cols * sizeof(int)));
    if (dataPtr == nullptr) {
      std::cerr << "Memory allocation failed\n";
      return 2;
    }
  }

  if (!readMatrix(in, dataPtr, rows, cols)) {
    std::cerr << "Invalid matrix data\n";
    if (mode == 2) {
      std::free(dataPtr);
    }
    return 2;
  }

  size_t diagCount = countDiagonalsWithoutZero(dataPtr, rows, cols);
  transformMatrixLayers(dataPtr, rows, cols);

  std::ofstream out(outputFile);
  if (!out.is_open()) {
    std::cerr << "Cannot open output file\n";
    if (mode == 2) {
      std::free(dataPtr);
    }
    return 2;
  }

  out << diagCount << '\n';
  writeMatrix(out, dataPtr, rows, cols);

  if (mode == 2) {
    std::free(dataPtr);
  }

  return 0;
}
