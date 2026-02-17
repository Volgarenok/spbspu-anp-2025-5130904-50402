#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstddef>
#include "getline.hpp"

namespace shirokov
{
  constexpr size_t LATIN_ALPHABET_LENGTH = 26;
  constexpr int CASE_DELTA = 32;
  constexpr char LITERAL[] = "def ";
  char *uniq(char *res, const char *str, size_t &rsize);
  char *otherLatinLetters(const char *str, char *res, char *buffer);
  char *combineLines(const char *str1, size_t s1, const char *str2, size_t s2, char *res);
  bool isSpace(char symbol);
}

int main()
{
  size_t s = 0;
  char **massive = shirokov::getline(std::cin, s, shirokov::isSpace);

  if (massive == nullptr || s == 0)
  {
    free(massive);
    std::cerr << "Couldn't read the line\n";
    return 1;
  }

  for (size_t i = 0; i < s; ++i)
  {
    size_t length = 0;
    for (; massive[i][length] != '\0'; ++length)
    {
    }
    char *res1 = reinterpret_cast< char * >(malloc((shirokov::LATIN_ALPHABET_LENGTH + 1) * sizeof(char)));
    char *res2 = reinterpret_cast< char * >(malloc((std::strlen(shirokov::LITERAL) + length + 1) * sizeof(char)));
    if (res1 == nullptr || res2 == nullptr)
    {
      for (size_t j = 0; j < s; ++j)
      {
        free(massive[j]);
      }
      free(massive);
      free(res1);
      free(res2);
      std::cerr << "Memory allocation error\n";
      return 1;
    }
    char *buffer = reinterpret_cast< char * >(malloc(sizeof(char) * (length + 1)));
    if (buffer == nullptr)
    {
      for (size_t j = 0; j < s; ++j)
      {
        free(massive[j]);
      }
      free(massive);
      free(res1);
      free(res2);
      std::cerr << "Memory allocation error\n";
      return 1;
    }
    res1[shirokov::LATIN_ALPHABET_LENGTH] = '\0';
    res2[std::strlen(shirokov::LITERAL) + length] = '\0';
    buffer[length] = '\0';
    res1 = shirokov::otherLatinLetters(massive[i], res1, buffer);
    res2 = shirokov::combineLines(massive[i], length, shirokov::LITERAL, std::strlen(shirokov::LITERAL), res2);

    std::cout << "String: " << massive[i] << '\n';
    std::cout << "\t1. " << res1 << '\n';
    std::cout << "\t2. " << res2 << '\n';
    free(res1);
    free(res2);
    free(buffer);
  }
  for (size_t i = 0; i < s; ++i)
  {
    free(massive[i]);
  }
  free(massive);
}

char *shirokov::otherLatinLetters(const char *str, char *res, char *buffer)
{
  size_t rsize = 0;
  buffer = uniq(buffer, str, rsize);
  size_t pos = 0;
  for (char letter = 'a'; letter <= 'z'; ++letter)
  {
    bool in_str = false;
    for (size_t i = 0; i < rsize; ++i)
    {
      if (buffer[i] == letter)
      {
        in_str = true;
        break;
      }
    }
    if (!in_str)
    {
      res[pos++] = letter;
    }
  }
  res[pos] = '\0';
  return res;
}

char *shirokov::combineLines(const char *str1, size_t s1, const char *str2, size_t s2, char *res)
{
  size_t minn = s1 < s2 ? s1 : s2;
  for (size_t i = 0; i < minn; ++i)
  {
    res[2 * i] = str1[i];
    res[2 * i + 1] = str2[i];
  }
  const char *maxString = s1 > s2 ? str1 : str2;
  for (size_t i = minn * 2; i < s1 + s2; ++i)
  {
    res[i] = maxString[i - minn];
  }
  return res;
}

char *shirokov::uniq(char *res, const char *str, size_t &rsize)
{
  rsize = 0;
  for (size_t i = 0; str[i] != '\0'; ++i)
  {
    bool in_res = false;
    char temp = str[i];
    if ('A' <= temp && temp <= 'Z')
    {
      temp += CASE_DELTA;
    }
    for (size_t j = 0; j < rsize; ++j)
    {
      if (temp == res[j])
      {
        in_res = true;
      }
    }
    if (!in_res)
    {
      res[rsize++] = temp;
    }
  }
  return res;
}

bool shirokov::isSpace(char symbol)
{
  if (symbol == ' ' || symbol == '\t' || symbol == '\n')
  {
    return true;
  }
  return false;
}
