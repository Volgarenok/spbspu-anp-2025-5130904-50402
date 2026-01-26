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
  char* removeLL(const char* inputString, char* resBuffer, size_t bufferSize) noexcept
  {
    if (inputString == nullptr || resBuffer == nullptr || bufferSize == 0) {
      return nullptr;
    }
    size_t resIndex = 0;
    for (const char* src = inputString; *src != '\0'; ++src) {
      if (!std::isalpha(static_cast<unsigned char>(*src))) {
        if (resIndex >= bufferSize - 1) {
          return nullptr;
        }
        resBuffer[resIndex] = *src;
        ++resIndex;
      }
    }
    resBuffer[resIndex] = '\0';
    return resBuffer;
  }
  char* getline(std::istream& in)
  {
    char* buffer = nullptr;
    size_t size = 0;
    size_t len = 0;
    int ch;
    while ((ch = in.get()) != EOF && ch != '\n') {
      if (len + 1 >= size) {
        size = size == 0 ? 32 : size * 2;
        char* new_buf = static_cast<char*>(std::realloc(buffer, size));
        if (!new_buf) {
          std::free(buffer);
          return nullptr;
        }
        buffer = new_buf;
      }
      buffer[len++] = static_cast<char>(ch);
    }
    if (buffer) {
      buffer[len] = '\0';
    } else if (ch == EOF && len == 0) {
      return nullptr;
    }
    return buffer;
  }
}
int main()
{
  char* inputLine = islamov::getline(std::cin);
  if (inputLine == nullptr) {
    std::cerr << "Error reading input" << '\n';
    return 1;
  }
  size_t inputLen = std::strlen(inputLine);
  char* resBuffer1 = static_cast<char*>(std::malloc(inputLen + 1));
  char* resBuffer2 = static_cast<char*>(std::malloc(inputLen + 1));
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
  char* result2 = islamov::removeLL(inputLine, resBuffer2, inputLen + 1);
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
