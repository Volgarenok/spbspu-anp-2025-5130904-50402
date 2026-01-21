#ifndef STRINGOPS_HPP
#define STRINGOPS_HPP

#include <iostream>
#include <cstddef>

namespace pozdnyakov
{
  constexpr size_t ALPHABET_SIZE = 26;

  void fillCharMap(const char* str, bool* map);
  char* readString(std::istream& in, size_t& size);
  void replaceChars(const char* input, char* output, char oldChar, char newChar);
  void mergeLatinLetters(const char* s1, const char* s2, char* dest);
}

#endif
