#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

bool anum(char * a);

int main(int argc, char ** argv)
{
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
  std::ifstream input(argv[2]);
  std::ofstream output(argv[3]);

  int n = 0, m = 0;
  if (!input || !(input >> n >> m) || n < 0 || m < 0 || n == 1 || m == 1 || (n*m)>10000) {
    std::cerr << "Array cannot exist\n";
    return 2;
  }
  if (n == 0 && m == 0) {
    output << "0\n";
    return 0;
  }
  if (n == 2 || m == 2) {
    output << "0\n";
    return 0;
  }
  if (std::stoi(argv[1]) == 1) {
    int a[10000] = {};
    size_t count = 0;
    for (size_t i = 0; i < n*m; ++i){
      input >> a[i];
      count = count + 1;
      if (!input) {
        std::cerr << "Failed to read array element\n";
        return 2;
      }
    }
    if (count != n * m) {
      std::cerr << " Invalid number of array elements\n";
      return 2;
    }
    
    size_t ix = m + 1;
    size_t kolmin = 0;
    size_t kolmax = 0;
    int min = a[m + 1];
    int max = a[m + 1];
    
    while (ix < n * m - 1 - m) {
      if (ix % m == m - 1) {
        ix = ix + 2;
      }
      if (ix % m == 0) {
        ix = ix + 1;
      }
      else {
        if (a[ix] < a[ix - m - 1] && a[ix] < a[ix - m] && a[ix] < a[ix - m + 1] && a[ix] < a[ix - 1] && a[ix] < a[ix + 1] && a[ix] < a[ix + m - 1] && a[ix] < a[ix + m] && a[ix] < a[ix + m + 1]) {
          min = a[ix];
        }
        if (a[ix] > a[ix - m - 1] && a[ix] > a[ix - m] && a[ix] > a[ix - m + 1] && a[ix] > a[ix - 1] && a[ix] > a[ix + 1] && a[ix] > a[ix + m - 1] && a[ix] > a[ix + m] && a[ix] > a[ix + m + 1]) {
          max = a[ix];
        }
        ix = ix + 1;
      }
    }
    for (size_t p = m; p < n * m - m; ++p) {
      if (a[p] == min && p % m != 0 && p % m != m - 1) {
        kolmin = kolmin + 1;
      }
      if (a[p] == max && p % m != 0 && p % m != m - 1) {
        kolmax = kolmax + 1;
      }
    }
    output << kolmin << "\n" << kolmax << "\n";
  }

  else {
    int* a = (int*)malloc(n * m * sizeof(int));
    if (!a) {
      std::cerr << "bad_alloc\n";
      return 3;
    }
    size_t count = 0;
    for (size_t i = 0; i < n*m; ++i) {
      input >> a[i];
      count = count + 1;
      if (!input) {
        std::cerr << "Failed to read array element\n";
        free(a);
        return 2;
      }
    }
    if (count != n * m) {
      std::cerr << " Invalid number of array elements\n";
      return 2;
    }

    size_t ix = m + 1;
    int min = a[m + 1];
    int max = a[m + 1];
    size_t kolmin = 0;
    size_t kolmax = 0;

    while (ix < n * m - 1 - m) {
      if (ix % m == 0) {
        ix = ix + 1;
      }
      if (ix % m == m - 1) {
        ix = ix + 2;
      }
      else {
        if (a[ix] < a[ix - m - 1] && a[ix] < a[ix - m] && a[ix] < a[ix - m + 1] && a[ix] < a[ix - 1] && a[ix] < a[ix + 1] && a[ix] < a[ix + m - 1] && a[ix] < a[ix + m] && a[ix] < a[ix + m + 1]) {
          min = a[ix];
        }
        if (a[ix] > a[ix - m - 1] && a[ix] > a[ix - m] && a[ix] > a[ix - m + 1] && a[ix] > a[ix - 1] && a[ix] > a[ix + 1] && a[ix] > a[ix + m - 1] && a[ix] > a[ix + m] && a[ix] > a[ix + m + 1]) {
          max = a[ix];
        }
        ix = ix + 1;
      }
    }
    for (size_t p = m; p < n * m - m; ++p) {
      if (a[p] == min && p % m != 0 && p % m != m - 1) {
        kolmin = kolmin + 1;
      }
      if (a[p] == max && p % m != 0 && p % m != m - 1) {
        kolmax = kolmax + 1;
      }
    }
    output << kolmin << "\n" << kolmax << "\n";
    free(a);
  }
}

bool anum(char * a) {
  for (size_t i = 0; i < std::strlen(a); ++i) {
    if (!std::isdigit(static_cast<unsigned char>(a[i]))) {
      return 0;
    }
  }
  return 1;
}
