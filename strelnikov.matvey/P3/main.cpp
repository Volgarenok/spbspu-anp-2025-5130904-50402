#include <iostream>
#include <fstream>

namespace strelnikov {

  const int static_max = 10000;

  bool prValidation(const char* a)
  {
    return (a && (a[0] == '1' || a[0] == '2') && a[1] == '\0');
  }

  std::istream& input(std::istream& in, int* a, size_t r, size_t c)
  {
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        in >> a[i * c + j];
      }
    }
    return in;
  }

  std::ostream& output(std::ostream& out, const int* a, size_t r, size_t c)
  {
    out << r << ' ' << c;
    for (size_t i = 0; i < r * c; ++i) {
      out << ' ' << a[i];
    }
    return out;
  }

  void doLftBotCnt(int* mtx, size_t r, size_t c)
  {
    if (r == 0 || c == 0) {
      return;
    }
    size_t top = 0, left = 0;
    size_t bot = r - 1, right = c - 1;
    int cnt = 1;
    while (left <= right && top <= bot) {
      for (size_t j = left; j <= right; ++j) {
        mtx[bot * c + j] += cnt++;
      }
      if (bot == 0) {
        break;
      }
      --bot;

      if (left <= right) {
        for (size_t i = bot + 1; i >= top && i > 0; --i) {
          mtx[(i - 1) * c + right] += cnt++;
        }
        if (right == 0) {
          break;
        }
        --right;
      }
      if (top <= bot && left <= right) {
        for (size_t j = right + 1; j >= left && j > 0; --j) {
          mtx[top * c + j - 1] += cnt++;
        }
        ++top;
      }
      if (left <= right && top <= bot) {
        for (size_t i = top; i <= bot; ++i) {
          mtx[i * c + left] += cnt++;
        }
        ++left;
      }
    }
  }

  size_t doCntNzrDig(const int* mtx, size_t r, size_t c)
  {
    size_t min = (r < c) ? r : c;
    size_t cnt = 0;
    for (size_t i = 1; i < min; ++i) {
      int tr = 1;
      for (size_t j = 0; j < min - i; ++j) {
        if (mtx[j * c + j + i] == 0) {
          tr = 0;
          break;
        }
      }

      int tr2 = 1;
      for (size_t j = 0; j < min - i; ++j) {
        if (mtx[(j + i) * c + j] == 0) {
          tr2 = 0;
          break;
        }
      }
      cnt += tr + tr2;
    }
    return cnt;
  }

}

int main(int argc, char* argv[])
{
  if (argc > 4) {
    std::cerr << "Too many arguments\n";
    return 1;
  }
  if (argc < 4) {
    std::cerr << "Too few arguments\n";
    return 1;
  }
  if (!strelnikov::prValidation(argv[1])) {
    std::cerr << "First argument must be a number(1 or 2)\n";
    return 1;
  }
  int pr = std::atoi(argv[1]);

  std::ifstream in(argv[2]);
  if (!in.is_open()) {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  size_t r, c;
  in >> r >> c;
  if (!in) {
    std::cerr << "Bad rows and cols file input\n";
    return 2;
  }
  if ((r > 10000 || c > 10000 || r / 10000 > c) && pr == 1) {
    std::cerr << "Matrix too large\n";
    return 2;
  }

  int* mtx = nullptr;
  int static_mtx[strelnikov::static_max];
  if (pr == 1) {
    mtx = static_mtx;
  } else {
    try {
      mtx = new int[r * c];
    } catch (const std::bad_alloc& e) {
      return 2;
    }
  }

  strelnikov::input(in, mtx, r, c);
  if (in.fail()) {
    std::cerr << "Wrong format\n";
    if (pr == 2) {
      delete[] mtx;
    }
    return 2;
  }
  in.close();

  std::ofstream out(argv[3]);
  if (!out.is_open()) {
    std::cerr << "Cannot open output file\n";
    if (pr == 2) {
      delete[] mtx;
    }
    return 2;
  }
  size_t dig = strelnikov::doCntNzrDig(mtx, r, c);
  strelnikov::doLftBotCnt(mtx, r, c);
  out << dig << '\n';
  strelnikov::output(out, mtx, r, c);
  out << '\n';
  if (pr == 2) {
    delete[] mtx;
  }
}
