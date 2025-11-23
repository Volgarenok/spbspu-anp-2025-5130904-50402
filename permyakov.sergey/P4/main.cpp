#include<iostream>
#include<cctype>

namespace permyakov
{
  void addSymToStr(char * & line, const char sym, size_t & size);
  int cinStr(char * & line, size_t & size);
  void LAT_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine, const size_t size);
  void UNI_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine);
}

int main()
{
  char * line1 = nullptr;
  size_t size1 = 0;
  const char * line2l = "def_ghk";
  const size_t size2l = 7;
  const char * line2u = "def_";
  const size_t size2u = 4;

  try {
    line1 = new char[size1 + 1];
  } catch (std::bad_alloc()) {
    std::cerr << "Failure to allocate memory\n";
    return 2;
  }

  namespace per = permyakov;
  line1[0] = '\0';
  if (per::cinStr(line1, size1)) {
    std::cerr << "Failure to allocate memory\n";
    delete[] line1;
    return 2;
  }

  if (!std::cin) {
    std::cerr << "Failure to read str\n";
    delete[] line1;
    return 1;
  }

  char * result1 = nullptr;
  size_t resSize1 = 0;
  for (size_t i = 0; i < size1; ++i) {
    if (isalpha(line1[i])) {
      resSize1++;
    }
  }
  for (size_t i = 0; i < size2l; ++i) {
    if (isalpha(line2l[i])) {
      resSize1++;
    }
  }
  char * result2 = nullptr;
  size_t resSize2 = size1 + size2u;

  try {
    result1 = new char[resSize1 + 1];
  } catch (std::bad_alloc()) {
    delete[] line1;
    std::cerr << "Failure to allocate memory\n";
    return 2;
  }
  try {
    result2 = new char[resSize2 + 1];
  } catch (std::bad_alloc()) {
    std::cerr << "Failure to allocate memory\n";
    delete[] line1;
    delete[] result1;
    return 2;
  }

  result1[resSize1] = '\0';
  result2[resSize2] = '\0';

  per::LAT_TWO(line1, size1, line2l, size2l, result1, resSize1);
  per::UNI_TWO(line1, size1, line2u, size2u, result2);

  std::cout << result1 << '\n';
  std::cout << result2 << '\n';

  delete[] result1;
  delete[] result2;
  delete[] line1;
}

void permyakov::addSymToStr(char * & line, const char sym, size_t & size)
{
  char * newLine = new char[size + 2];
  for (size_t i = 0; i < size; ++i) {
    newLine[i] = line[i];
  }
  newLine[size] = sym;
  newLine[size + 1] = '\0';
  delete[] line;
  line = newLine;
  size++;
}

int permyakov::cinStr(char * & line, size_t & size)
{
  char sym = ' ';
  std::cin.get(sym);
  while (sym != '\n' && std::cin) {
    try {
      addSymToStr(line, sym, size);
    } catch (std::bad_alloc()) {
      return 1;
    }
    std::cin.get(sym);
  }
  return 0;
}

void permyakov::LAT_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine, const size_t size)
{
  size_t istr = 0;
  for (size_t i = 0; i < size1; ++i) {
    if (isalpha(line1[i])) {
      newLine[istr] = line1[i];
      istr++;
    }
  }
  for (size_t i = 0; i < size2; ++i) {
    if (isalpha(line2[i])) {
      newLine[istr] = line2[i];
      istr++;
    }
  }
  char swch = ' ';
  for (size_t i = size - 1; i > 0; --i) {
    for (size_t j = 0; j < i; ++j) {
      if (newLine[j] > newLine[j + 1]) {
        swch = newLine[j];
        newLine[j] = newLine[j + 1];
        newLine[j + 1] = swch;
      }
    }
  }
}

void permyakov::UNI_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine)
{
  size_t minSize = size1 < size2 ? size1 : size2;
  for (size_t i = 0; i < minSize; ++i) {
    newLine[i * 2] = line1[i];
  }
  for (size_t i = 0; i < minSize; ++i) {
    newLine[i * 2 + 1] = line2[i];
  }

  const char * maxLine = size1 > size2 ? line1 : line2;
  size_t maxSize = size1 > size2 ? size1 : size2;

  for (size_t i = minSize; i < maxSize; ++i) {
    newLine[minSize + i] = maxLine[i];
  }
}
