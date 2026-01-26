#include <iostream>
#include <fstream>
#include "mtxChange.hpp"
#include "mtxAction.hpp"
#include <cstdlib>

int main(int argc, char** argv)
{
  namespace karp = karpovich;
  if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments\n" : "Too many arguments\n");
    return 1;
  }

  char* endptr = nullptr;
  size_t num = std::strtol(argv[1], std::addressof(endptr), 10);
  if (endptr == argv[1] || *endptr != '\0') {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  if (num != 1 && num != 2) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Failed to open input file\n";
    return 2;
  }
  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "Failed to open output file\n";
    return 2;
  }

  size_t rows = 0, cols = 0;
  if (!(input >> rows >> cols)) {
    std::cerr << "Failed to read rows and cols\n";
    return 2;
  }

  size_t size = rows * cols;
  int* active_arr = nullptr;
  int fixed_length_array[karp::MAX];
  bool is_dynamic = false;

  if (num == 1) {
    active_arr = fixed_length_array;
  } else {
    try {
      active_arr = new int[size];
      is_dynamic = true;
    } catch (const std::bad_alloc&) {
      std::cerr << "Memory allocation failed\n";
      return 2;
    }
  }

  karp::inputFunc(input, active_arr, size);
  if (!input) {
    std::cerr << "Failed to read matrix data\n";
    if (is_dynamic) {
      delete[] active_arr;
    }
    return 2;
  }

  size_t res1 = karp::locMin(active_arr, rows, cols);
  output << res1 << '\n';
  karp::lftTopClk(active_arr, rows, cols);
  karp::printArray(output, active_arr, rows, cols);

  if (is_dynamic) {
    delete[] active_arr;
  }
  return 0;
}
