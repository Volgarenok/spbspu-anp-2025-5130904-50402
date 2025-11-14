#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <limits>

bool is_digit(char * str);
int str_to_int(char * str);
int my_pow(int a, int b);
size_t cnt_loc_min(int * arr, size_t x, size_t y);
size_t num_col_lsr(int * arr, size_t x, size_t y);
void input_file(std::istream &in, int *m, size_t lng);

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
  if (!is_digit(argv[1]))
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  int num = 0;
  try
  {
    num = str_to_int(argv[1]);
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

  size_t x, y;

  std::ifstream input(argv[2]);

  if(!input.is_open())
  {
    std::cerr << "Uncorrect file" << "\n";
    return 2;
  }

  if (input.peek() == std::ifstream::traits_type::eof())
  {
    std::cerr << "Input file is empty" << "\n";
    return 2;
  }

  if (!(input >> x >> y))
  {
    std::cerr << "Uncorrect file" << "\n";
    return 2;
  }

  size_t total = x * y;

  int arr2[10000] = {};
  int total_for_memory = static_cast<int>(x) * static_cast<int>(y);
  int *arr = static_cast<int*>(malloc(total_for_memory * sizeof(int)));

  size_t ans_2;
  size_t ans_11;

  int *matrix = nullptr;
  try
  {
    if (num == 1)
    {
      input_file(input, arr2, total);
      matrix = arr2;
      free(arr);
    }
    else
    {
      input_file(input, arr, total);
      matrix = arr;
    }
  } catch (const std::logic_error &e)
  {
    std::cerr << e.what() << '\n';
    free(arr);
    return 2;
  }

  ans_2 = cnt_loc_min(matrix, x, y);
  ans_11 = num_col_lsr(matrix, x, y);

  std::ofstream output;
  output.open(argv[3]);

  output << "Answer for var_2: " << ans_2 << "\n";
  output << "Answer for var_11: " << ans_11 << "\n";
  if(num == 2)
  {
    free(arr);
  }
}



bool is_digit(char * str)
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

int str_to_int(char *str)
{
  int res = 0;
  int iLimit = std::numeric_limits<int>::max();
  size_t len = std::strlen(str);

  for (size_t i = 0; i < len; i++)
  {
    int temp = 0;
    temp = my_pow(10, len - i - 1);
    if ((str[i] - '0') > iLimit / temp)
    {
      throw std::overflow_error("First parameter is out of range");
    }
    res += (str[i] - '0') * temp;
  }
  return res;
}

int my_pow(int a, int b)
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

size_t cnt_loc_min(int * arr, size_t x, size_t y){

  size_t ans_2 = 0;
  size_t total = x * y;

  for (size_t i = y + 1; i < total - y; ++i)
  {
    try
    {
      if ((arr[i] < arr[i + 1]) && (arr[i] < arr[i - 1]) &&
          (arr[i] < arr[i + y]) && (arr[i] < arr[i - y]) &&
          (arr[i] < arr[i + y + 1]) && (arr[i] < arr[i - y - 1]) &&
          (arr[i] < arr[i + y - 1]) && (arr[i] < arr[i - y + 1]))
          {
        ans_2 += 1;
      }
    } catch (const std::out_of_range &)
    {
      continue;
    }
  }

  return ans_2;
}

size_t num_col_lsr(int * arr, size_t x, size_t y)
{
  int ans_11 = 0;
  size_t total = x * y;
  int max_length = 0;

  int mas[y][2];
  for (size_t i = 0; i < y; ++i)
  {
    mas[i][0] = -1;
    mas[i][1] = 0;
  }

  for (size_t i = 0; i < total; ++i)
  {
    size_t j = i % y;
    if (mas[j][0] == -1)
    {
      mas[j][0] = j + 1;
      mas[j][1] = 1;
    }else
    {
      if (arr[i] == arr[i - y])
      {
        ++mas[j][1];
      } else
      {
        if(mas[j][1] > max_length)
        {
          max_length = mas[j][1];
        }
        mas[j][1] = 1;
      }
    }
  }

  for (size_t i = 0; i < y; ++i)
  {
    if (max_length < mas[i][1])
    {
      ans_11 = mas[i][0];
      max_length = mas[i][1];
    }
  }

  return ans_11;
}

void input_file(std::istream &in, int *m, size_t lng)
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
