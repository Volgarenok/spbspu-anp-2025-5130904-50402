#include <iostream>
#include <iomanip>
#include "getline.hpp"
#include "extend.hpp"
#include "strChange.hpp"

int main() {
  namespace karp = karpovich;

  size_t s = 0;
  char* str = karp::getline(std::cin, s);
  if (!str) {
    std::cerr << "Failed to alloc memory" << '\n';
    return 1;
  }
  if (s == 0 && std::cin.eof()) {
    free(str);
    std::cerr << "No input" << '\n';
    return 1;
  }

  char* data = reinterpret_cast< char* >(malloc(s + 1));
  if (!data) {
    free(str);
    std::cerr << "Failed to alloc memory" << '\n';
    return 1;
  }
  data[s] = '\0';
  if (!karp::repsym(str, data)) {
    free(data);
    free(str);
    std::cerr << "repsym failed" << '\n';
    return 1;
  }

  size_t s2 = 4;
  const char* str2 = "def ";

  char* data2 = reinterpret_cast< char* >(malloc(s + s2 + 1));
  if (!data2) {
    std::free(data);
    std::free(str);
    std::cerr << "Failed to alloc memory" << '\n';
    return 1;
  }
  data2[s + s2] = '\0';

  if (!karp::unitwo(str, str2, s, s2, data2)) {
    free(data2);
    free(data);
    free(str);
    std::cerr << "unitwo failed" << '\n';
    return 1;
  }

  std::cout << data << '\n';
  std::cout << data2 << '\n';

  free(data2);
  free(str);
  free(data);
  return 0;
}
