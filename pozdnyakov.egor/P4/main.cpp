#include <iostream>
#include <stdexcept>
#include <new>
#include "stringOps.hpp"

int main()
{
  using namespace pozdnyakov;

  char **words = nullptr;
  size_t wordsCount = 0;

  try {
    words = inputString(std::cin, wordsCount);
  } catch (const std::bad_alloc &) {
    std::cerr << "Memory allocation error\n";
    if (words != nullptr) {
      cleanupWords(words, wordsCount);
    }
    return 2;
  }

  if (wordsCount == 0) {
    std::cerr << "Empty input\n";
    return 1;
  }

  const char OLD_CHAR = 'c';
  const char NEW_CHAR = 'b';
  const char *SECOND_STRING = "def_ghk";

  char *result1 = nullptr;
  char *result2 = nullptr;

  try {
    for (size_t i = 0; i < wordsCount; ++i) {
      char *currentWord = words[i];
      size_t len = getLength(currentWord);

      result1 = new char[len + 1];
      result1[len] = '\0';
      replaceChars(currentWord, result1, OLD_CHAR, NEW_CHAR);
      std::cout << result1 << '\n';
      delete[] result1;
      result1 = nullptr;

      result2 = new char[ALPHABET_SIZE + 1];
      result2[ALPHABET_SIZE] = '\0';
      mergeLatinLetters(currentWord, SECOND_STRING, result2);
      std::cout << result2 << '\n';
      delete[] result2;
      result2 = nullptr;
    }
  } catch (const std::bad_alloc &) {
    std::cerr << "Memory allocation error\n";
    delete[] result1;
    delete[] result2;
    cleanupWords(words, wordsCount);
    return 2;
  }
  cleanupWords(words, wordsCount);

  return 0;
}
