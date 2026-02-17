#include "variants.hpp"
#include "massive.hpp"

void shirokov::spiral(int *matrix, size_t m, size_t n)
{
  if (m == 0 || n == 0)
  {
    return;
  }
  size_t ptr = matrix[transformIndexes(m - 1, 0, n)];
  size_t leftBorder = 0;
  size_t rightBorder = n - 1;
  size_t upperBorder = 0;
  size_t lowerBorder = m - 1;

  size_t deductible = 1;
  while (leftBorder < n || upperBorder < m || rightBorder > 0 || lowerBorder > 0)
  {
    for (size_t i = lowerBorder; i + 1 >= upperBorder + 1; i--)
    {
      ptr = transformIndexes(i, leftBorder, n);
      matrix[ptr] -= deductible++;
    }
    if (leftBorder < n)
    {
      leftBorder++;
    }

    for (size_t j = leftBorder; j <= rightBorder; j++)
    {
      ptr = transformIndexes(upperBorder, j, n);
      matrix[ptr] -= deductible++;
    }
    if (upperBorder < m)
    {
      upperBorder++;
    }

    for (size_t i = upperBorder; i <= lowerBorder; i++)
    {
      ptr = transformIndexes(i, rightBorder, n);
      matrix[ptr] -= deductible++;
    }
    if (rightBorder > 0)
    {
      rightBorder--;
    }

    for (size_t j = rightBorder; j + 1 >= leftBorder + 1; j--)
    {
      ptr = transformIndexes(lowerBorder, j, n);
      matrix[ptr] -= deductible++;
    }
    if (lowerBorder > 0)
    {
      lowerBorder--;
    }
  }
}

bool shirokov::isTriangularMatrix(const int *matrix, size_t m, size_t n)
{
  if (m == 0 || n == 0)
  {
    return false;
  }
  size_t minn = m < n ? m : n;
  for (size_t i = 0; i < minn - 1; ++i)
  {
    for (size_t j = i + 1; j < minn; ++j)
    {
      if (matrix[transformIndexes(i, j, n)] != 0)
      {
        return false;
      }
    }
  }
  return true;
}
