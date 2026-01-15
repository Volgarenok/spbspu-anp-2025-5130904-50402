#include <cctype>
#include <cstring>
#include <iostream>

namespace dirko
{
  const size_t alphaSize = 26;
  const char *find(const char *start, const char *end, char target)
  {
    for (const char *ch = start; ch < end; ++ch) {
      if (*ch == target) {
        return ch;
      }
    }
    return end;
  }
  char *extendSize(const char *str, size_t size)
  {
    char *newStr = reinterpret_cast< char * >(malloc(size * 2 * sizeof(char)));
    if (newStr == nullptr) {
      return nullptr;
    }
    std::strncpy(newStr, str, size);
    newStr[size] = '\0';
    return newStr;
  }
  char *getLine(std::istream &in, size_t &size)
  {
    size_t copasity = 1;
    bool isSkipWp = in.flags() & std::ios::skipws;
    if (isSkipWp) {
      in >> std::noskipws;
    }
    char *str = reinterpret_cast< char * >(malloc(sizeof(char) * copasity));
    if (str == nullptr) {
      if (isSkipWp) {
        in >> std::skipws;
      }
      return str;
    }
    char ch = 0;
    while (in >> ch && ch != '\n') {
      if (size == copasity) {
        char *tmp = extendSize(str, size);
        free(str);
        if (tmp == nullptr) {
          str = nullptr;
          if (isSkipWp) {
            in >> std::skipws;
          }
          return str;
        }
        str = tmp;
        copasity *= 2;
      }
      str[size++] = ch;
    }
    if (isSkipWp) {
      in >> std::skipws;
    }
    if (size == copasity) {
      char *tmp = extendSize(str, size);
      free(str);
      if (tmp == nullptr) {
        str = nullptr;
        return str;
      }
      str = tmp;
      copasity *= 2;
    }
    str[size] = '\0';
    return str;
  }
  size_t doDifLat(const char *str)
  {
    size_t count = 0;
    char seen[alphaSize] = {};
    for (const char *ch = str; *ch != '\0'; ++ch) {
      if (std::isalpha(*ch)) {
        if (dirko::find(seen, seen + count, std::tolower(*ch)) == seen + count) {
          seen[count++] = std::tolower(*ch);
        }
      }
    }
    return count;
  }
  char *doUppLow(const char *source, char *distention)
  {
    for (size_t i = 0; source[i] != '\0'; ++i) {
      distention[i] = (std::isalpha(source[i])) ? std::tolower(source[i]) : source[i];
    }
    return distention;
  }
}
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
