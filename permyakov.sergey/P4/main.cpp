#include<iostream>
#include<cctype>

namespace permyakov
{
  void addSymToStr(char * & line, const char sym, size_t & size);
  void cinStr(char * & line, size_t & size);
  void LAT_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine);
  void UNI_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine);
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

  char * result1 = nullptr;
  size_t resSize1 = 0;

  for (size_t i = 0; i < size1; ++i){
    if (isalpha(line1[i])){
      resSize1++;
    }
  }
  for (size_t i = 0; i < size2; ++i){
    if (isalpha(line2[i])){
      resSize1++;
    }
  }

  char * result2 = nullptr;
  size_t resSize2 = size1 + size2;

  try{
    result1 = new char[resSize1];
  } catch (std::bad_alloc()) {
    delete[] line1;
    delete[] line2;
    std::cerr << "Failure to allocate memory\n";
    return 2;
  }

  try{
    result2 = new char[resSize2];
  } catch (std::bad_alloc()) {
    std::cerr << "Failure to allocate memory\n";
    delete[] line1;
    delete[] line2;
    delete[] result1;
    return 2;
  }

  per::LAT_TWO(line1, size1, line2, size2, result1);
  per::UNI_TWO(line1, size1, line2, size2, result2);

  std::cout << result1 << '\n';
  std::cout << result2 << '\n';

  delete[] result1;
  delete[] result2;
  delete[] line1;
  delete[] line2;
}

void permyakov::addSymToStr(char * & line, const char sym, size_t & size)
{
  char * newLine = new char[size + 1];
  for (size_t i = 0; i < size; ++i){
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

void permyakov::LAT_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine)
{
  (void)line1;
  (void)size1;
  (void)line2;
  (void)size2;
  (void)newLine;
}

void permyakov::UNI_TWO(const char * line1, const size_t size1, const char * line2, const size_t size2, char * newLine)
{
  (void)line1;
  (void)size1;
  (void)line2;
  (void)size2;
  (void)newLine;
}
