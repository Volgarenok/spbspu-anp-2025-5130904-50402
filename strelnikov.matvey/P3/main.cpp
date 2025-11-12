#include <iostream>
#include <fstream>

namespace strelnikov {

  void rm(int** a, size_t r)
  {
    if (!a) {
      return;
    }
    for (size_t i = 0; i < r; ++i) {
      delete[] a[i];
    }
    delete[] a;
  }

  void staticmtx(std::ifstream& out, int** a, size_t r, size_t c)
  {
    size_t cnt = 0;
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        out >> a[i][j];
        if (out.eof() || out.fail()) {
          throw std::logic_error("Bad input\n");
        }
        ++cnt;
      }
    }

    if (cnt != c * r) {
      throw std::logic_error("Wrong format\n");
    }
  }

  int** dynamicmtx(std::ifstream& out, size_t r, size_t c)
  {
    int** a = nullptr;
    size_t cnt = 0;
    try {
      a = new int*[r];
      for (size_t i = 0; i < r; ++i) {
        a[i] = new int[c];
      }
    } catch (const std::bad_alloc& e) {
      rm(a, r);
      throw;
    }

    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        out >> a[i][j];
        if (out.fail()) {
          rm(a, r);
          throw std::logic_error("Bad input\n");
        }
        ++cnt;
      }
    }
    if (cnt != c * r) {
      throw std::logic_error("Wrong format\n");
    }
    return a;
  }

  void lftBotCnt(int** mtx, size_t r, size_t c)
  {
    if (r == 0 || c == 0) {
      return;
    }
    size_t top = 0, left = 0;
    size_t bot = r - 1, right = c - 1;
    int cnt = 1;

    while (left <= right && top <= bot) {
      for (size_t j = left; j <= right; ++j) {
        mtx[bot][j] += cnt++;
      }
      if (bot == 0) {
        break;
      }
      --bot;

      if (left <= right) {
        for (size_t i = bot + 1; i-- > top;) {
          mtx[i][right] += cnt++;
          if (i == 0) {
            break;
          }
        }
        if (right == 0) {
          break;
        }
        --right;
      }

      if (top <= bot && left <= right) {
        for (size_t j = right + 1; j-- > left;) {
          mtx[top][j] += cnt++;
          if (j == 0) {
            break;
          }
        }
        ++top;
      }

      if (left <= right && top <= bot) {
        for (size_t i = top; i <= bot; ++i) {
          mtx[i][left] += cnt++;
        }
        ++left;
      }
    }
  }

  size_t cntNzrDig(int** mtx, size_t r, size_t c)
  {
    size_t min = (r < c) ? r : c;
    size_t cnt = 0;
    for (size_t i = 1; i < min; ++i) {
      int tr = 1;
      size_t ecri = i;
      for (size_t j = 0; j < min - i; ++j) {
        if (mtx[j][ecri] == 0) {
          tr = 0;
        }
        ++ecri;
      }
      cnt += tr;
    }
    for (size_t i = 1; i < min; ++i) {
      size_t ecri = i;
      int tr = 1;
      for (size_t j = 0; j < min - i; ++j) {
        if (mtx[ecri][j] == 0) {
          tr = 0;
        }
        ++ecri;
      }
      cnt += tr;
    }
    return cnt;
  }

  void doall(int pr, const char* outf, const char* inf)
  {
    std::ifstream out(outf);
    if (!out.is_open()) {
      throw std::logic_error("Cannot open input file\n");
    }

    size_t r, c;
    out >> r >> c;
    if (out.fail()) {
      throw std::logic_error("Bad rows and cols file input\n");
    }

    int** mtx = 0;

    try {
      if (pr == 1) {
        static int static_mtx[1000][1000];
        if (r > 1000 || c > 1000) {
          throw std::logic_error("Matrix too large\n");
        }
        int* rows[1000];
        for (size_t i = 0; i < r; ++i) {
          rows[i] = static_mtx[i];
        }

        staticmtx(out, rows, r, c);
        mtx = rows;
        out.close();

        std::ofstream in(inf);
        if (!in.is_open()) {
          throw std::logic_error("Cannot open output file");
        }

        size_t dig = cntNzrDig(rows, r, c);
        in << dig << '\n';
        lftBotCnt(rows, r, c);
        for (size_t i = 0; i < r; ++i) {
          for (size_t j = 0; j < c; ++j)
            in << rows[i][j] << ' ';
          in << '\n';
        }
        in.close();
        return;
      } else {
        mtx = dynamicmtx(out, r, c);
        out.close();

        std::ofstream in(inf);
        if (!in.is_open()) {
          throw std::logic_error("Cannot open output file");
        }
        size_t dig = cntNzrDig(mtx, r, c);
        in << dig << '\n';
        lftBotCnt(mtx, r, c);
        for (size_t i = 0; i < r; ++i) {
          for (size_t j = 0; j < c; ++j) {
            in << mtx[i][j] << ' ';
          }
          in << '\n';
        }
        in.close();
        rm(mtx, r);
      }
    } catch (const std::exception& e) {
      if (pr == 2) {
        rm(mtx, r);
      }
      throw;
    }
  }

} // namespace strelnikov
int main(int argc, char const* argv[])
{
  int ex_code = 0;
  if (argc != 4) {
    if (argc > 4) {
    } else {
      std::cerr << "Too few arguments\n";
    }
    ex_code = 1;
  } else {
    int pr;
    try {
      pr = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
      std::cerr << "First argument is not a number\n";
      ex_code = 1;
    } catch (const std::out_of_range& e) {
      std::cerr << "First argument is out of range\n";
      ex_code = 1;
    }
    if (ex_code == 0) {
      if (pr != 1 && pr != 2) {
        std::cerr << "First argument is out of range\n";
        ex_code = 1;
      }
    }
    if (ex_code == 0) {
      try {
        strelnikov::doall(pr, argv[2], argv[3]);
      } catch (const std::exception& e) {
        std::cerr << e.what();
        ex_code = 2;
      }
    }
  }
  return ex_code;
}
