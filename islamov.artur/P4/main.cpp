#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>

namespace islamov
{
  char* excludeCFFS(const char* string1, const char* string2, char* resBuffer) noexcept
  {
    if (string1 == nullptr || string2 == nullptr || resBuffer == nullptr) {
      return nullptr;
    }
    size_t resIndex = 0;
    for (const char* src = string1; *src != '\0'; ++src) {
      bool shouldExclude = false;
      for (const char* search = string2; *search != '\0'; ++search) {
        if (*src == *search) {
          shouldExclude = true;
          break;
        }
      }
      if (!shouldExclude) {
        resBuffer[resIndex] = *src;
        ++resIndex;
      }
    }
    resBuffer[resIndex] = '\0';
    return resBuffer;
  }
  char* removeLL(const char* inputString, char* resBuffer) noexcept
  {
    if (inputString == nullptr || resBuffer == nullptr) {
      return nullptr;
    }
    size_t resIndex = 0;
    for (const char* src = inputString; *src != '\0'; ++src) {
      if (!std::isalpha(static_cast< unsigned char >(*src))) {
        resBuffer[resIndex] = *src;
        ++resIndex;
      }
    }
    resBuffer[resIndex] = '\0';
    return resBuffer;
  }
  char* getline(std::istream& in, size_t& length)
  {
    char* buffer = nullptr;
    size_t capacity = 0;
    size_t len = 0;
    char ch = '\0';
    while (in.get(ch) && ch != '\n') {
      if (len + 1 >= capacity) {
        size_t new_capacity = capacity == 0 ? 32 : capacity * 2;
        char* new_buf = static_cast< char* >(std::malloc(new_capacity));
        if (!new_buf) {
          std::free(buffer);
          length = 0;
          return nullptr;
        }
        if (buffer) {
          std::memcpy(new_buf, buffer, len);
          std::free(buffer);
        }
        buffer = new_buf;
        capacity = new_capacity;
      }
      buffer[len++] = ch;
    }
    if (in.eof() && len == 0) {
      length = 0;
      return nullptr;
    }
    if (buffer) {
      buffer[len] = '\0';
    }
    length = len;
    return buffer;
  }
}
int main()
{
  size_t inputLen = 0;
  char* inputLine = islamov::getline(std::cin, inputLen);
  if (inputLine == nullptr) {
    std::cerr << "Error reading input" << '\n';
    return 1;
  }
  char* resBuffer1 = static_cast< char* >(std::malloc(inputLen + 1));
  char* resBuffer2 = static_cast< char* >(std::malloc(inputLen + 1));
  if (resBuffer1 == nullptr || resBuffer2 == nullptr) {
    std::cerr << "Memory allocation failed" << '\n';
    std::free(inputLine);
    std::free(resBuffer1);
    std::free(resBuffer2);
    return 1;
  }
  resBuffer1[0] = '\0';
  resBuffer2[0] = '\0';
  char* result1 = islamov::excludeCFFS(inputLine, "abc", resBuffer1);
  char* result2 = islamov::removeLL(inputLine, resBuffer2);
  if (result1 == nullptr || result2 == nullptr) {
    std::cerr << "Error in string processing" << '\n';
    std::free(inputLine);
    std::free(resBuffer1);
    std::free(resBuffer2);
    return 1;
  }
  std::cout << result1 << '\n';
  std::cout << result2 << '\n';
  std::free(inputLine);
  std::free(resBuffer1);
  std::free(resBuffer2);
  return 0;
}
