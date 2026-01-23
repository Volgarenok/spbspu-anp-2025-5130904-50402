#ifndef STRINGOPS_HPP
#define STRINGOPS_HPP

#include <iostream>
#include <cstddef>

namespace pozdnyakov
{
  constexpr size_t ALPHABET_SIZE = 26;

  bool isAlpha(char ch);
  char toLower(char ch);
  size_t getLength(const char *str);
  bool isSeparator(char ch);
  void fillCharMap(const char *str, bool *map);
  void cleanupWords(char **words, size_t count);
  char *readWord(std::istream &in, bool (*check)(char));
  char **inputString(std::istream &in, size_t &count);
  void replaceChars(const char *input, char *output, char oldChar, char newChar);
  void mergeLatinLetters(const char *s1, const char *s2, char *dest);
}

#endif
