#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

namespace afanasev
{
  long long CNT_LOC_MIN(const long long *mtx, const size_t r, const size_t c)
  {
    if (!mtx || r < 3 || c < 3)
    {
      return 0;
    }
    bool flag = 1;
    long long n = 0;
    size_t ans = 0;

    for (size_t y = 1; y < r - 1; y++)
    {
      flag = 1;
      for (size_t x = 1; x < c - 1; x++)
      {
        n = mtx[(y * c) + (x)];
        flag = flag && (n < mtx[(y - 1) * c + (x - 1)]);
        flag = flag && (n < mtx[(y - 1) * c + (x)]);
        flag = flag && (n < mtx[(y - 1) * c + (x + 1)]);

        flag = flag && (n < mtx[(y * c) + (x - 1)]);
        flag = flag && (n < mtx[(y * c) + (x + 1)]);

        flag = flag && (n < mtx[(y + 1) * c + (x - 1)]);
        flag = flag && (n < mtx[(y + 1) * c + (x)]);
        flag = flag && (n < mtx[(y + 1) * c + (x + 1)]);

        if (flag)
        {
          ans++;
        }
      }
    }
    return ans;
  }

  long long CNT_LOC_MAX(const long long *mtx, const size_t r, const size_t c)
  {
    if (!mtx || r < 3 || c < 3)
    {
      return 0;
    }
    bool flag = 1;
    long long n = 0;
    size_t ans = 0;

    for (size_t y = 1; y < r - 1; y++)
    {
      flag = 1;
      for (size_t x = 1; x < c - 1; x++)
      {
        n = mtx[(y * c) + (x)];
        flag = flag && (n > mtx[(y - 1) * c + (x - 1)]);
        flag = flag && (n > mtx[(y - 1) * c + (x)]);
        flag = flag && (n > mtx[(y - 1) * c + (x + 1)]);

        flag = flag && (n > mtx[(y * c) + (x - 1)]);
        flag = flag && (n > mtx[(y * c) + (x + 1)]);

        flag = flag && (n > mtx[(y + 1) * c + (x - 1)]);
        flag = flag && (n > mtx[(y + 1) * c + (x)]);
        flag = flag && (n > mtx[(y + 1) * c + (x + 1)]);

        if (flag)
        {
          ans++;
        }
      }
    }
    return ans;
  }
}

int main (int argc, char ** argv)
{
  if (argc == 4)
  {
    std::string arg = argv[1];
    if (arg != "2" && arg != "1")
    {
      std::cerr << "Invalid first argument" << "\n";
      return 1;
    }

    long long r_1 = 0, c_1 = 0;
    std::ifstream input(argv[2]);
    input >> r_1 >> c_1;
    if (input.fail() || r_1 < 0 || c_1 < 0)
    {
      std::cerr << "Incorrect input" << "\n";
      return 2;
    }
    size_t r = r_1, c = c_1;

    if (arg == "2")
    {
      long long *mtx = static_cast<long long*>(malloc(r * c * sizeof(long long)));

      if (mtx == nullptr)
      {
        std::cerr << "Get memory failed" << "\n";
        return 2;
      }

      for (size_t i = 0; i < (r * c); i++)
      {
        input >> mtx[i];
        if (input.fail())
        {
          std::cerr << "Incorrect input" << "\n";
          free(mtx);
          return 2;
        }
      }

      long long min = 0;
      min = afanasev::CNT_LOC_MIN(mtx, r, c);
      long long max = 0;
      max = afanasev::CNT_LOC_MAX(mtx, r, c);

      free(mtx);

      std::ofstream output(argv[3]);
      output << min << " " << max << "\n";
      output.close();
    }
    else if (arg == "1")
    {
      long long mtx[10000] = {};

      for (size_t i = 0; i < (r * c); i++)
      {
        input >> mtx[i];
        if (input.fail())
        {
          std::cerr << "Incorrect input" << "\n";
          return 2;
        }
      }
      long long min = 0;
      min = afanasev::CNT_LOC_MIN(mtx, r, c);
      long long max = 0;
      max = afanasev::CNT_LOC_MAX(mtx, r, c);

      std::ofstream output(argv[3]);
      output << min << " " << max << "\n";
      output.close();
    }
    input.close();
  }
  else
  {
    std::cerr << "Incorrect quantity of arguments" << "\n";
    return 1;
  }
  return 0;
}
