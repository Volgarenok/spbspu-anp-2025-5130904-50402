#include <iostream>
#include <new>
#include <stdexcept>
#include "stringOps.hpp"

int main()
{
  using namespace pozdnyakov;

  const char OLD_CHAR = 'c';
  const char NEW_CHAR = 'b';
  const char* SECOND_STRING = "def_ghk";

  char* inputStr = nullptr;
  char* result1 = nullptr;
  char* result2 = nullptr;
  size_t inputSize = 0;

  try
  {
    inputStr = readString(std::cin, inputSize);

    if (inputStr[0] == '\0')
    {
      std::cerr << "Empty input\n";
      delete[] inputStr;
      return 1;
    }

    result1 = new char[inputSize + 1];
    result1[inputSize] = '\0';
    replaceChars(inputStr, result1, OLD_CHAR, NEW_CHAR);
    std::cout << result1 << '\n';

    result2 = new char[ALPHABET_SIZE + 1];
    result2[ALPHABET_SIZE] = '\0';
    mergeLatinLetters(inputStr, SECOND_STRING, result2);
    std::cout << result2 << '\n';

    delete[] result1;
    delete[] result2;
    delete[] inputStr;

    return 0;
  }
  catch (const std::bad_alloc&)
  {
    std::cerr << "Memory allocation error\n";

    delete[] result1;
    delete[] result2;
    delete[] inputStr;

    return 2;
  }
  catch (const std::runtime_error& e)
  {
    std::cerr << "Error: " << e.what() << '\n';

    delete[] inputStr;
    delete[] result1;
    delete[] result2;

    return 2;
  }
}
