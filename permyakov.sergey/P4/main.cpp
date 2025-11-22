#include<iostream>

namespace permyakov
{
  void addSymToStr(char * & line, const char sym, size_t & size);
  void cinStr(char * & line, size_t & size);
}

int main()
{
  char * line1 = nullptr;
  size_t size1 = 0;
  char * line2 = nullptr;
  size_t size2 = 0;

  try{
    line1 = new char[size1];
  } catch (std::bad_alloc()) {
    std::cerr << "Failure to allocate memory\n";
    return 2;
  }

  try{
    line2 = new char[size2];
  } catch (std::bad_alloc()) {
    std::cerr << "Failure to allocate memory\n";
    delete[] line2;
    return 2;
  }

  namespace per = permyakov;

  try{
    per::cinStr(line1, size1);
    per::cinStr(line2, size2);
  } catch (...) {
    std::cerr << "Failure to read str\n";
    delete[] line1;
    delete[] line2;
    return 1;
  }
  std::cout << line1 << ' ' << size1 << '\n';
  std::cout << line2 << ' ' << size2 << '\n';

  delete[] line1;
  delete[] line2;
}

void permyakov::addSymToStr(char * & line, const char sym, size_t & size)
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

void permyakov::cinStr(char * & line, size_t & size)
{
  char sym = ' ';
  std::cin.get(sym);
  while (sym != '\n'){
    addSymToStr(line, sym, size);
    std::cin.get(sym);
  }
}
