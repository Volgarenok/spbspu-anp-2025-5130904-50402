#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <limits>

size_t lengthInput(char *file);
void input(std::istream &in, int *m, size_t lng);
void output(std::ostream &out, const int *res1, size_t m, size_t n, bool res2);
bool isDigit(char *str);
int strToInt(char *str);
int myPow(int a, int b);
int *copy(const int *a, size_t k);
int *spiral(int *matrix, size_t m, size_t n);
bool isTriangularMatrix(int *matrix, size_t m, size_t n);
size_t transformIndexes(size_t i, size_t j);
void cutMatrix(int *matrix, size_t m, size_t n);

int main(int argc, char **argv)
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  if (argc > 4)
  {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  if (!isDigit(argv[1]))
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  int num = 0;
  try
  {
    num = strToInt(argv[1]);
  } catch (const std::overflow_error &e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  if (num != 1 && num != 2)
  {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  size_t lng = 0;
  try
  {
    lng = lengthInput(argv[2]);
  } catch (const std::logic_error &e)
  {
    std::cerr << e.what() << '\n';
    return 2;
  }
  int a[10000] = {};
  int *b = new int[lng];
  std::ifstream in(argv[2]);
  size_t m, n;
  in >> m >> n;

  int *matrix = nullptr;
  try
  {
    if (num == 1)
    {
      input(in, a, lng);
      matrix = a;
      delete[] b;
    }
    else
    {
      input(in, b, lng);
      matrix = b;
    }
  } catch (const std::logic_error &e)
  {
    std::cerr << e.what() << '\n';
    delete[] b;
    return 2;
  }
  std::ofstream out(argv[3]);
  int *res1 = spiral(matrix, m, n);
  bool res2 = isTriangularMatrix(matrix, m, n);
  output(out, res1, m, n, res2);
  delete[] res1;
  if (num == 2)
  {
    delete[] matrix;
  }
}

size_t lengthInput(char *file)
{
  std::ifstream in(file);
  size_t a, b;
  in >> a >> b;
  if (in.fail())
  {
    throw std::logic_error("Couldn't read the size of matrix");
  }
  return a * b;
}

void input(std::istream &in, int *m, size_t lng)
{
  for (size_t i = 0; i < lng; i++)
  {
    in >> m[i];
    if (in.fail())
    {
      throw std::logic_error("Couldn't read the matrix");
    }
  }
}

bool isDigit(char *str)
{
  size_t len = std::strlen(str);
  for (size_t i = 0; i < len; i++)
  {
    if (!('0' <= str[i] && str[i] <= '9'))
    {
      return false;
    }
  }
  return true;
}

int strToInt(char *str)
{
  int res = 0;
  int iLimit = std::numeric_limits<int>::max();
  size_t len = std::strlen(str);

  for (size_t i = 0; i < len; i++)
  {
    int temp = 0;
    temp = myPow(10, len - i - 1);
    if ((str[i] - '0') > iLimit / temp)
    {
      throw std::overflow_error("First parameter is out of range");
    }
    res += (str[i] - '0') * temp;
  }
  return res;
}

int myPow(int a, int b)
{
  if (a == 0 || a == 1)
  {
    return a;
  }
  int res = 1;
  for (int i = 0; i < b; i++)
  {
    if (res > std::numeric_limits<int>::max() / a)
    {
      throw std::overflow_error("First parameter is out of range");
    }
    res *= a;
  }
  return res;
}

void output(std::ostream &out, const int *res1, size_t m, size_t n, bool res2)
{
  out << "Решение варианта 1:\n";
  out << m << ' ' << n << ' ';
  for (size_t i = 0; i < m; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      out << res1[i * n + j] << ' ';
    }
  }
  out << "\nРешение варианта 2:\n" << res2;
}

int *copy(const int *a, size_t k)
{
  int *b = new int[k];
  for (size_t i = 0; i < k; ++i)
  {
    b[i] = a[i];
  }
  return b;
}

size_t transformIndexes(size_t i, size_t j, size_t n)
{
  return i * n + j;
}

int *spiral(int *matrix, size_t m, size_t n)
{
  if (m == 0 || n == 0)
  {
    return {};
  }
  int *mtx = copy(matrix, m * n);
  size_t ptr = mtx[transformIndexes(m - 1, 0, n)];
  size_t leftBorder = 0;
  size_t rightBorder = n - 1;
  size_t upperBorder = 0;
  size_t lowerBorder = m - 1;

  size_t deductible = 1;
  while (leftBorder < n || upperBorder < m || rightBorder > 0 ||
         lowerBorder > 0)
  {
    for (size_t i = lowerBorder; i + 1 >= upperBorder + 1; i--)
    {
      ptr = transformIndexes(i, leftBorder, n);
      mtx[ptr] -= deductible++;
    }
    if (leftBorder < n)
    {
      leftBorder++;
    }

    for (size_t j = leftBorder; j <= rightBorder; j++)
    {
      ptr = transformIndexes(upperBorder, j, n);
      mtx[ptr] -= deductible++;
    }
    if (upperBorder < m)
    {
      upperBorder++;
    }

    for (size_t i = upperBorder; i <= lowerBorder; i++)
    {
      ptr = transformIndexes(i, rightBorder, n);
      mtx[ptr] -= deductible++;
    }
    if (rightBorder > 0)
    {
      rightBorder--;
    }

    for (size_t j = rightBorder; j + 1 >= leftBorder + 1; j--)
    {
      ptr = transformIndexes(lowerBorder, j, n);
      mtx[ptr] -= deductible++;
    }
    if (lowerBorder > 0)
    {
      lowerBorder--;
    }
  }
  return mtx;
}

void cutMatrix(int *matrix, size_t m, size_t n)
{
  if (m == n)
  {
    return;
  }
  size_t minn = std::min(m, n);
  int *temp = new int[minn * minn];
  for (size_t i = 0; i < minn; ++i)
  {
    for (size_t j = 0; j < minn; ++j)
    {
      temp[transformIndexes(i, j, minn)] = matrix[transformIndexes(i, j, n)];
    }
  }
  delete[] matrix;
  matrix = copy(temp, minn * minn);
}

bool isTriangularMatrix(int *matrix, size_t m, size_t n)
{
  if (m == 0 || n == 0)
  {
    return false;
  }
  cutMatrix(matrix, m, n);
  bool flag = true;
  for (size_t i = 0; i < m - 1; ++i)
  {
    for (size_t j = i + 1; j < n; ++j)
    {
      if (matrix[transformIndexes(i, j, n)] != 0)
      {
        flag = false;
        break;
      }
    }
  }
  return flag;
}
