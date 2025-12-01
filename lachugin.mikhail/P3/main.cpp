#include <iostream>
#include <fstream>
namespace lachugin
{
  std::istream &make(std::istream &fin, size_t rows, size_t cols, int *mtx)
  {
    for (size_t i = 0; i < rows * cols; i++)
    {
      fin >> mtx[i];
      if (fin.eof())
      {
        return fin;
      }
      if (fin.fail())
      {
        return fin;
      }
    }
    return fin;
  }
  void doLftBotClk(int *mtx, size_t rows, size_t cols)
  {
    int d = 1;
    size_t k = 1;
    size_t n = 0;
    while (k < rows * cols + 1)
    {
      for (size_t i = cols * (rows - 1 - n) + n; i > n * (cols + 1); i -= cols)
      {
        mtx[i] -= d;
        d++;
        k++;
      }
      if (k == rows * cols + 1)
      {
        break;
      }
      for (size_t i = n * (cols + 1); i <= cols * (1 + n) - n - 1; i++)
      {
        mtx[i] -= d;
        d++;
        k++;
      }
      if (k == rows * cols+1)
      {
        break;
      }
      for (size_t i = cols * (2 + n) - n - 1; i <= cols * (rows - n) - n-1; i += cols)
      {
        mtx[i] -= d;
        d++;
        k++;
      }
      if (k == rows * cols+1)
      {
        break;
      }
      for (size_t i = cols * (rows - n) - n - 2; i > cols * (rows - 1 - n) + n; i--)
      {
        mtx[i] -= d;
        d++;
        k++;
      }
      n++;
    }
  }
  void fopy(double *ptr, const int *mtx, size_t r, size_t c)
  {
    for (size_t i = 0; i < r * c; i++)
    {
      ptr[i] = mtx[i];
    }
  }
  double circle(const int *mtx, size_t i, size_t r, size_t c)
  {
    double k = 0;
    double sum = 0;
    size_t row = i / c;
    size_t col = i % c;
    if (row > 0)
    {
      if (col > 0)
      {
        size_t left_up = i - 1 - c;
        if (left_up < r * c)
        {
          k++;
          sum += mtx[left_up];
        }
      }
      size_t up = i - c;
      if (up < r * c)
      {
        k++;
        sum += mtx[up];
      }
      if (col < c - 1)
      {
        size_t right_up = i + 1 - c;
        if (right_up < r * c)
        {
          k++;
          sum += mtx[right_up];
        }
      }
    }
    if (col > 0)
    {
      size_t left = i - 1;
      if (left < r * c) {
        k++;
        sum += mtx[left];
      }
    }
    if (col < c - 1)
    {
      size_t right = i + 1;
      if (right < r * c)
      {
        k++;
        sum += mtx[right];
      }
    }
    if (row < r - 1)
    {
      if (col > 0)
      {
        size_t left_down = i - 1 + c;
        if (left_down < r * c)
        {
          k++;
          sum += mtx[left_down];
        }
      }
      size_t down = i + c;
      if (down < r * c)
      {
        k++;
        sum += mtx[down];
      }
      if (col < c - 1)
      {
        size_t right_down = i + 1 + c;
        if (right_down < r * c)
        {
          k++;
          sum += mtx[right_down];
        }
      }
    }
    double arf_mean = sum / k;
    int temp = arf_mean * 10;
    double res = temp / 10.0;
    return res;
  }
  void doBltSmtMtr(const int *mtx, size_t rows, size_t cols, double *res2)
  {
    for (size_t i = 0; i < rows * cols; i++)
    {
      res2[i] = circle(mtx, i, rows, cols);
    }
  }
  void outputForInt(std::ofstream &output, size_t rows, size_t cols, int *mtx)
  {
    output << rows << ' ' << cols;
    for (size_t i = 0; i < rows * cols; ++i)
    {
      output << ' ' << mtx[i];
    }
  }
  void outputForDouble(std::ofstream &output, size_t rows, size_t cols, double *mtx)
  {
    output << rows << ' ' << cols;
    for (size_t i = 0; i < rows * cols; ++i)
    {
      output << ' ' << mtx[i];
    }
  }
  void copy(int *ptr, const int *mtx, size_t r, size_t c)
  {
    for (size_t i = 0; i < r * c; i++)
    {
      ptr[i] = mtx[i];
    }
  }
}
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
  char *endFrstArg = nullptr;;
  long prmt = std::strtol(argv[1], std::addressof(endFrstArg), 10);
  if (!(prmt == 1 || prmt == 2) || *endFrstArg != '\0')
  {
    std::cerr << "First argument is not correct\n";
    return 1;
  }
  std::ifstream fin(argv[2]);
  if (!fin.is_open())
  {
     std::cerr << "Error opening file\n";
    return 1;
  }
  size_t rows = 0, cols = 0;
  fin >> rows >> cols;
  if (fin.fail())
  {
    std::cerr << "Error reading file\n";
    return 1;
  }
  std::ofstream output(argv[3]);
  int *res1 = nullptr;
  double *res2 = nullptr;
  int *mtx = nullptr;
  const int count = 10000;
  int arr1[count];
  double arr2[count];
  int arr3[count];
  if (prmt == 2)
  {
    try
    {
      res1 = new int[rows * cols];
      res2 = new double[rows * cols];
      mtx = new int[rows * cols];
    }
    catch (const std::bad_alloc &e)
    {
      std::cerr << e.what() << '\n';
      delete[] res1;
      delete[] res2;
      delete[] mtx;
      return 3;
    }
  }
  else
  {
    res1 = arr1;
    res2 = arr2;
    mtx = arr3;
  }
  lachugin::make(fin, rows, cols, mtx);
  if (fin.fail())
  {
    if (prmt == 2)
    {
      delete[] res1;
      delete[] res2;
      delete[] mtx;
    }
    std::cerr << "Cant read\n";
    return 2;
  }
  fin.close();
  lachugin::copy(res1, mtx, rows, cols);
  lachugin::doLftBotClk(res1, rows, cols);
  lachugin::doBltSmtMtr(mtx, rows, cols, res2);
  lachugin::outputForInt(output, rows, cols, res1);
  output << '\n';
  lachugin::outputForDouble(output, rows, cols, res2);
  output << '\n';
  output.close();
  if (prmt == 2)
    {
    delete[] res1;
    delete[] res2;
    delete[] mtx;
  }
}
