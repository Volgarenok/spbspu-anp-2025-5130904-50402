#include <iostream>
#include <iomanip>
namespace karpovich
{
  char* extend(char* old_str, size_t old_size)
  {
    char* new_str = reinterpret_cast< char* >(malloc(old_size + 1));
    if (new_str == nullptr) {
      return nullptr;
    }
    for (size_t i = 0; i < old_size; ++i) {
      new_str[i] = old_str[i];
    }
    return new_str;
  }
  char* getline(std::istream& in, size_t& s)
  {
    bool is_skipws = in.flags() & std::ios_base::skipws;
    if (is_skipws) {
      in >> std::noskipws;
    }
    char* str = nullptr;
    char ch = '\0';
    while (in >> ch && ch != '\n') {
      char* temp = extend(str, s);
      if (temp == nullptr) {
        free(str);
        if (is_skipws) {
          in >> std::skipws;
        }
        return nullptr;
      }
      free(str);
      str = temp;
      str[s] = ch;
      ++s;
    }
    char* temp = extend(str, s);
    if (temp == nullptr) {
      free(str);
      if (is_skipws) {
        in >> std::skipws;
      }
      return nullptr;
    }
    free(str);
    str = temp;
    str[s] = '\0';
    if (is_skipws) {
      in >> std::skipws;
    }
    return str;
  }
  char* repsym(const char* str, char* data)
  {
    constexpr size_t MAX_ascii = 256;
    if (!str || !data) {
      return nullptr;
    }

    int repeat[MAX_ascii] = {};
    for (size_t i = 0; str[i] != '\0'; ++i) {
      unsigned char c = static_cast< unsigned char >(str[i]);
      ++repeat[c];
    }

    int saw[MAX_ascii] = {};
    size_t pos = 0;
    for (size_t i = 0; str[i] != '\0'; ++i) {
      unsigned char c = static_cast< unsigned char >(str[i]);
      if (repeat[c] > 1 && saw[c] == '\0') {
        data[pos++] = c;
        saw[c] = 1;
      }
    }
    data[pos] = '\0';
    return data;
  }
  char* unitwo(const char* str1, const char* str2, size_t s1, size_t s2, char* data)
  {
    if (!str1 || !str2 || !data) {
      return nullptr;
    }
    size_t i = 0, j = 0, k = 0;
    while (i < s1 && j < s2) {
      data[k++] = str1[i++];
      data[k++] = str2[j++];
    }
    while (i < s1) {
      data[k++] = str1[i++];
    }
    while (j < s2) {
      data[k++] = str2[j++];
    }
    data[k] = '\0';
    return data;
  }
}

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
