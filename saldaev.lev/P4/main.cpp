#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>

namespace saldaev
{
  const size_t block_size = 10;

  char *getLine(std::istream &in, size_t &len, const size_t block_size = block_size)
  {
    char *data = reinterpret_cast< char * >(malloc((block_size + 1) * sizeof(char)));
    if (data == nullptr) {
      return nullptr;
    }
    bool skipws_on = in.flags() & std::ios_base::skipws;
    if (skipws_on) {
      in >> std::noskipws;
    }

    size_t capacity = block_size;
    len = 0;
    char crnt_char = ' ';
    while (in >> crnt_char && crnt_char != '\r' && crnt_char != '\n') {
      len++;
      if (len > capacity) {
        char *tmp = reinterpret_cast< char * >(malloc((capacity + block_size + 1) * sizeof(char)));
        if (tmp == nullptr) {
          free(data);
          if (skipws_on) {
            in >> std::skipws;
          }
          return nullptr;
        }
        capacity += block_size;
        for (size_t i = 0; i < len - 1; ++i) {
          tmp[i] = data[i];
        }
        free(data);
        data = tmp;
      }
      data[len - 1] = crnt_char;
    }
    if (skipws_on) {
      in >> std::skipws;
    }
    if (crnt_char == '\r' || crnt_char == '\n' || in.eof()) {
      data[len] = '\0';
      return data;
    }
    free(data);
    return nullptr;
  }

  char *spcRmv(const char *data, char *new_arr)
  {
    if (new_arr == nullptr) {
      return nullptr;
    }
    size_t crnt_digit = 0;
    size_t i = 0;
    char crnt_char = data[i];
    char prev_char = ' ';
    while (crnt_char != '\0') {
      if (crnt_char != ' ' || prev_char != ' ') {
        new_arr[crnt_digit] = crnt_char;
        crnt_digit++;
      }
      prev_char = crnt_char;
      i++;
      crnt_char = data[i];
    }
    new_arr[crnt_digit] = '\0';
    return new_arr;
  }

  char *latRmv(const char *data, char *new_arr)
  {
    if (new_arr == nullptr) {
      return nullptr;
    }
    size_t crnt_digit = 0;
    size_t i = 0;
    char crnt_char = data[i];
    while (crnt_char != '\0') {
      if (!std::isalpha(crnt_char)) {
        new_arr[crnt_digit] = crnt_char;
        crnt_digit++;
      }
      i++;
      crnt_char = data[i];
    }
    new_arr[crnt_digit] = '\0';
    return new_arr;
  }
}

int main()
{
  size_t len = 0;
  char *line = saldaev::getLine(std::cin, len);
  if (line == nullptr) {
    std::cerr << "Could not read the string\n";
    return 1;
  }
  if (line[0] == '\0') {
    std::cerr << "Empty line\n";
    free(line);
    return 1;
  }

  char *new_line = reinterpret_cast< char * >(malloc((len + 1) * sizeof(char)));
  if (new_line == nullptr) {
    std::cerr << "Failed to allocate memory\n";
    free(line);
    return 1;
  }
  new_line[len] = '\0';

  new_line = saldaev::spcRmv(line, new_line);
  std::cout << new_line << "\n";
  free(new_line);

  new_line = reinterpret_cast< char * >(malloc((len + 1) * sizeof(char)));
  if (new_line == nullptr) {
    std::cerr << "Failed to allocate memory\n";
    free(line);
    return 1;
  }
  new_line[len] = '\0';

  new_line = saldaev::latRmv(line, new_line);
  std::cout << new_line << "\n";
  free(line);
  free(new_line);
}
