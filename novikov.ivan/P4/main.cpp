#include <cstddef>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <istream>

namespace novikov {
  const char literal[] = "abs\0";
  const size_t literal_size = 4;
  const size_t alphabet_size = 26;
  const size_t ascii_size = 256;

  size_t countLatin(const char * str)
  {
    size_t count = 0;
    char unique[alphabet_size] = {0};

    for (size_t i = 0; str[i] != '\0'; ++i) {
      if (std::isalpha(str[i])) {
        size_t j = 0;
        while (j < count && unique[j] != std::tolower(str[i])) {
          ++j;
        }
        if (unique[j] == 0) {
          unique[j] = std::tolower(str[i]);
          ++count;
        }
      }
    }

    return count;
  }

  size_t countSame(const char * str, const char * literal)
  {
    size_t count1[ascii_size] = {0};
    size_t count2[ascii_size] = {0};
    size_t sameCount = 0;

    for (size_t i = 0; str[i] != '\0'; ++i) {
      ++count1[static_cast< unsigned char >(str[i])];
    }

    for (size_t i = 0; literal[i] != '\0'; ++i) {
      ++count2[static_cast< unsigned char >(literal[i])];
    }

    for (size_t i = 0; i < ascii_size; ++i) {
      if (count1[i] > 0 && count2[i] > 0) {
        sameCount += count1[i] < count2[i] ? count1[i] : count2[i];
      }
    }

    return sameCount;
  }

  void extend(char ** str, size_t size, size_t & capacity)
  {
    char * temp = reinterpret_cast< char * >(malloc(capacity * 2));
    if (temp == nullptr) {
      return;
    }
    for (size_t i = 0; i < size; ++i) {
      temp[i] = (*str)[i];
    }
    *str = temp;
    capacity *= 2;
  }

  char * getline(std::istream & in, size_t & size)
  {
    bool is_skipws = in.flags() & std::ios_base::skipws;
    if (is_skipws) {
      in >> std::noskipws;
    }

    size_t capacity = 2;
    char * str = reinterpret_cast< char * >(malloc(capacity * sizeof(char)));

    if (str == nullptr) {
      if (is_skipws) {
        in >> std::noskipws;
      }
      return nullptr;
    }
    while (in) {
      if (size + 1 == capacity) {
        char * old_str = str;
        extend(&str, size, capacity);
        if (size + 1 == capacity) {
          free(str);
          str = nullptr;
          break;
        }
        free(old_str);
      }
      if (str == nullptr) {
        if (is_skipws) {
          in >> std::noskipws;
        }
        return nullptr;
      }
      in >> str[size];
      if (!in || str[size] == '\n') {
        break;
      }
      ++size;
    }

    if (str) {
      str[size] = '\0';
    }

    if (is_skipws) {
      in >> std::skipws;
    }
    return str;
  }
}

int main()
{
  size_t size = 0;
  char * str = novikov::getline(std::cin, size);

  if (str == nullptr) {
    std::cerr << "Reading failed\n";
    return 1;
  } else if (size == 0) {
    free(str);
    std::cerr << "String is empty\n";
    return 1;
  }

  size_t result1 = novikov::countLatin(str);
  size_t result2 = novikov::countSame(str, novikov::literal);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
  free(str);
}
