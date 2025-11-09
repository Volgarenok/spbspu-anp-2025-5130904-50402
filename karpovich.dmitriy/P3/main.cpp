#include <iostream>
#include <fstream>
namespace karpovich
{
  const size_t MAX = 10000;
  bool inputStaticFunc(std::istream& input, int* arr, size_t& rows, size_t& cols) {
    if (!(input >> rows >> cols)) {
      return false;
    }
    if (rows * cols > MAX) {
      return false;
    }
    for (size_t i = 0; i < rows * cols; ++i) {
      if (!(input >> arr[i])) {
        return false;
      }
    }
    return true;
  }
  int* inputFunc(std::istream& input, size_t& rows, size_t& cols) {
    if (!(input >> rows >> cols)) {
      return nullptr;
    }
    size_t total = rows * cols;
    int* arr = new int[total];
    for (size_t i = 0; i < total; ++i) {
      if (!(input >> arr[i])) {
        delete[] arr;
        return nullptr;
      }
    }
    return arr;
  }
  void outputFunc(std::ostream& output, size_t res1, int* res2, size_t rows, size_t cols) {
    output << res1 << '\n';
    output << rows << ' ';
    output << cols << ' ';
    for (size_t i = 0; i < rows * cols; ++i) {
      output << res2[i] << ' ';
    }
    output << '\n';
  }
  size_t locMin(const int* arrdyn, size_t rows, size_t cols) {
    if (!arrdyn || rows < 3 || cols < 3) {
      return 0;
    }
    size_t minimum = 0;
    for (size_t i = 1; i < rows - 1; ++i) {
      for (size_t j = 1; j < cols - 1; ++j) {
        size_t k = i * cols + j;
        int now = arrdyn[k];
        bool yeslocMin = (now < arrdyn[k - 1]) && (now < arrdyn[k + 1]);
        yeslocMin = yeslocMin && (now < arrdyn[k - cols]) && (now < arrdyn[k + cols]);
        yeslocMin = yeslocMin && (now < arrdyn[k - 1 + cols]);
        yeslocMin = yeslocMin && (now < arrdyn[k + 1 + cols]);
        yeslocMin = yeslocMin && (now < arrdyn[k + 1 - cols]);
        yeslocMin = yeslocMin && (now < arrdyn[k - 1 - cols]);
        if (yeslocMin) {
          minimum += 1;
        }
      }
    }
    return minimum;
  }
  int* lfttopclk(const int* arrdyn, size_t rows, size_t cols) {
    if (!arrdyn) {
      return nullptr;
    }
    size_t total = rows * cols;
    int* result = new int[total];
    for (size_t i = 0; i < total; ++i) {
      result[i] = arrdyn[i];
    }
    if (total == 0) {
      return result;
    }
    size_t top = 0, bottom = rows - 1;
    size_t left = 0, right = cols - 1;
    int current_value = 1;
    while (top <= bottom && left <= right) {
      for (size_t j = left; j <= right; ++j) {
        result[top * cols + j] -= current_value++;
      }
      top++;
      for (size_t i = top; i <= bottom; ++i) {
        result[i * cols + right] -= current_value++;
      }
      right--;
      if (top <= bottom) {
        for (size_t j = right + 1; j > left; --j) {
          result[bottom * cols + j - 1] -= current_value++;
        }
        bottom--;
      }
      if (left <= right) {
        for (size_t i = bottom + 1; i > top; --i) {
          result[(i - 1) * cols + left] -= current_value++;
        }
        left++;
      }
    }
    return result;
  }
}

int main(int argc, char ** argv) {
  if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments\n" : "Too many arguments\n");
    return 1;
  }
  if (argv[1][0] == '\0' || argv[1][1] != '\0') {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  char c = argv[1][0];
  if (c != '1' && c != '2') {
    if (c >= '0' && c <= '9') {
      std::cerr << "First parameter is out of range\n";
    } else {
      std::cerr << "First parameter is not a number\n";
    }
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Failed to open input file\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;
  namespace karp = karpovich;

  if (c == '1') {
    int arr_static[karp::MAX];
    if (!karp::inputStaticFunc(input, arr_static, rows, cols)) {
      std::cerr << "Failed to read input data\n";
      input.close();
      return 2;
    }
    input.close();

    int* res2 = karp::lfttopclk(arr_static, rows, cols);
    if (!res2) {
      std::cerr << "Failed to process matrix\n";
      return 2;
    }

    std::ofstream output(argv[3]);
    if (!output.is_open()) {
      std::cerr << "Failed to open output file\n";
      delete[] res2;
      return 2;
    }

    size_t res1 = karp::locMin(arr_static, rows, cols);
    karp::outputFunc(output, res1, res2, rows, cols);
    output.close();
    delete[] res2;
  }
  else if (c == '2') {
    int* arrdyn = karp::inputFunc(input, rows, cols);
    input.close();
    if (!arrdyn) {
      std::cerr << "Failed to read input data\n";
      return 2;
    }

    int* res2 = karp::lfttopclk(arrdyn, rows, cols);

    std::ofstream output(argv[3]);
    if (!output.is_open()) {
      std::cerr << "Failed to open output file\n";
      delete[] arrdyn;
      delete[] res2;
      return 2;
    }

    size_t res1 = karp::locMin(arrdyn, rows, cols);
    karp::outputFunc(output, res1, res2, rows, cols);
    output.close();
    delete[] arrdyn;
    delete[] res2;
  }
  return 0;
}
