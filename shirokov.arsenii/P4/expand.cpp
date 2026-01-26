#include "expand.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>

namespace
{
  double getCoefficient(size_t capacity)
  {
    if (capacity < 10)
    {
      return 2;
    }
    if (capacity < 100)
    {
      return 1.5;
    }
    return 1.1 + 1 / std::pow(capacity + 1, 0.2);
  }
}

void shirokov::expand(char **str, size_t size, size_t &capacity)
{
  double coefficient = getCoefficient(capacity);
  size_t newCapacity = static_cast< size_t >(capacity * coefficient);
  char *tempString = reinterpret_cast< char * >(malloc(newCapacity * sizeof(char)));
  if (tempString == nullptr)
  {
    free(*str);
    *str = nullptr;
    return;
  }
  for (size_t i = 0; i < size; ++i)
  {
    tempString[i] = (*str)[i];
  }
  free(*str);
  *str = tempString;
  capacity = newCapacity;
}

void shirokov::expand(char ***str, size_t size, size_t &capacity)
{
  double coefficient = getCoefficient(capacity);
  size_t newCapacity = static_cast< size_t >(capacity * coefficient);
  char **tempMassive = reinterpret_cast< char ** >(malloc(newCapacity * sizeof(char *)));
  if (tempMassive == nullptr)
  {
    for (size_t i = 0; i < size; ++i)
    {
      free((*str)[i]);
    }
    free(*str);
    *str = nullptr;
    return;
  }
  for (size_t i = 0; i < size; ++i)
  {
    size_t len = std::strlen((*str)[i]);
    char *tempString = static_cast< char * >(malloc(len + 1));
    for (size_t j = 0; j < len; ++j)
    {
      tempString[j] = (*str)[i][j];
    }
    tempString[len] = '\0';
    free((*str)[i]);
    tempMassive[i] = tempString;
  }
  free(*str);
  *str = tempMassive;
  capacity = newCapacity;
}
