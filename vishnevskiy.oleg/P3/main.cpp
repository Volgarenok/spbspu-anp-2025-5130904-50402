#include <iostream>
#include <fstream>
#include <string>

int main(){
  int num = 0, n = 0, m = 0;
  std::string inputName = "blank", outputName = "blank";
  std::cin >> num >> inputName >> outputName;
  std::fstream input;
  input.open(inputName);
  if (!input){
    std::cerr<<"File not found"<<'\n';
    return 1;
  }
  input >> n >> m;
  if (num == 1){
    int matrix[n*m];
    for (size_t i = 0; i < n*m; ++i){
      input >> matrix[i];
    }
  }
  else if (num == 2){
    int * matrix = new int[n*m];
    for (size_t i = 0; i < n*m; ++i){
      input >> matrix[i];
    }
  }
  else{
    std::cerr<<"Invalid number"<<'\n';
    return 1;
  }
}
