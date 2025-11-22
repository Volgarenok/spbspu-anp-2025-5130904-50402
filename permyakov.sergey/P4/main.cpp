#include<iostream>

namespace permyakov
{
  void addSymToStr(char * line, const char sym, size_t & size);
  void cinStr(char * line, size_t & size);
}

int main()
{
  return 0;
}

void permyakov::addSymToStr(char * line, const char sym, size_t & size)
{
  char * newLine = new char[size + 1];
  for(size_t i = 0; i < size; ++i){
    newLine[i] = line[i];
  }
  newLine[size] = sym;
  delete[] line;
  line = newLine;
  size++;
}

void permyakov::cinStr(char * line, size_t & size)
{
  char sym = ' ';
  while (std::cin >> sym){
    addSymToStr(line, sym, size);
  }
}
