#include "stringOps.hpp"
#include <new>

namespace pozdnyakov
{
  bool isAlpha(char ch)
  {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
  }

  char toLower(char ch)
  {
    if (ch >= 'A' && ch <= 'Z') {
      return ch + ('a' - 'A');
    }
    return ch;
  }

  size_t getLength(const char *str)
  {
    size_t len = 0;
    while (str[len] != '\0') {
      ++len;
    }
    return len;
  }

  bool isSeparator(char ch)
  {
    return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
  }

  void fillCharMap(const char *str, bool *map)
  {
    for (size_t i = 0; str[i] != '\0'; ++i) {
      if (isAlpha(str[i])) {
        char lowerCh = toLower(str[i]);
        map[lowerCh - 'a'] = true;
      }
    }
  }

  void cleanupWords(char **words, size_t count)
  {
    for (size_t i = 0; i < count; ++i) {
      delete[] words[i];
    }
    delete[] words;
  }

  char *readWord(std::istream &in, bool (*check)(char))
  {
    size_t capacity = 16;
    size_t size = 0;
    char *str = nullptr;
    char ch;

    while (in.get(ch)) {
      if (ch == '\n') {
        return nullptr;
      }
      if (!check(ch)) {
        in.unget();
        break;
      }
    }

    if (!in) {
      return nullptr;
    }

    str = new char[capacity];

    while (in.get(ch)) {
      if (ch == '\n') {
        in.unget();
        break;
      }
      if (check(ch)) {
        break;
      }

      if (size + 1 >= capacity) {
        size_t newCapacity = capacity * 2;
        char *newStr = nullptr;
        try {
          newStr = new char[newCapacity];
        } catch (const std::bad_alloc &) {
          delete[] str;
          throw;
        }
        for (size_t i = 0; i < size; ++i) {
          newStr[i] = str[i];
        }
        delete[] str;
        str = newStr;
        capacity = newCapacity;
      }

      str[size] = ch;
      ++size;
    }

    if (size == 0) {
      delete[] str;
      return nullptr;
    }

    str[size] = '\0';
    return str;
  }

  char **inputString(std::istream &in, size_t &count)
  {
    char **words = nullptr;
    char *currentWord = nullptr;
    count = 0;

    while ((currentWord = readWord(in, isSeparator)) != nullptr) {
      char **newWords = nullptr;
      try {
        newWords = new char *[count + 1];
      } catch (const std::bad_alloc &) {
        delete[] currentWord;
        cleanupWords(words, count);
        throw;
      }

      for (size_t i = 0; i < count; ++i) {
        newWords[i] = words[i];
      }

      newWords[count] = currentWord;
      ++count;

      delete[] words;
      words = newWords;
    }
    return words;
  }

  void replaceChars(const char *input, char *output, char oldChar, char newChar)
  {
    size_t i = 0;
    while (input[i] != '\0') {
      if (input[i] == oldChar) {
        output[i] = newChar;
      } else {
        output[i] = input[i];
      }
      ++i;
    }
    output[i] = '\0';
  }

  void mergeLatinLetters(const char *s1, const char *s2, char *dest)
  {
    bool present[ALPHABET_SIZE];
    for (size_t k = 0; k < ALPHABET_SIZE; ++k) {
      present[k] = false;
    }

    fillCharMap(s1, present);
    fillCharMap(s2, present);

    size_t idx = 0;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
      if (present[i]) {
        dest[idx] = static_cast< char >('a' + i);
        ++idx;
      }
    }
    dest[idx] = '\0';
  }
}
