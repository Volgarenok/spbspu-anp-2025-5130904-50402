#include <iostream>
#include <fstream>
#include <string>

bool anum(char * a);

int main(int argc, char ** argv)
{
  std::ifstream input(argv[2]);
  std::ofstream output(argv[3]);
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  if (!anum (argv[1])) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  if (std::stoi(argv[1])>2 || std::stoi(argv[1]) < 1) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  int n = 0, m = 0;
  input >> n >> m;
  if (!input || !(input >> n >> m) || n < 0 || m < 0 || (n*m)>10000) {
    std::cerr << "Array cannot exist\n";
    return 2;
  }
  if (std::stoi(argv[1]) == 1) {
    int a[10000] = {};
    for (size_t i = 0; i < n; ++i){
      input >> a[i];
      if (!input) {
        std::cerr << "failed to read array element\n";
        free(a);
        return 2;
      }
    }
  }
  else {
    int** a = (int**)malloc(n * sizeof(int*));
    if (!a) {
      std::cerr << "bad_alloc\n";
      return 3;
    }

  }

  std::cout << "\n";
}

bool anum(char * a) {
  for (size_t i = 0; i < std::strlen(a); ++i) {
    if (!std::isdigit(static_cast<unsigned char>(a[i]))) {
      return 0;
    }
  return 1;
}
