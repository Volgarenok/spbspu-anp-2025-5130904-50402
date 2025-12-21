#include <iostream>
#include <iomanip>

namespace samarin {
  const char* kComparisonString = "absgd_fdsfre";
  const size_t kInitialBufferSize = 16;
  const size_t kExtendSize = 30;
  const size_t kAsciiTableSize = 256;
  const size_t kInitialResultSize = 50;
  char * getline(std::istream & in, size_t & res_size);
  char * cut(char * str, size_t size);
  char * findDifference(char * first_string, const char * second_string, size_t & s);
  char * extend(char * s1, size_t & size, const size_t k);
  char * latinVowelDelete(char * str, size_t size_before, size_t & size_after);
}


int main()
{
  size_t size = 0;
  char * first_string = nullptr;
  try {
    first_string = samarin::getline(std::cin, size);
  }
  catch (const std::exception & e) {
    return 1;
  }
  if (!first_string) {
    return 1;
  }
  const char * second_string = samarin::kComparisonString;
  size_t dif_count = 0;
  char * result1 = nullptr;
  char * result_without_vowels = nullptr;

   try {
     result1 = samarin::findDifference(first_string, second_string, dif_count);
   }
  catch (const std::exception & e) {
    delete[] first_string;
    return 1;
  }

  size_t size_without_vowels = 0;
  try {
    result_without_vowels = samarin::latinVowelDelete(first_string, size, size_without_vowels);
  }
  catch (const std::exception & e) {
    delete[] first_string;
    delete[] result1;
    return 1;
  }
  std::cout << result1 << "\n";
  std::cout << result_without_vowels << "\n";

  delete[] first_string;
  delete[] result1;
  delete[] result_without_vowels;
  return 0;
}

char * samarin::getline(std::istream & in, size_t & res_size)
{
  size_t size = kInitialBufferSize;
  res_size = size;
  char * input_string = new char[size];
  bool is_skipws = in.flags() & std::ios::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }
  size_t counter = 0;
  char character;
  while (in >> character) {
    if (counter >= size) {
      input_string = samarin::extend(input_string, size, kExtendSize);
    }

    input_string[counter] = character;
    counter++;
  }
  if ((in.fail() && !in.eof()) || in.bad()) {
    delete[] input_string;
    res_size = 0;
    return nullptr;
  }
  else if (in.eof()) {
    res_size = counter;
  }

  if (res_size == 0) {
    delete[] input_string;
    return nullptr;
  }

  try {
    return samarin::cut(input_string, res_size);
  }
  catch (const std::exception & e) {
    delete[] input_string;
    throw;
  }
}

char * samarin::cut(char * str, size_t size)
{
  char * temp = new char[size+1];
  for (size_t i = 0; i < size; i++) {
    temp[i] = str[i];
  }
  temp[size] = '\0';
  delete[] str;
  return temp;
}

char * samarin::findDifference(char * first_string, const char * second_string, size_t & s)
{
  if (!first_string || !second_string) {
    return nullptr;
  }
  size_t counter = 0;
  size_t size_result = kInitialResultSize;
  char * result = new char[size_result];
  bool * check = nullptr;
  bool * check2 = nullptr;
  try {
    check = new bool[kAsciiTableSize]();
  }
  catch (const std::bad_alloc & e) {
    delete[] result;
    throw;
  }
  try {
    check2 = new bool[kAsciiTableSize]();
  }
  catch (const std::bad_alloc & e) {
    delete[] result;
    delete[] check;
    throw;
  }

  for (char * p = first_string; *p; p++) {
    check[(unsigned char)*p] = true;
  }
  for (const char * p = second_string; *p; p++) {
    check2[(unsigned char)*p] = true;
  }
  for (char * p = first_string; *p; p++) {
    if (!check2[(unsigned char)*p]) {
      if (counter < size_result) {
        result[counter] = *p;
        counter++;
      } else {
        result = samarin::extend (result, size_result, kExtendSize);
        result[counter] = *p;
        counter++;
      }
    }
  }
  for (const char * p = second_string; *p; p++) {
    if (!check[(unsigned char)*p]) {
      if (counter < size_result) {
        result[counter] = *p;
        counter++;
      } else {
        result = samarin::extend (result, size_result, kExtendSize);
        result[counter] = *p;
        counter++;
      }
    }
  }
  s = counter;
  delete[] check;
  delete[] check2;
  try {
    return samarin::cut(result, s);
  }
  catch (const std::bad_alloc & e) {
    delete[] result;
    throw;
  }
}

char * samarin::latinVowelDelete(char * str, size_t size_before, size_t & size_after)
{
  if (!str || !size_before) {
    return nullptr;
  }
  size_t counter = 0;
  const char * vowels = "AaEeIiOoUuYy";
  char * result = new char[size_before];
  bool * vowels_check = nullptr;
  try {
    vowels_check = new bool[kAsciiTableSize]();
  }
  catch (const std::bad_alloc & e) {
    delete[] result;
    throw e;
  }
  for (const char * p = vowels; *p; p++) {
    vowels_check[(unsigned char)*p] = true;
  }
  for (char * p = str; *p; p++) {
    if (vowels_check[(unsigned char)*p] == false) {
      result[counter] = *p;
      counter++;
    }
  }
  size_after = counter;

  try {
    delete[] vowels_check;
    return samarin::cut(result, size_after);
  }
  catch (const std::bad_alloc & e) {
    delete[] result;
    delete[] vowels_check;
    throw e;
  }
}

char * samarin::extend(char * s1, size_t & size, const size_t k)
{
  char * temp = new char[size+k];
  for (size_t i = 0; i < size; i++) {
    temp[i] = s1[i];
  }
  delete[] s1;
  s1 = temp;
  size = size + k;
  return s1;
}
