#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

namespace alisov 
{
   
}

bool isdig(char * argv);
int sti(char * n);

int main(int argc, char ** argv)
{
    if(argc<4)
    {
      std::cerr<<"Not enough arguments \n";
      return 1;
    }
    if(argc>4)
    {
      std::cerr<<"Too many arguments \n";
      return 1;
    }
    if(!isdig(argv[1]))
    {
      std::cerr<<"first parameter is not a number \n";
      return 1;
    }
    int n = 0;
    try
    {
      n = sti(argv[1]);
    } catch(const std::overflow_error &e)
    {
      std::cerr<<e.what() << "\n";
      return 1;
    }
    if(n!=1 || n!=2)
    {
      std::cerr<<"First parameter is out of range \n";
      return 1;
    }

    size_t a,b;
    std::ifstream in(argv[2]);
    
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

    if(!(in >> a >> b))
    {
        std::cerr <<"Not correct file";
        return 2;
    }
    size_t len = a*b;
    int arr2[10000] = {};
    int *arr = new int [len];
    std::ifstream in(argv[2]);
    size_t m, n;
    in >> m >> n;

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