#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <typeinfo>
#include <cstddef>
#include <cstdlib>

namespace muhamadiarov
{
  struct matric_info
  {
    int* start;
    int rows;
    int colons;
  };

  int max_int();
  int min_int();

  int* fll_inc_wav(matric_info matrix);

  void out(int* begin, matric_info matrix, const char* output, int* res1, long long int res2);

  long long toFindMaxRight(int* ptr, int order);
  long long toFindMaxinLeft(int* ptr, int order);
  long long max_sum_mdg(matric_info matrix, int order);

  bool tocheckparam(int argc, char* argv[]);
  void tocoutargv(int argc, char * argv[]);
}

bool isOutfileOpened = true;

int main(int argc, char *argv[])
{
  namespace muh = muhamadiarov;
  bool checkparam = muh::tocheckparam(argc, argv); //проверка параметров main
  if (!checkparam)
  {
    return 1;
  }
  std::ifstream input(argv[2]);
  //открылся ли файл?
  if (!input)
  {
    std::cerr << "Error is openning file\n";
    return 2;
  }

  //считаем количество элементов в файле
  //и проверяем: достаточно ли количество элементов
  int number = 0;
  size_t count = 0;
  while (input >> number)
  {
    ++count;
  }

  if (count == 0)
  {
    std::cerr << "The file is empty\n";
    return 2;
  }
  else if (count < 2)
  {
    std::cerr << "Not enough vaules\n";
    return 2;
  }

  input.clear();
  input.seekg(0);

  //выбор метода хранения данных
  int* ptr = nullptr;
  int arr[10000];
  if (argv[1][0] == '1')
  {
    ptr = &arr[0];
  }
  else if (argv[1][0] == '2')
  {
    ptr = static_cast<int*>(malloc(count * sizeof(int)));
    if (ptr == nullptr)
    {
      std::cerr << "Failed to allocate memory\n";
      return 1;
    }
  }
  // для вывода исходной матрицы
  int copy[count];
  int* beginCopy = copy;

  size_t k = 0;
  int k1 = 0; // для отслеживания ситуации(чтение r and c)
  int r = 0, c = 0;
  while (input >> number)
  {
    k1 += 1;
    //проверяем элемент файла
    bool ch1 =  typeid(number).name() != typeid(int).name();
    bool ch2 = ch1 || number < muh::min_int() || number > muh::max_int();
    if (ch2)
    {
      std::cerr << "Incorrect type for number\n";
      if (argv[1][0] == '2')
      {
        free(ptr);
      }
      return 2;
    }


    if (k1 == 1)
    {
      r = number;
    }
    else if (k1 == 2)
    {
      c = number;
    }
    if (input.peek() == '\n') // когда дошли до конца строки
    {
      // проверяем: правильно ли даны размеры файла
      bool c1 = (k1 - 2 != r * c) && (r * c != 0);
      bool c2 = !(r == 0 &&  c == 0 && k1 == 2);
      bool c3 = (r != 0 && c == 0) || (r == 0 && c != 0);
      bool c4 = r < 0 || c < 0;
      if (((c3 || c1) && c2) || c4)
      {
        std::cerr << "Wrong size matric\n";
        if (argv[1][0] == '2')
        {
          free(ptr);
        }
        return 2;
      }
      k1 = 0;
    }

    *(beginCopy + k) = number;
    *(ptr + k) = number;
    ++k;
  }
  input.close();
  // count - count elements in file
  // r  - rows
  // c - colons

  int* result_1 = nullptr;
  long long int result_2 = 0;
  muh::matric_info matrix;
  size_t i = 0;
  while (i < count)
  {
    int r = ptr[i];
    int c = ptr[i + 1];
    i += 2;
    if (r == 0 && c == 0) // если матрица нулевая
    {
      matrix = {ptr + i - 2, 0, 0};
      muh::out(beginCopy + i - 2, matrix, argv[3], result_1, result_2);
    }
    else
    {
      matrix = {ptr + i, r, c};
      int order = std::min(r, c);
      result_2 = muh::max_sum_mdg(matrix, order);
      result_1 = muh::fll_inc_wav(matrix);
      muh::out(beginCopy + i, matrix, argv[3], result_1, result_2);
      i += r * c;
    }
    if (!isOutfileOpened) // если не открылся output
    {
      std::cerr << "Error in openning file\n";
      if (argv[1][0] == '2')
      {
        free(ptr);
      }
      return 1;
    }
  }
  if (argv[1][0] == '2')
  {
    free(ptr);
  }
  return 0;
}

int* muhamadiarov::fll_inc_wav(muhamadiarov::matric_info matrix)
{
  namespace muh = muhamadiarov;
  int n = matrix.rows;
  int k = matrix.colons;
  int* ptr = matrix.start;
  for (int j = 0; j < n; ++j)
  {
    for (int i = 0; i < k; ++i)
    {
      //смотрим позицию
      int p = 0;
      int top = j;
      int bottom = n - 1 - j;
      int left = i;
      int right = k - 1 - i;
      p = std::min(std::min(top, bottom), std::min(left, right)) + 1;
      *(ptr + j*k + i) += p;
    }
  }
  return ptr;
}


long long muhamadiarov::max_sum_mdg(muhamadiarov::matric_info matrix, int order)
{
  namespace muh = muhamadiarov;
  long long max_result = 0;
  //разделяем массив на две части по побочной диагонали
  //чтобы удобнее было найти
  long long par_branch_right = muh::toFindMaxRight(matrix.start, order);
  long long par_branch_left = muh::toFindMaxinLeft(matrix.start, order);
  //нам нужен максимальный из них
  max_result = std::max(par_branch_right, par_branch_left);
  return max_result;
}

long long muhamadiarov::toFindMaxinLeft(int* ptr, int order)
{
  long long max_r = 0;
  long long result = 0;
  int diag = 0;
  while (diag < order)
  {
    for (int i = diag, j = 0; i >= 0 && j <= diag; --i, ++j)
    {
      result += ptr[j * order + i];
    }
    diag += 1;
    max_r = std::max(result, max_r);
    result = 0;
  }
  return max_r;
}

long long muhamadiarov::toFindMaxRight(int* ptr, int order)
{
  long long int max_r = 0;
  long long int result = 0;
  int diag = order - 1;
  while (diag > 0)
  {
    for (int i = order - 1, j = diag; j < order && i >= diag; --i, ++j)
    {
      result += ptr[j * order + i];
    }
    diag -= 1;
    max_r = std::max(result, max_r);
    result = 0;
  }
  return max_r;
}

void muhamadiarov::out(int* begin, muhamadiarov::matric_info matrix,const char* output, int* res1, long long int res2)
{
  int n = matrix.rows;
  int k = matrix.colons;
  size_t count = n * k;
  std::ofstream outfile(output, std::ios::app);
  if (!outfile)
  {
    isOutfileOpened = false;
  }
  else
  {
    for (short j = 0; j < 2; ++j)
    {
      outfile << n << " " << k << " ";
      for (size_t i = 0; i < count; ++i)
      {
        outfile << *(begin + i) << " ";
      }
      outfile << "//Expect output (return code 0): ";
      if (j == 0)
      {
        outfile << n << ' ' << k << ' ';
        if (res1 != nullptr)
        {
          for (int j = 0; j < n; ++j)
          {
            for (int i = 0; i < k; ++i)
            {
              outfile << res1[j*k + i] << " ";
            }
          }
        }
        outfile << "\n";
      }
      else
      {
        outfile << res2 << "\n";
      }
    }
    outfile.close();
  }
}


void muhamadiarov::tocoutargv(int argc, char* argv[])
{
  for (int i = 0; i < argc; ++i)
  {
    std::cout << argv[i] << " ";
  }
}

bool muhamadiarov::tocheckparam(int argc, char* argv[])
{
  namespace muh = muhamadiarov;
  if (argc < 4)
  {
    muh::tocoutargv(argc, argv);
    std::cerr << "//Not enough arguments\n";
    return false;
  }
  if (argc > 4)
  {
    muh::tocoutargv(argc, argv);
    std::cerr << "//Too many arguments\n";
    return false;
  }
  if (argv[1][0] < '0' || argv[1][0] > '9')
  {
    muh::tocoutargv(argc, argv);
    std::cerr << "//First parameter is not a number\n";
    return false;
  }
  else if (argv[1][0] > '2')
  {
    muh::tocoutargv(argc, argv);
    std::cerr << "//First parameter is out of range\n";
    return false;
  }
  return true;
}


int muhamadiarov::max_int()
{
  using namespace std;
  return numeric_limits<int>::max();
}

int muhamadiarov::min_int()
{
  using namespace std;
  return numeric_limits<int>::min();
}

