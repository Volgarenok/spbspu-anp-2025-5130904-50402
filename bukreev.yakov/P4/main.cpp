#include <iostream>
#include <cstdlib>
#include <cstring>

namespace bukreev
{
  constexpr size_t initialSize = 4;

  char* inputString(std::istream& in, size_t& len);

  char* growString(const char* oldStr, size_t& capacity);

  char* excsnd(const char* first, const char* second, char* resStr);
  char* latrmv(const char* str, char* resStr);
}

int main()
{
  size_t len = 0;

  char* str = bukreev::inputString(std::cin, len);
  if (!str)
  {
    std::cerr << "Not enough memory for string input.\n";
    return 1;
  }
  if (len == 0)
  {
    free(str);
    std::cerr << "The string is empty.\n";
    return 2;
  }

  size_t allocSize = (len + 1) * sizeof(char);

  char* res1 = reinterpret_cast< char* >(malloc(allocSize));
  if (!res1)
  {
    std::cerr << "Not enough memory for EXC_SND.\n";
    free(str);
    return 1;
  }
  res1[len] = '\0';

  char* res2 = reinterpret_cast< char* >(malloc(allocSize));
  if (!res2)
  {
    std::cerr << "Not enough memory for LAT_RMV.\n";
    free(str);
    free(res1);
    return 1;
  }
  res2[len] = '\0';

  const char* stringToExclude = "abc";

  res1 = bukreev::excsnd(str, stringToExclude, res1);
  res2 = bukreev::latrmv(str, res2);

  free(str);

  std::cout << res1 << '\n';
  std::cout << res2 << '\n';

  free(res1);
  free(res2);
}

char* bukreev::inputString(std::istream& in, size_t& len)
{
  size_t capacity = initialSize;

  char* buffer = reinterpret_cast< char* >(malloc(initialSize * sizeof(char)));
  if (!buffer)
  {
    return nullptr;
  }

  in >> std::noskipws;

  size_t size;
  char c = ' ';
  for (size = 0; in && c != '\0'; size++)
  {
    if (size >= capacity)
    {
      char* oldBuffer = buffer;
      buffer = growString(buffer, capacity);
      free(oldBuffer);

      if (!buffer)
      {
        in >> std::skipws;
        return nullptr;
      }
    }

    in >> c;
    c = (c == '\n' ? '\0' : c);
    buffer[size] = c;
  }
  size--;

  if (!in)
  {
    if (in.eof())
    {
      buffer[size] = '\0';
    }
    else
    {
      in >> std::skipws;
      free(buffer);
      return nullptr;
    }
  }

  in >> std::skipws;
  len = size;
  return buffer;
}

char* bukreev::growString(const char* oldStr, size_t& capacity)
{
  constexpr double growRatio = 1.5;
  size_t newCapacity = capacity * growRatio;

  char* newStr = reinterpret_cast< char* >(malloc(newCapacity * sizeof(char)));
  if (!newStr)
  {
    return nullptr;
  }

  std::strncpy(newStr, oldStr, capacity);
  capacity = newCapacity;

  return newStr;
}

char* bukreev::excsnd(const char* first, const char* second, char* resStr)
{
  size_t resIndex = 0;

  for (size_t i = 0; first[i] != '\0'; i++)
  {
    char c = first[i];
    bool exclude = false;

    for (size_t j = 0; second[j] != '\0'; j++)
    {
      if (second[j] == c)
      {
        exclude = true;
        break;
      }
    }

    if (!exclude)
    {
      resStr[resIndex] = c;
      resIndex++;
    }
  }

  resStr[resIndex] = '\0';

  return resStr;
}

char* bukreev::latrmv(const char* str, char* resStr)
{
  size_t resIndex = 0;

  for (size_t i = 0; str[i] != '\0'; i++)
  {
    if (!std::isalpha(str[i]))
    {
      resStr[resIndex] = str[i];
      resIndex++;
    }
  }

  resStr[resIndex] = '\0';

  return resStr;
}
