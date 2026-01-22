#include <cstring>
#include <iostream>
#include "getLine.hpp"
#include "strActions.hpp"

int main()
{
  size_t size = 0;
  char *str = dirko::getLine(std::cin, size);
  if (str == nullptr) {
    std::cerr << "Cant alloc\n";
    return 1;
  }
  if (std::cin.fail()) {
    free(str);
    std::cerr << "Cant read\n";
    return 1;
  }
  if (size == 0) {
    free(str);
    std::cerr << "Empty string\n";
    return 1;
  }
  size_t result1 = dirko::doDifLat(str);
  char *result2 = reinterpret_cast< char * >(malloc(sizeof(char) * (size + 1)));
  if (result2 == nullptr) {
    free(str);
    std::cerr << "Cant alloc\n";
    return 1;
  }
  result2[size] = '\0';
  dirko::doUppLow(str, result2);
  std::cout << result1 << '\n';
  std::cout << result2 << '\n';
  free(str);
  free(result2);
}
