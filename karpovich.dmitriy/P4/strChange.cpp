#include "strChange.hpp"
#include <iostream>
char* karpovich::repsym(const char* str, char* data)
{
  constexpr size_t MAX_ascii = 256;
  if (!str || !data) {
    return nullptr;
  }

  int repeat[MAX_ascii] = {};
  for (size_t i = 0; str[i] != '\0'; ++i) {
    unsigned char c = static_cast< unsigned char >(str[i]);
    ++repeat[c];
  }

  int saw[MAX_ascii] = {};
  size_t pos = 0;
  for (size_t i = 0; str[i] != '\0'; ++i) {
    unsigned char c = static_cast< unsigned char >(str[i]);
    if (repeat[c] > 1 && saw[c] == '\0') {
      data[pos++] = c;
      saw[c] = 1;
    }
  }
  data[pos] = '\0';
  return data;
}
char* karpovich::unitwo(const char* str1, const char* str2, size_t s1, size_t s2, char* data)
{
  if (!str1 || !str2 || !data) {
    return nullptr;
  }
  size_t i = 0, j = 0, k = 0;
  while (i < s1 && j < s2) {
    data[k++] = str1[i++];
    data[k++] = str2[j++];
  }
  while (i < s1) {
    data[k++] = str1[i++];
  }
  while (j < s2) {
    data[k++] = str2[j++];
  }
  data[k] = '\0';
  return data;
}
