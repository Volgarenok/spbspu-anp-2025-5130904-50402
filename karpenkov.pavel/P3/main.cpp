#include <iostream>
int main(int argc, char **argv) {
  if (argc < 4) {
    std::cout << "Not enough arguments";
    return 1;
  }
  if (argc > 4) {
    std::cout << "Too many arguments";
    return 1;
  }
  try {
    int num = std::stoi(argv[1]);
    if (num < 1 || num > 2) {
      std::cout << "First parametr is out of range";
    }
  } catch (std::invalid_argument) {
    std::cout << "First parametr is not a number";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cout << "Cannot open file";
    return 2;
  }
  if (input.peek() == std::ifstream::traits_type::eof()) {
    std::cout << "File is empty";
    return 2;
  }

  size_t m, n;
  if (!(input >> m >> n)) {
    std::cout << "Cannot read matrix dimension";
    return 2;
  }
}
