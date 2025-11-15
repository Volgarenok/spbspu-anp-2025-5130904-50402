#include <iostream>
#include <fstream>
#include <string>

struct Matrix
{
  int r;
  int c;
  int data[100][100];
};

struct MatrixD
{
  int r;
  int c;
  int** data;
};

void for_fix(std::string input_file, std::string output_file);
void for_dinamyc(const char* input, const char* output);
Matrix get_matrix(std::string input_file);
void write(int a, std::string output_file, Matrix m);
void change_matrix(Matrix m);

MatrixD get_matrix_dynamic(const char* input_file);
void free_matrix_dynamic(MatrixD& m);
int count_local_maxima_dynamic(MatrixD& m);
void change_matrix_dynamic(MatrixD& m);
void write_dynamic(int a, const char* output_file, MatrixD& m);

int main(int argc, char* argv[])
{
  if (argc > 4) {
    std::cerr << "Too many arguments";
    return 1;
  }
  else if (argc < 4) {
    std::cerr << "Not enough arguments";
    return 1;
  }
  int action = atoi(argv[1]);
  std::string input_file = argv[2];
  std::string output_file = argv[3];

  if (action == 1) {
    try {
      for_fix(input_file, output_file);
    }
    catch (const std::logic_error& e) {
      if (e.what() == std::string("cant open")) {
        std::cerr << "cant open";
        return 2;
      }
      else if (e.what() == std::string("use action 2")) {
        std::cerr << "use action 2";
        return 1;
      }
    }
  }
  else if (action == 2) {
    try {
      for_dinamyc(input_file.c_str(), output_file.c_str());
    }
    catch (const std::logic_error& e) {
      if (e.what() == std::string("cant open")) {
        std::cerr << "cant open";
        return 2;
      }
      else if (e.what() == std::string("use action 2")) {
        std::cerr << "use action 2";
        return 1;
      }
    }
  }
  else {
    std::cerr << "Invalid action. Use 1 or 2.";
    return 1;
  }
  return 0;
}

void for_fix(std::string input_file, std::string output_file)
{
  try {
    Matrix m = get_matrix(input_file);
    int point = 0;
    int answ_counter = 0;
    for (int i = 1; i < m.r - 1; ++i) {
      for (int k = 1; k < m.c - 1; ++k) {
        point = m.data[i][k];
        if (point > m.data[i - 1][k] && point > m.data[i - 1][k - 1]) {
          if (point > m.data[i][k - 1] && point > m.data[i + 1][k]) {
            if (point > m.data[i + 1][k + 1] && point > m.data[i][k + 1]) {
              if (point > m.data[i + 1][k - 1] && point > m.data[i - 1][k + 1]) {
                answ_counter++;
              }
            }
          }
        }
      }
    }
    change_matrix(m);
    write(answ_counter, output_file, m);
  }
  catch (const std::logic_error& e) {
    if (e.what() == std::string("use action 2")) {
      throw std::logic_error("use action 2");
    }
    else if (e.what() == std::string("cant open")) {
      throw std::logic_error("cant open");
    }
  }
}

void for_dinamyc(const char* input, const char* output)
{
  try {
    MatrixD m = get_matrix_dynamic(input);

    int count = count_local_maxima_dynamic(m);
    change_matrix_dynamic(m);
    write_dynamic(count, output, m);

    free_matrix_dynamic(m);
  }
  catch (const std::logic_error& e) {
    if (e.what() == std::string("cant open")) {
      throw std::logic_error("cant open");
    }
    else if (e.what() == std::string("use action 2")) {
      throw std::logic_error("cant open");
    }
  }
}

Matrix get_matrix(std::string input_file)
{
  std::ifstream input(input_file);
  if (!input) {
    throw std::logic_error("cant open");
  }
  int cols = 0;
  int rows = 0;
  int data[10000];
  std::string word;
  int counter = 0;
  while (input >> word) {
    if (counter == 0) {
      rows = std::stoi(word);
      counter++;
    }
    else if (counter == 1) {
      cols = std::stoi(word);
      counter++;
    }
    else if (counter >= 2) {
      data[counter - 2] = std::stoi(word);
      counter++;
    }
  }
  if (rows > 100 || cols > 100) {
    throw std::logic_error("use action 2");
  }
  if (rows < 3 || cols < 3) {
    std::cerr << "matrix is too small...";
    throw std::logic_error("cant open");
  }
  int true_data[100][100];
  int count = 0;
  for (int i = 0; i < rows; ++i) {
    for (int k = 0; k < cols; ++k) {
      true_data[i][k] = data[count];
      count++;
    }
  }
  Matrix result;
  result.r = rows;
  result.c = cols;
  for (int i = 0; i < rows; ++i) {
    for (int k = 0; k < cols; ++k) {
      result.data[i][k] = true_data[i][k];
    }
  }
  return result;
}

void write(int a, std::string output_file, Matrix m)
{
  std::ofstream output(output_file);
  if (!output) {
    std::cerr << "cant open";
    throw std::logic_error("cant open");
  }
  std::string text = "";
  for (int i = 0; i < m.r; ++i) {
    for (int k = 0; k < m.c; ++k) {
      text += std::to_string(m.data[i][k]);
      text += " ";
    }
    text += "\n";
  }
  output << a << "\n" << text;
  output.close();
}

void change_matrix(Matrix m)
{
  int rows = m.r;
  int cols = m.c;
  int count_r_s = 0;
  int count_c_s = 0;
  int count_c_f = cols - 1;
  int count_r_f = rows - 1;
  while (count_r_s <= count_r_f && count_c_s <= count_c_f) {
    for (int i = count_r_f; i >= count_r_s; --i) {
      m.data[i][count_c_s]--;
    }
    count_c_s++;
    if (count_c_s <= count_c_f) {
      for (int k = count_c_s; k <= count_c_f; ++k) {
        m.data[count_r_s][k]--;
      }
    }
    count_r_s++;
    if (count_r_s <= count_r_f) {
      for (int i = count_r_s; i <= count_r_f; ++i) {
        m.data[i][count_c_f]--;
      }
      count_c_f--;
    }
    if (count_c_s <= count_c_f && count_r_s <= count_r_f) {
      for (int k = count_c_f; k >= count_c_s; --k) {
        m.data[count_r_f][k]--;
      }
      count_r_f--;
    }
  }
}

MatrixD get_matrix_dynamic(const char* input_file)
{
  std::ifstream input(input_file);
  if (!input) {
    throw std::logic_error("cant open");
  }

  int rows = 0, cols = 0;
  input >> rows >> cols;

  if (rows < 3 || cols < 3) {
    std::cerr << "matrix is too small...";
    throw std::logic_error("cant open");
  }

  int** data = new int* [rows];
  for (int i = 0; i < rows; ++i) {
    data[i] = new int[cols];
    for (int j = 0; j < cols; ++j) {
      input >> data[i][j];
    }
  }
  MatrixD result;
  result.r = rows;
  result.c = cols;
  result.data = data;

  return result;
}

void free_matrix_dynamic(MatrixD& m)
{
  for (int i = 0; i < m.r; ++i) {
    delete[] m.data[i];
  }
  delete[] m.data;
  m.data = nullptr;
}

int count_local_maxima_dynamic(MatrixD& m)
{
  int count = 0;
  for (int i = 1; i < m.r - 1; ++i) {
    for (int j = 1; j < m.c - 1; ++j) {
      int center = m.data[i][j];
      bool isLocalMax = true;

      for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
          if (di == 0 && dj == 0) continue;
          if (m.data[i + di][j + dj] >= center) {
            isLocalMax = false;
            break;
          }
        }
        if (!isLocalMax) break;
      }

      if (isLocalMax) {
        ++count;
      }
    }
  }
  return count;
}

void change_matrix_dynamic(MatrixD& m)
{
  int rows = m.r;
  int cols = m.c;
  int count_r_s = 0;
  int count_c_s = 0;
  int count_c_f = cols - 1;
  int count_r_f = rows - 1;

  while (count_r_s <= count_r_f && count_c_s <= count_c_f) {
    for (int i = count_r_f; i >= count_r_s; --i) {
      m.data[i][count_c_s]--;
    }
    count_c_s++;
    if (count_c_s <= count_c_f) {
      for (int k = count_c_s; k <= count_c_f; ++k) {
        m.data[count_r_s][k]--;
      }
    }
    count_r_s++;
    if (count_r_s <= count_r_f) {
      for (int i = count_r_s; i <= count_r_f; ++i) {
        m.data[i][count_c_f]--;
      }
      count_c_f--;
    }
    if (count_c_s <= count_c_f && count_r_s <= count_r_f) {
      for (int k = count_c_f; k >= count_c_s; --k) {
        m.data[count_r_f][k]--;
      }
      count_r_f--;
    }
  }
}

void write_dynamic(int a, const char* output_file, MatrixD& m)
{
  std::ofstream output(output_file);
  if (!output) {
    std::cerr << "cant open";
    throw std::logic_error("cant open");
  }

  output << a << "\n";
  for (int i = 0; i < m.r; ++i) {
    for (int j = 0; j < m.c; ++j) {
      output << m.data[i][j] << " ";
    }
    output << "\n";
  }
  output.close();
}
