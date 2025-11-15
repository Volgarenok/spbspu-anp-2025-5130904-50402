#include <iostream>
#include <fstream>
#include <string>

bool bottomTri(int * mt, int n){
  int zNum = 0, zRow = 0, f = 1;
  for (size_t i = 0; i < n; ++i){
    zNum = (n-(i+1));
    zRow = 0;
    for (size_t j = 0; j < n; ++j){
      if (j >= (n-zNum)){
        if (mt[i*n+j]==0){
          zRow += 1;
        }
      }
    }
    if (zNum != zRow){
      f = 0;
    }
  }
  if (f == 1){
    return true;
  }
  else{
    return false;
  }
}

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
    std::cout<<bottomTri(matrix,n)<<'\n';
  }
  else if (num == 2){
    int * matrix = new int[n*m];
    for (size_t i = 0; i < n*m; ++i){
      input >> matrix[i];
    }
    std::cout<<bottomTri(matrix,n)<<'\n';
  }
  else{
    std::cerr<<"Invalid number"<<'\n';
    return 1;
  }
}
