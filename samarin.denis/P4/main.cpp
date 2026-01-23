#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace samarin {
  const char* kComparisonString = "absgd_fdsfre";
  const size_t kInitialBufferSize = 16;
  const size_t kExtendSize = 30;
  const size_t kAsciiTableSize = 256;
  const size_t kInitialResultSize = 50;
  char * getline(std::istream & in, size_t & res_size, char *& input_string, size_t & capacity);
  char * cut(char * str, size_t size);
  char * findDifference(char * first_string, const char * second_string, size_t & s, char *& final_string);
  char * extend(char * s1, size_t & size, size_t k);
  char * latinVowelDelete(char * str, size_t size_before, size_t & size_after, char * final_string);
}


int main()
{
  size_t size = 0;
  size_t capacity = samarin::kInitialBufferSize;
  char * input_string = new char[capacity];
  char * first_string = nullptr;
  try {
    first_string = samarin::getline(std::cin, size, input_string, capacity);
  } catch (const std::exception & e) {
    delete[] input_string;
    return 1;
  }
  if (size == 0) {
    delete[] first_string;
    first_string = new char[1];
    first_string[0] = '\0';
  }
  if (!first_string) {
    return 1;
  }
  const char * second_string = samarin::kComparisonString;
  size_t dif_count = 0;
  char * result1 = nullptr;
  char * result_without_vowels = nullptr;

  try {
    char * final_string_diff = new char[samarin::kInitialResultSize];
    result1 = samarin::findDifference(first_string, second_string, dif_count, final_string_diff);
  } catch (const std::exception & e) {
    delete[] first_string;
    return 1;
  }

  size_t size_without_vowels = 0;
  try {
    char * final_string = new char[size];
    result_without_vowels = samarin::latinVowelDelete(first_string, size, size_without_vowels, final_string);
  } catch (const std::exception & e) {
    delete[] first_string;
    delete[] result1;
    return 1;
  }

  if (result1 != nullptr) {
    std::cout << result1 << "\n";
  }
  if (result_without_vowels != nullptr) {
    std::cout << result_without_vowels << "\n";
  }

  delete[] first_string;
  delete[] result1;
  delete[] result_without_vowels;
  return 0;
}

char * samarin::getline(std::istream & in, size_t & res_size, char *& input_string, size_t & capacity)
{
  bool is_skipws = in.flags() & std::ios::skipws;
  if (is_skipws) {
    in >> std::noskipws;
  }
  res_size = 0;
  char character;
  while (in >> character) {
    if (res_size >= capacity) {
      char* new_input_string = samarin::extend(input_string, capacity, kExtendSize);
      delete[] input_string;
      input_string = new_input_string;
    }

    input_string[res_size] = character;
    res_size++;
  }
  if ((in.fail() && !in.eof()) || in.bad()) {
    res_size = 0;
    throw std::runtime_error("Input stream error: failed to read data");
  }

  input_string[res_size] = '\0';
  return input_string;
}

char * samarin::cut(char * str, size_t size)
{
  char * temp = new char[size + 1];
  for (size_t i = 0; i < size; i++) {
    temp[i] = str[i];
  }
  temp[size] = '\0';
  return temp;
}

char * samarin::findDifference(char * first_string, const char * second_string, size_t & s, char *& final_string)
{
  if (!first_string || !second_string) {
    throw std::invalid_argument("Null pointer passed to findDifference");
  }
  size_t counter = 0;
  size_t size_result = kInitialResultSize;
  bool check[kAsciiTableSize] = {};
  bool check2[kAsciiTableSize] = {};


  for (char * p = first_string; *p; p++) {
    check[static_cast<unsigned char>(*p)] = true;
  }
  for (const char * p = second_string; *p; p++) {
    check2[static_cast<unsigned char>(*p)] = true;
  }
  for (char * p = first_string; *p; p++) {
    if (!check2[static_cast<unsigned char>(*p)]) {
      if (counter < size_result) {
        final_string[counter] = *p;
        counter++;
      } else {
        char* new_final_string = samarin::extend(final_string, size_result, kExtendSize);
        delete[] final_string;
        final_string = new_final_string;
        final_string[counter] = *p;
        counter++;
      }
    }
  }
  for (const char * p = second_string; *p; p++) {
    if (!check[static_cast<unsigned char>(*p)]) {
      if (counter < size_result) {
        final_string[counter] = *p;
        counter++;
      } else {
        char* new_final_string = samarin::extend(final_string, size_result, kExtendSize);
        delete[] final_string;
        final_string = new_final_string;
        final_string[counter] = *p;
        counter++;
      }
    }
  }
  s = counter;
  final_string[s] = '\0';
  return final_string;
}

char * samarin::latinVowelDelete(char * str, size_t size_before, size_t & size_after, char * final_string)
{
  if (!str || !size_before) {
    throw std::invalid_argument("Invalid arguments passed to latinVowelDelete");
  }
  size_t counter = 0;
  const char * vowels = "AaEeIiOoUuYy";
  bool vowels_check[kAsciiTableSize] = {};
  for (const char * p = vowels; *p; p++) {
    vowels_check[static_cast<unsigned char>(*p)] = true;
  }
  for (char * p = str; *p; p++) {
    if (vowels_check[static_cast<unsigned char>(*p)] == false) {
      final_string[counter] = *p;
      counter++;
    }
  }
  size_after = counter;

  final_string[size_after] = '\0';
  return final_string;
}

char * samarin::extend(char * s1, size_t & size, const size_t k)
{
  char * temp = new char[size+k];
  for (size_t i = 0; i < size; i++) {
    temp[i] = s1[i];
  }
  size = size + k;
  return temp;
}
