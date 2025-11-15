#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>


bool isDig(char * argv);
int sti(char * n);
void input(std::istream &in, int *m, size_t lng);
size_t ncl(int * arr, size_t m, size_t n);
int minSum(int * matr, size_t m,size_t n);
size_t lengthInput(char *file);


int main(int argc, char ** argv)
{
  if(argc<4)
  {
    std::cerr << "Not enough arguments \n";
    return 1;
  }
  if(argc>4)
  {
    std::cerr << "Too many arguments \n";
    return 1;
  }
  if(!isdig(argv[1]))
  {
    std::cerr << "first parameter is not a number \n";
    return 1;
  }
  int num = 0;
  try
  {
    num = sti(argv[1]);
  } catch(const std::overflow_error &e)
  {
    std::cerr<<e.what() << "\n";
    return 1;
  }
  if(num!=1 || num!=2)
  {
    std::cerr<<"First parameter is out of range \n";
    return 1;
  }

  size_t m,n;
  std::ifstream in(argv[2]);
  in >> m >> n;

  if(!in.is_open())
  {
    std::cerr<<"Not correct file";
    return 2;
  }

  if(in.peek() == std::ifstream::traits_type::eof())
  {
    std::cerr << "File is empty";
    return 2;
  }

  if(!(in >> m >> n))
  {
    std::cerr <<"Not correct file";
    return 2;
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
  int *b = new int [lng];
  std::ifstream in(argv[2]);
  int *matr = nullptr;

  try
  {
    if(num == 1)
    {
      input(in, a, lng);
      matr = a;
      delete[] b;
    }
    if(num == 2)
    {
      input(in, b, lng);
      matr = b;
    }
    } catch(std::logic_error &e)
    {
      std::cerr<<e.what()<<"\n";
      delete[] b;
      return 2;
    }
  std::ofstream out(argv[3]);
  size_t res1 = ncl(matr, m, n);
  int res2 = minSum(matr, m, n);
  std::ofstream out;
  out << "Answer for var-1: " << res1 << "\n";
  out << "Answer for var-2: " << res2 << "\n";

  if (num == 2)
  {
    delete[] matr;
  }
}

void input(std::istream &in, int *m, size_t lng)
{
  for (size_t i = 0; i < lng; i++)
  {
    in >> m[i];
    if (in.fail())
    {
      throw std::logic_error("Cant read the matrix");
    }
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

bool isdig(char * argv)
{
  size_t l = std::strlen(argv);
  for (size_t i=0;i<l;i++)
  {
    if(!('0' <= argv[i] && '9' >= argv[i]))
    {
      return false;
    }
  }
  return true;
}

int sti(char *str)
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

int minSum(int ** mtr, size_t m, size_t n)
{
  int sum;
  int min = std::numeric_limits<int>::max();
  if (m + n < 2)
  {
    return 0;
  }
  for (size_t k = 0; k <= m + n - 2; ++k)
  {
    sum = 0;
    for (size_t i = 0; i < m; ++i)
    {
      if (k - i < n) {
        sum += mtr[i][k - i];
      }
    }
    min = (sum < min) ? sum : min;
  }
  return min;
}
